#include "../../include/network/client.hpp"

#include <iostream>
#include <thread>
#include <mutex>
#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <cstring>
#include <stdexcept>

struct MsgHeader
{
	int type;
	uint64_t bodySize;
};

Client::Client()
: m_socketFD(-1)
, m_connected(false)
, m_readThread(nullptr)
, m_readThreadRunning(false)
{}

Client::~Client()
{
	disconnect();
}

void Client::connect(const std::string& address, const size_t& port)
{
	if (m_connected)
	{
		std::cerr << "[Client] Already connected.\n";
		return;
	}

	m_socketFD = socket(AF_INET, SOCK_STREAM, 0);
	if (m_socketFD < 0)
	{
		throw std::runtime_error("Client: Failed to create socket.");
	}

	sockaddr_in serverAddr;
	std::memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(static_cast<uint16_t>(port));

	if (inet_pton(AF_INET, address.c_str(), &serverAddr.sin_addr) <= 0)
	{
		hostent* he = gethostbyname(address.c_str());
		if (!he)
		{
			close(m_socketFD);
			m_socketFD = -1;
			throw std::runtime_error("Client: gethostbyname failed.");
		}
		std::memcpy(&serverAddr.sin_addr, he->h_addr_list[0], he->h_length);
	}

	if (::connect(m_socketFD, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0)
	{
		close(m_socketFD);
		m_socketFD = -1;
		throw std::runtime_error("Client: connect() failed.");
	}

	m_connected = true;
	std::cout << "[Client] Connected to " << address << ":" << port << std::endl;

	m_readThreadRunning = true;
	m_readThread = new std::thread(&Client::readLoop, this);
}

void Client::disconnect()
{
	if (!m_connected) return;

	close(m_socketFD);
	m_socketFD = -1;
	m_connected = false;

	m_readThreadRunning = false;
	if (m_readThread && m_readThread->joinable())
	{
		m_readThread->join();
	}
	delete m_readThread;
	m_readThread = nullptr;

	std::cout << "[Client] Disconnected.\n";
}

void Client::defineAction(Message::Type messageType, const std::function<void(const Message&)>& action)
{
	m_actions[messageType] = action;
}

void Client::send(const Message& message)
{
	if (!m_connected)
	{
		std::cerr << "[Client] Cannot send: not connected.\n";
		return;
	}

	MsgHeader header;
	header.type = message.type();
	header.bodySize = message.size();

	sendRaw(reinterpret_cast<const char*>(&header), sizeof(header));
	if (header.bodySize > 0)
	{
		sendRaw(message.body().data(), message.size());
	}
}

void Client::update()
{
	std::vector<Message> toProcess;
	{
		std::lock_guard<std::mutex> lock(m_incomingMutex);
		toProcess.swap(m_incoming);
	}

	for (auto &msg : toProcess)
	{
		auto it = m_actions.find(msg.type());
		if (it != m_actions.end())
		{
			it->second(msg);
		}
	}
}

void Client::readLoop()
{
	while (m_readThreadRunning)
	{
		MsgHeader header;
		if (!recvAll(reinterpret_cast<char*>(&header), sizeof(header)))
		{
			std::cerr << "[Client] Server closed connection or error.\n";
			m_readThreadRunning = false;
			break;
		}

		Message msg(header.type);
		if (header.bodySize > 0)
		{
			std::vector<char> buffer(header.bodySize);
			if (!recvAll(buffer.data(), header.bodySize))
			{
				std::cerr << "[Client] Server closed connection or error.\n";
				m_readThreadRunning = false;
				break;
			}

			for (char c : buffer) {
				msg << c;
			}
        }

		{
			std::lock_guard<std::mutex> lock(m_incomingMutex);
			m_incoming.push_back(std::move(msg));
		}
	}
}

void Client::sendRaw(const char* data, size_t size)
{
	size_t bytesSent = 0;
	while (bytesSent < size)
	{
		ssize_t result = ::send(m_socketFD, data + bytesSent, size - bytesSent, 0);
		if (result <= 0)
		{
			throw std::runtime_error("[Client] send() failed or connection lost.");
		}
		bytesSent += result;
	}
}

bool Client::recvAll(char* data, size_t size)
{
	size_t bytesReceived = 0;
	while (bytesReceived < size)
	{
		ssize_t result = ::recv(m_socketFD, data + bytesReceived, size - bytesReceived, 0);
		if (result <= 0)
		{
			return false;
		}
		bytesReceived += result;
	}
	return true;
}
