#ifndef RANDOM_2D_COORDINATE_GENERATOR_HPP
#define RANDOM_2D_COORDINATE_GENERATOR_HPP

class Random2DCoordinateGenerator
{
public:
	Random2DCoordinateGenerator(long long s = 42LL);

	long long seed() const;

	long long operator()(const long long& x, const long long& y);

private:
	long long m_seed;
};

#endif // RANDOM_2D_COORDINATE_GENERATOR_HPP