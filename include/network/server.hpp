#ifndef SERVER_HPP
#define SERVER_HPP

#include <string>
#include <functional>
#include <unordered_map>
#include <vector>
#include <atomic>
#include <thread>
#include <mutex>
#include "message.hpp"

class Server
{
public:
	Server();
	~Server();

	void start(const size_t& p_port);

	void defineAction(Message::Type messageType, const std::function<void(long long& clientID, const Message& msg)>& action);

	void sendTo(const Message& message, long long clientID);

	void sendToArray(const Message& message, std::vector<long long> clientIDs);

	void sendToAll(const Message& message);

	void update();

private:
	struct ClientInfo
	{
		int socketFD;
		bool active;
	};

	std::atomic<long long> m_nextClientID;

	std::unordered_map<long long, ClientInfo> m_clients;

	std::thread* m_acceptThread;
	bool m_acceptThreadRunning;
	int m_listenFD;

	std::unordered_map<long long, std::thread*> m_clientThreads;

	std::mutex m_incomingMutex;
	std::vector<std::pair<long long, Message>> m_incoming;

	std::unordered_map<Message::Type, std::function<void(long long&, const Message&)>> m_actions;

	void acceptLoop();
	void clientReadLoop(long long clientID);

	void sendRaw(int socketFD, const char* data, size_t size);
	bool recvAll(int socketFD, char* data, size_t size);
};
#endif