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
			throw std::logic_error("Instance not yet created");

		return m_instance.get();
	}

	template <typename... TArgs>
	static void instantiate(TArgs&&... p_args)
	{
		if (m_instance)
			throw std::logic_error("Instance already created");

		m_instance = std::unique_ptr<TType>(new TType(std::forward<TArgs>(p_args)...));
	}


private:
	// inline: to declare and define a static member in one place
	// unique_ptr: non-copyable, auto-deletes object when out of scope
	inline static std::unique_ptr<TType> m_instance = nullptr;

	Singleton() = default;
	~Singleton() = default;

	Singleton(const Singleton&) = delete;
	Singleton& operator=(const Singleton&) = delete;
};

# endif // SINGELTON_HPP