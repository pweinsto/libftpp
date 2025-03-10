#include "../../include/thread/worker_pool.hpp"

WorkerPool::WorkerPool(size_t numThreads) : m_stopFlag(false)
{
	for (size_t i = 0; i < numThreads; ++i)
	{
		m_workers.emplace_back(&WorkerPool::workerThread, this);
	}
}

WorkerPool::~WorkerPool()
{
	{
		std::lock_guard<std::mutex> lock(m_queueMutex);
		m_stopFlag = true;
	}
	m_condition.notify_all();

	for (std::thread& worker : m_workers)
	{
		if (worker.joinable())
		{
			worker.join();
		}
	}
}

void WorkerPool::addJob(const std::function<void()>& jobToExecute)
{
	{
		std::lock_guard<std::mutex> lock(m_queueMutex);
		m_jobQueue.push(jobToExecute);
	}
	m_condition.notify_one();
}

void WorkerPool::workerThread()
{
	while (true)
	{
		std::function<void()> job;
		{
			std::unique_lock<std::mutex> lock(m_queueMutex);
			m_condition.wait(lock, [this] {
				return !m_jobQueue.empty() || m_stopFlag.load();
			});

			if (m_stopFlag && m_jobQueue.empty())
			{
				return;
			}

			job = m_jobQueue.front();
			m_jobQueue.pop();
		}

		job();
	}
}
