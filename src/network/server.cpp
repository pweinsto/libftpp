#include "../../include/network/server.hpp"

#include <iostream>
#include <thread>
#include <mutex>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <cstring>
#include <stdexcept>
#include <algorithm>

struct MsgHeader
{
	int type;
	uint64_t bodySize;
};

Server::Server()
: m_nextClientID(1)
, m_acceptThread(nullptr)
, m_acceptThreadRunning(false)
, m_listenFD(-1)
{}

Server::~Server()
{
	m_acceptThreadRunning = false;

	if (m_listenFD >= 0)
	{
		close(m_listenFD);
		m_listenFD = -1;
	}

	if (m_acceptThread && m_acceptThread->joinable())
	{
		m_acceptThread->join();
	}
	delete m_acceptThread; 
	m_acceptThread = nullptr;

	for (auto &pair : m_clientThreads)
	{
		auto it = m_clients.find(pair.first);
		if (it != m_clients.end())
		{
			close(it->second.socketFD);
		}
		if (pair.second->joinable())
			pair.second->join();
		delete pair.second;
	}
	m_clientThreads.clear();

	for (auto &c : m_clients) {
		c.second.active = false;
		if (c.second.socketFD >= 0)
		{
			close(c.second.socketFD);
		}
	}
	m_clients.clear();
}

void Server::start(const size_t& p_port)
{
	m_listenFD = socket(AF_INET, SOCK_STREAM, 0);
	if (m_listenFD < 0)
	{
		throw std::runtime_error("Server: Failed to create socket.");
	}

	int opt = 1;
	setsockopt(m_listenFD, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

	sockaddr_in serverAddr;
	std::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = INADDR_ANY;
	serverAddr.sin_port = htons(static_cast<uint16_t>(p_port));

	if (bind(m_listenFD, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
	{
		close(m_listenFD);
		m_listenFD = -1;
		throw std::runtime_error("Server: bind() failed.");
	}

	if (listen(m_listenFD, SOMAXCONN) < 0)
	{
		close(m_listenFD);
		m_listenFD = -1;
		throw std::runtime_error("Server: listen() failed.");
	}

	std::cout << "[Server] Started on port " << p_port << std::endl;

	m_acceptThreadRunning = true;
	m_acceptThread = new std::thread(&Server::acceptLoop, this);
}

void Server::acceptLoop()
{
	std::cout << "[Server] Listening for new connections...\n";
	while (m_acceptThreadRunning)
	{
		sockaddr_in clientAddr;
		socklen_t clientLen = sizeof(clientAddr);
		int clientSock = accept(m_listenFD, (sockaddr*)&clientAddr, &clientLen);
		if (clientSock < 0)
		{
			if (m_acceptThreadRunning)
			{
				std::cerr << "[Server] accept() failed.\n";
			}
			continue;
		}

		long long clientID = m_nextClientID.fetch_add(1);
		std::cout << "[Server] Accepted client with ID=" << clientID << std::endl;

		ClientInfo info;
		info.socketFD = clientSock;
		info.active = true;
		m_clients[clientID] = info;

		std::cout << "accept thread: " << m_acceptThreadRunning << std::endl;
		std::thread* t = new std::thread(&Server::clientReadLoop, this, clientID);
		m_clientThreads[clientID] = t;
	}
}

void Server::clientReadLoop(long long clientID)
{
	auto it = m_clients.find(clientID);
	if (it == m_clients.end())
	{
		return;
	}
	int sockfd = it->second.socketFD;

	while (true)
	{
		MsgHeader header;
		if (!recvAll(sockfd, reinterpret_cast<char*>(&header), sizeof(header)))
		{
			break;
		}

		Message msg(header.type);
		if (header.bodySize > 0)
		{
			std::vector<char> buffer(header.bodySize);
			if (!recvAll(sockfd, buffer.data(), header.bodySize))
			{
				break;
			}
			for (char c : buffer)
			{
				msg << c;
			}
		}

		{
			std::lock_guard<std::mutex> lock(m_incomingMutex);
			m_incoming.push_back({clientID, std::move(msg)});
		}
	}

	std::cout << "[Server] Client " << clientID << " disconnected.\n";
	close(sockfd);
	it->second.active = false;
}

void Server::defineAction(Message::Type messageType, const std::function<void(long long&, const Message&)>& action)
{
	m_actions[messageType] = action;
}

void Server::update()
{
	std::vector<std::pair<long long, Message>> toProcess;
	{
		std::lock_guard<std::mutex> lock(m_incomingMutex);
		toProcess.swap(m_incoming);
	}

	for (auto &item : toProcess)
	{
		long long cid = item.first;
		Message &msg = item.second;
		auto it = m_actions.find(msg.type());
		if (it != m_actions.end())
		{
			it->second(cid, msg);
		}
	}
}

void Server::sendTo(const Message& message, long long clientID)
{	
	auto it = m_clients.find(clientID);
	if (it == m_clients.end() || !it->second.active) 
	{
		std::cerr << "[Server] Can't send to client " << clientID << ": not active.\n";
		return;
	}
	MsgHeader header;
	header.type = message.type();
	header.bodySize = message.size();

	try
	{
		sendRaw(it->second.socketFD, reinterpret_cast<const char*>(&header), sizeof(header));
		if (header.bodySize > 0)
		{
			sendRaw(it->second.socketFD, message.body().data(), header.bodySize);
		}
	}
	catch (const std::exception &e)
	{
		std::cerr << "[Server] sendTo failed for client " << clientID << " : " << e.what() << "\n";
	}
}

void Server::sendToArray(const Message& message, std::vector<long long> clientIDs)
{
	for (auto cid : clientIDs)
	{
		sendTo(message, cid);
	}
}

void Server::sendToAll(const Message& message)
{
	for (auto &pair : m_clients)
	{
		if (pair.second.active)
		{
			sendTo(message, pair.first);
		}
	}
}

void Server::sendRaw(int socketFD, const char* data, size_t size)
{
	size_t bytesSent = 0;
	while (bytesSent < size)
	{
		ssize_t result = send(socketFD, data + bytesSent, size - bytesSent, 0);
		if (result <= 0)
		{
			throw std::runtime_error("[Server] send() failed or connection lost.");
		}
		bytesSent += result;
	}
}

bool Server::recvAll(int socketFD, char* data, size_t size)
{
	size_t bytesReceived = 0;
	while (bytesReceived < size)
	{
		ssize_t result = recv(socketFD, data + bytesReceived, size - bytesReceived, 0);
		if (result <= 0)
		{
			return false;
		}
		bytesReceived += result;
	}
	return true;
}
