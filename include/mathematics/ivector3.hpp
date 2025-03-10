#ifndef IVECTOR3_HPP
#define IVECTOR3_HPP

#include <cmath>

template <typename TType>
struct IVector3
{
	TType x;
	TType y;
	TType z;

	IVector3()
		: x(static_cast<TType>(0)), 
		  y(static_cast<TType>(0)), 
		  z(static_cast<TType>(0)) {}

	IVector3(TType x, TType y, TType z)
		: x(x), y(y), z(z) {}

	IVector3<TType> operator+(const IVector3<TType>& other) const
	{
		return IVector3<TType>(this->x + other.x, 
								this->y + other.y, 
								this->z + other.z);
	}

	IVector3<TType> operator-(const IVector3<TType>& other) const
	{
		return IVector3<TType>(this->x - other.x, 
								this->y - other.y, 
								this->z - other.z);
	}

	IVector3<TType> operator*(const IVector3<TType>& other) const
	{
		return IVector3<TType>(this->x * other.x, 
								this->y * other.y, 
								this->z * other.z);
	}

	IVector3<TType> operator/(const IVector3<TType>& other) const
	{
		return IVector3<TType>(this->x / other.x, 
								this->y / other.y, 
								this->z / other.z);
	}

	bool operator==(const IVector3<TType>& other) const
	{
		return (this->x == other.x) && 
				(this->y == other.y) &&
				(this->z == other.z);
	}

	bool operator!=(const IVector3<TType>& other) const
	{
		return !(*this == other);
	}

	float length() const
	{
		return std::sqrt(static_cast<float>(x) * static_cast<float>(x) +
							static_cast<float>(y) * static_cast<float>(y) +
							static_cast<float>(z) * static_cast<float>(z));
	}

	IVector3<float> normalize() const
	{
		float len = length();
		if (len == 0.0f)
		{
			return IVector3<float>(0.0f, 0.0f, 0.0f);
		}
		return IVector3<float>(
			static_cast<float>(x) / len,
			static_cast<float>(y) / len,
			static_cast<float>(z) / len
		);
	}

	float dot(const IVector3<TType>& other) const
	{
		return (static_cast<float>(this->x) * static_cast<float>(other.x)) +
				(static_cast<float>(this->y) * static_cast<float>(other.y)) +
				(static_cast<float>(this->z) * static_cast<float>(other.z));
	}

	IVector3<TType> cross(const IVector3<TType>& other) const
	{
		return IVector3<TType>(
			(this->y * other.z) - (this->z * other.y),
			(this->z * other.x) - (this->x * other.z),
			(this->x * other.y) - (this->y * other.x)
		);
	}
};

#endif // IVECTOR3_HPP