#include "../../include/mathematics/random_2D_coordinate_generator.hpp"
#include <random>

Random2DCoordinateGenerator::Random2DCoordinateGenerator(long long s) : m_seed(s)
{}

long long Random2DCoordinateGenerator::seed() const
{
    return m_seed;
}

long long Random2DCoordinateGenerator::operator()(const long long& x, const long long& y)
{
	std::seed_seq seq{m_seed, x, y};
	std::mt19937 mt(seq);

	return static_cast<long long>(mt());	
}