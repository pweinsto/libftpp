#ifndef POOL_HPP
# define POOL_HPP

# include <vector>
#include <iostream>
#include <stdexcept>

// Manages a collection of reusable templated TType objects,
// provided to the user via a Pool::Object class. This class
// handles the acquirable pointer, and releases it back to the pool
// when needed, calling the destructor of the TType object but
// without deallocating the memory
template<typename TType>
class Pool
{

public:

	Pool() { std::cout << "Pool constructor" << std::endl; }
	~Pool() { std::cout << "Pool destructor" << std::endl; }

	class Object
	{

	public:

		Object(TType *ptr, Pool *pool) : m_ptr(ptr), m_pool(pool) {}
		~Object()
		{
            if (m_ptr)
			{
                m_ptr->~TType();
                m_pool->release(m_ptr);
            }
        }

		TType* operator->()
		{
            return m_ptr;
        }

	private:

		TType	*m_ptr;
		Pool	*m_pool;

	};

	// allocates the memory in a vector for numberOfObjectsStored of type TType
	void resize(const size_t& numberOfObjectStored)
	{
		m_pool.reserve(numberOfObjectStored);

		for (size_t i = 0; i < numberOfObjectStored; ++i)
		{
			m_pool.push_back(new char[sizeof(TType)]);
		}

		std::cout << "Pool size is set to " << numberOfObjectStored << std::endl;
	}

	// Returns an Pool::Object object
	// Takes rawMemory from the Pool and constructs with it an object of type TType
	// If the pool is empty allocate a new memory block
	// Calls constructor of Pool::Object with a pointer to the object
	template<typename ... TArgs>
	typename Pool::Object acquire(TArgs&&... p_args)
	{
		char *rawMemory;
		
		if (m_pool.size())
		{
			rawMemory = m_pool.back();
			m_pool.pop_back();
		}
		else
		{
			//throw std::runtime_error("Pool is empty");
			rawMemory = new char[sizeof(TType)];
		}
		
		std::cout << "Pool size is " << m_pool.size() << std::endl;

		TType* object = new (rawMemory) TType(std::forward<TArgs>(p_args)...);
		return Object(object, this);
	}

	void release(TType* object)
	{
        m_pool.push_back(reinterpret_cast<char*>(object));

		std::cout << "Pool size is " << m_pool.size() << std::endl;
    }

private:

	std::vector<char *>	m_pool;
};

# endif