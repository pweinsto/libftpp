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
	std::string threadName;
	std::function<void()> functToExecute;
	std::thread internalThread;
	std::atomic<bool> isRunning;
};

# endif