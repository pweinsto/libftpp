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

	// for standard iostream manipulators
	ThreadSafeIOStream& operator<<(std::ostream& (*manip)(std::ostream&));

	template <typename T>
	ThreadSafeIOStream& operator>>(T& value)
	{
		std::lock_guard<std::mutex> lock(m_mutex);
		std::cin >> value;
		return *this;
	}

	template <typename T>
	void prompt(const std::string& question, T& dest)
	{
		m_buffer << question;
		flushWithPrefix();
		std::lock_guard<std::mutex> lock(m_mutex);
		std::getline(std::cin, dest);

	}

	void setPrefix(const std::string& prefix);

private:
	void flushWithPrefix();
	
	std::ostringstream m_buffer;
	std::string m_prefix;
	static std::mutex m_mutex;
};

extern thread_local ThreadSafeIOStream threadSafeCout;
extern thread_local ThreadSafeIOStream threadSafeCin;

# endif