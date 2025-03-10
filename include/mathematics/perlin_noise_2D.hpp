#ifndef PERLIN_NOISE_2D_HPP
#define PERLIN_NOISE_2D_HPP

#include <vector>
#include <cstdint>

class PerlinNoise2D
{
public:
	PerlinNoise2D();

	float sample(float x, float y) const;

private:
	std::vector<std::uint8_t> m_permutation;

	float fade(float t) const;
	float grad2(int hash, float x, float y) const;
};

#endif // PERLIN_NOISE_2D_HPP
