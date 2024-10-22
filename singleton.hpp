#ifndef SINGLETON_HPP
# define SINGLETON_HPP

# include <memory>
# include <stdexcept>

template<typename TType>
class Singleton
{
public:
	static TType* instance()
	{
		if (!m_instance)
			throw std::runtime_error("Instance not yet created");

		return m_instance.get();
	}

	template <typename... TArgs>
	static void instantiate(TArgs&&... args)
	{
		if (m_instance)
			throw std::runtime_error("Instance already created");

		m_instance = std::unique_ptr<TType>(new TType(std::forward<TArgs>(args)...));
	}


private:
	static std::unique_ptr<TType> m_instance;

	Singleton() = default;
	~Singleton() = default;

	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;

	friend TType;
};

template <typename TType>
std::unique_ptr<TType> Singleton<TType>::m_instance = nullptr;

# endif