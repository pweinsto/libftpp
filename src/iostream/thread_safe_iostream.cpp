#include "../../include/iostream/thread_safe_iostream.hpp"

std::mutex ThreadSafeIOStream::m_mutex;
thread_local ThreadSafeIOStream threadSafeCout;
thread_local ThreadSafeIOStream threadSafeCin;


ThreadSafeIOStream& ThreadSafeIOStream::operator<<(std::ostream& (*manip)(std::ostream&))
{
	m_buffer << manip;
	flushWithPrefix();
	return *this;
}

void ThreadSafeIOStream::setPrefix(const std::string& prefix)
{
	m_prefix = prefix;
}

void ThreadSafeIOStream::flushWithPrefix()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	std::cout << m_prefix << m_buffer.str();
	m_buffer.str("");
	m_buffer.clear();
}