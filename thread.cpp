#include "thread.hpp"

Thread::Thread(const std::string& name, std::function<void()> functToExecute)
	: threadName(name), functToExecute(std::move(functToExecute)), isRunning(false) {}

void Thread::start()
{
	if (!isRunning)
	{
		isRunning = true;
		internalThread = std::thread([this]()
		{
			threadSafeCout.setPrefix(threadName);
			threadSafeCout << " Starting execution" << std::endl;
			functToExecute();
			threadSafeCout << " Finished execution" << std::endl;
		});
	}
}

void Thread::stop()
{
	if (isRunning && internalThread.joinable())
	{
		internalThread.join();
		isRunning = false;
		std::cout << threadName << " Stopped" << std::endl;
	}
}
