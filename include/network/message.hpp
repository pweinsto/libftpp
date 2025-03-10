#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <vector>
#include <cstdint>
#include <cstring>
#include <stdexcept>
#include <type_traits>

class Message
{
public:

	using Type = int;

	Message(int type)
		: m_type(type)
		, m_readPos(0)
	{}

	int type() const;

	template <typename T>
	Message& operator<<(const T& data)
	{
		const size_t oldSize = m_body.size();
		m_body.resize(oldSize + sizeof(T));
	
		std::memcpy(m_body.data() + oldSize, &data, sizeof(T));
		return *this;
	}

	template <typename T>
	const Message& operator>>(T& data) const
	{
		if (m_readPos + sizeof(T) > m_body.size())
		{
			throw std::runtime_error("Not enough data in message to deserialize.");
		}
		std::memcpy(&data, m_body.data() + m_readPos, sizeof(T));
		m_readPos += sizeof(T);
		return *this;
	}

	const std::vector<char>& body() const;
	size_t size() const;

private:
	int m_type;
	std::vector<char> m_body;
	mutable size_t m_readPos;
};

#endif