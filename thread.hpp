#ifndef THREAD_HPP
# define THREAD_HPP

# include <string>
# include <functional>
# include <thread>
# include <atomic>
# include "thread_safe_iostream.hpp"

class Thread
{
public:
	Thread(const std::string& name, std::function<void()> functToExecute);

	void start();
	void stop();

private:
	std::string m_threadName;
	std::function<void()> m_functToExecute;
	std::thread m_internalThread;
	std::atomic<bool> m_isRunning;
};

# endif