#include "../../include/network/message.hpp"

int Message::type() const 
{
	return m_type;
}

const std::vector<char>& Message::body() const
{
	return m_body;
}

size_t Message::size() const
{
	return m_body.size();
}