#ifndef PERSISTENT_WORKER_HPP
# define PERSISTENT_WORKER_HPP

# include <functional>
# include <unordered_map>
# include <thread>
# include <mutex>
# include <atomic>
# include <condition_variable>

class PersistentWorker
{
public:
	PersistentWorker();
	~PersistentWorker();

	void addTask(const std::string& name, const std::function<void()>& jobToExecute);
	void removeTask(const std::string& name);

private:
	void workerLoop();

	std::unordered_map<std::string, std::function<void()>> m_taskPool;
	std::mutex m_mutex;
	std::condition_variable m_cv;
	std::atomic<bool> m_running;
	std::thread m_workerThread;
};

# endif // PERSISTENT_WORKER_HPP
