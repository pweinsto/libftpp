#ifndef IVECTOR2_HPP
#define IVECTOR2_HPP

#include <cmath>
#include <type_traits>


template <typename TType>
struct IVector2
{
	TType x;
	TType y;

	IVector2()
		: x(static_cast<TType>(0)), y(static_cast<TType>(0)) {}

	IVector2(TType x, TType y)
		: x(x), y(y) {}

	IVector2<TType> operator+(const IVector2<TType>& other) const
	{
		return IVector2<TType>(this->x + other.x, this->y + other.y);
	}

	IVector2<TType> operator-(const IVector2<TType>& other) const
	{
		return IVector2<TType>(this->x - other.x, this->y - other.y);
	}

	IVector2<TType> operator*(const IVector2<TType>& other) const
	{
		return IVector2<TType>(this->x * other.x, this->y * other.y);
	}

	IVector2<TType> operator/(const IVector2<TType>& other) const
	{
		return IVector2<TType>(this->x / other.x, this->y / other.y);
	}

	bool operator==(const IVector2<TType>& other) const
	{
		return (this->x == other.x) && (this->y == other.y);
	}

	bool operator!=(const IVector2<TType>& other) const
	{
		return !(*this == other);
	}

	float length() const
	{
		return std::sqrt(static_cast<float>(x) * static_cast<float>(x) +
						static_cast<float>(y) * static_cast<float>(y));
	}

	IVector2<float> normalize() const
	{
		float len = length();
		if (len == 0.0f)
		{
			return IVector2<float>(0.0f, 0.0f);
		}
		return IVector2<float>(static_cast<float>(x) / len,
								static_cast<float>(y) / len);
	}

	float dot(const IVector2<TType>& other) const
	{
		return (static_cast<float>(this->x) * static_cast<float>(other.x)) +
	 			(static_cast<float>(this->y) * static_cast<float>(other.y));
	}

	IVector2<TType> cross() const 
	{
		return IVector2<TType>(-this->y, this->x);
	}
};

#endif // IVECTOR2_HPP