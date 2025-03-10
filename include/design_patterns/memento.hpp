#ifndef MEMENTO_HPP
# define MEMENTO_HPP

# include <stdexcept>
# include <iostream>
# include <vector>
# include <cstdint>
# include "../data_structures/data_buffer.hpp"

class Memento
{
public:
	Memento();
	~Memento();

	class Snapshot
	{
	public:

		Snapshot();
		~Snapshot();

		template<typename T>
		Snapshot &operator<<(const T& value)
		{
			m_buffer << value;
			return *this;
		}

		template<typename T>
		Snapshot &operator>>(T& value)
		{
			m_buffer >> value;
			return *this;
		}

	private:
		DataBuffer m_buffer;
	};

	Snapshot save();
	void load(const Memento::Snapshot& state);

private:
	virtual void _saveToSnapshot(Memento::Snapshot& snapshot) const = 0;
	virtual void _loadFromSnapshot(Memento::Snapshot& snapshot) = 0;
};

# endif // MEMENTO_HPP