#ifndef MEMENTO_HPP
# define MEMENTO_HPP

# include "data_buffer.hpp"

class Memento
{
public:	
	class Snapshot
	{
	public:

		template<typename T>
		Snapshot &operator<<(const T &value)
		{
			m_buffer << value;
			return *this;
		}

		template<typename T>
		Snapshot &operator>>(T &value)
		{
			m_buffer >> value;
			return *this;
		}

	private:
		DataBuffer m_buffer;
	};

	Snapshot save()
	{
		Snapshot snapshot;
		_saveToSnapshot(snapshot);
		return snapshot;
	}

	void load(const Memento::Snapshot& state)
	{
		Snapshot snapshot = state;
		_loadFromSnapshot(snapshot);
	}

private:
	virtual void _saveToSnapshot(Memento::Snapshot& snapshot) = 0;
	virtual void _loadFromSnapshot(Memento::Snapshot& snapshot) = 0;
};

# endif