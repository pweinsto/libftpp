#include "thread_safe_iostream.hpp"

std::mutex ThreadSafeIOStream::m_mutex;
thread_local ThreadSafeIOStream threadSafeCout;
thread_local ThreadSafeIOStream threadSafeCin;