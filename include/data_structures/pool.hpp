#ifndef POOL_HPP
# define POOL_HPP

# include <vector>
# include <iostream>
# include <stdexcept>

template<typename TType>
class Pool
{

public:

	Pool()
	{
		std::cout << "Pool constructor" << std::endl;
	}

	~Pool()
	{
		for (char* rawMemory : m_pool)
		{
			delete[] rawMemory;
		}
		
		std::cout << "Pool destructor" << std::endl;
	}

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

		std::cout << "Pool size is set to " << m_pool.size() << std::endl;
	}


	// Takes rawMemory from the Pool and constructs with it an object of type TType
	template<typename ... TArgs>
	typename Pool::Object acquire(TArgs&& ... p_args)
	{
		char *rawMemory;
		TType* object;
		
		if (m_pool.size())
		{
			rawMemory = m_pool.back();
			m_pool.pop_back();
		}
		else
		{
			throw std::runtime_error("Pool is empty");
		}
		
		std::cout << "Pool size is " << m_pool.size() << std::endl;


		try
		{
			// perfect forwarding so that it's lvalue or rvalue is preserved. lvalue should be copied and rvalue should be moved
			object = new (rawMemory) TType(std::forward<TArgs>(p_args)...);
		}
		catch (...)
    	{
        	// Return the memory block back to the pool if construction fails
        	m_pool.push_back(rawMemory);

			// Re-throw the exception
        	throw; 
    	}
		

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

# endif // POOL_HPP