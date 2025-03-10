#ifndef CLIENT_HPP
#define CLIENT_HPP

#include <string>
#include <functional>
#include <unordered_map>
#include <thread>
#include <mutex>
#include "message.hpp"

class Client
{
public:
	Client();
	~Client();

	void connect(const std::string& address, const size_t& port);

	void disconnect();

	void defineAction(Message::Type messageType, const std::function<void(const Message& msg)>& action);

	void send(const Message& message);

	void update();

private:
	int m_socketFD;
	bool m_connected;

	std::thread* m_readThread;
	bool m_readThreadRunning;

	std::unordered_map<Message::Type, std::function<void(const Message&)>> m_actions;

	std::mutex m_incomingMutex;
	std::vector<Message> m_incoming;

	void readLoop();

	void sendRaw(const char* data, size_t size);

	bool recvAll(char* data, size_t size);
};

#endif