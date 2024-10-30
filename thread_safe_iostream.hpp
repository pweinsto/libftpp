#ifndef THREAD_SAFE_IOSTREAM_HPP
# define THREAD_SAFE_IOSTREAM_HPP

# include <iostream>
# include <mutex>
# include <string>
# include <sstream>

class ThreadSafeIOStream
{
public:
	template <typename T>
	ThreadSafeIOStream& operator<<(const T& value)
	{
		m_buffer << value;
		return *this;
	}

	ThreadSafeIOStream& operator<<(std::ostream& (*manip)(std::ostream&))
	{
		if (manip == static_cast<std::ostream& (*)(std::ostream&)>(std::endl))
		{
			flushWithPrefix();
		}
		else
		{
			m_buffer << manip;
		}
		return *this;
	}

	template <typename T>
	ThreadSafeIOStream& operator>>(T& value)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		std::cin >> value;
		return *this;
	}

	void setPrefix(const std::string& prefix)
	{
		m_prefix = prefix;
	}

private:
	void flushWithPrefix()
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		std::cout << m_prefix << m_buffer.str() << std::endl;
		m_buffer.str("");
		m_buffer.clear();
	}
	
	std::ostringstream m_buffer;
	std::string m_prefix;
	static std::mutex m_mutex;

};

std::mutex ThreadSafeIOStream::m_mutex;

thread_local ThreadSafeIOStream threadSafeCout;
thread_local ThreadSafeIOStream threadSafeCin;

# endif