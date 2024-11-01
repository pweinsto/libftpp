#ifndef THREAD_SAFE_QUEUE_HPP
# define THREAD_SAFE_QUEUE_HPP

#include <deque>
#include <mutex>
#include <stdexcept>
#include <condition_variable>

template<typename TType>
class ThreadSafeQueue
{
public:
	void push_back(const TType& newElement)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_queue.push_back(newElement);
	}

	void push_front(const TType& newElement)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		m_queue.push_front(newElement);
	}

	TType pop_back()
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		if (m_queue.empty())
		{
			throw std::runtime_error("Queue is empty. Cannot pop back.");
		}
		TType value = m_queue.back();
		m_queue.pop_back();
		return value;
	}

	TType pop_front() {
		std::unique_lock<std::mutex> lock(m_mutex);
		if (m_queue.empty())
		{
			throw std::runtime_error("Queue is empty. Cannot pop front.");
		}
		TType value = m_queue.front();
		m_queue.pop_front();
		return value;
	}

private:
	std::deque<TType> m_queue;
	std::mutex m_mutex;
};

# endif