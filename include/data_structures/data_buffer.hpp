#ifndef DATA_BUFFER_HPP
# define DATA_BUFFER_HPP

# include <stdexcept>
# include <iostream>
# include <vector>
# include <cstdint>

class DataBuffer {
public:
	DataBuffer();
	~DataBuffer();

	template<typename T>
	DataBuffer& operator<<(const T& p_object)
	{
		size_t size = sizeof(T);
		const uint8_t* rawData = reinterpret_cast<const uint8_t*>(&p_object);
		m_buffer.insert(m_buffer.end(), rawData, rawData + size);

		return *this;
	}

	template<typename T>
	DataBuffer& operator>>(T& p_object)
	{
		if (m_buffer.size() >= sizeof(T))
		{
			std::memcpy(&p_object, m_buffer.data(), sizeof(T));
			m_buffer.erase(m_buffer.begin(), m_buffer.begin() + sizeof(T));
		}
		else
		{
			throw std::runtime_error("Not enough data to deserialize");
		}
		return *this;
	}

private:
	std::vector<uint8_t> m_buffer;

};

#endif // DATA_BUFFER_HPP