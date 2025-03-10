#include "../../include/thread/persistent_worker.hpp"

PersistentWorker::PersistentWorker() : m_running(true)
{
	m_workerThread = std::thread(&PersistentWorker::workerLoop, this);
}

PersistentWorker::~PersistentWorker()
{
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_running = false;
	}

	m_cv.notify_all();

	if (m_workerThread.joinable())
	{
		m_workerThread.join();
	}
}

void PersistentWorker::addTask(const std::string& name, const std::function<void()>& jobToExecute)
{
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_taskPool[name] = jobToExecute;
	}

	m_cv.notify_all();
}

void PersistentWorker::removeTask(const std::string& name)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	m_taskPool.erase(name);
}

void PersistentWorker::workerLoop()
{
	while (m_running)
	{
		std::unordered_map<std::string, std::function<void()>> currentTasks;

		{
			std::unique_lock<std::mutex> lock(m_mutex);
			m_cv.wait(lock, [this]() { return !m_running || !m_taskPool.empty(); });

			if (!m_running)
				break;

			currentTasks = m_taskPool;
		}

		for (const auto& task : currentTasks)
		{
			task.second();
		}
	}
}