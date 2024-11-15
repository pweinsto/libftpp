#ifndef WORKER_POOL_HPP
# define WORKER_POOL_HPP

# include <thread>
# include <vector>
# include <queue>
# include <functional>
# include <mutex>
# include <condition_variable>
# include <atomic>
# include <iostream>

class WorkerPool
{
public:
	WorkerPool(size_t numThreads);

	~WorkerPool();

	void addJob(const std::function<void()>& jobToExecute);

	class IJob {
		virtual ~IJob() = default;
		virtual void execute() = 0;
	};

private:
	void workerThread();

	std::vector<std::thread> m_workers;
	std::queue<std::function<void()>> m_jobQueue;
	std::mutex m_queueMutex;
	std::condition_variable m_condition;
	std::atomic<bool> m_stopFlag;
};

# endif