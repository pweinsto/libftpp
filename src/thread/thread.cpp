#include "../../include/thread/thread.hpp"

Thread::Thread(const std::string& name, std::function<void()> functToExecute)
	: m_threadName(name), m_functToExecute(std::move(functToExecute)), m_isRunning(false) {}

Thread::~Thread()
{
	
}

void Thread::start()
{
	if (!m_isRunning)
	{
		m_isRunning = true;
		m_internalThread = std::thread([this]()
		{
			threadSafeCout.setPrefix(m_threadName);
			threadSafeCout << " Starting execution" << std::endl;
			m_functToExecute();
			threadSafeCout << " Finished execution" << std::endl;
		});
	}
}

void Thread::stop()
{
	if (m_isRunning && m_internalThread.joinable())
	{
		m_internalThread.join();
		m_isRunning = false;
		std::cout << m_threadName << " Stopped" << std::endl;
	}
}
