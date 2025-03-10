#include "../../include/mathematics/perlin_noise_2D.hpp"
#include <algorithm>
#include <random>
#include <cmath>
#include <iostream>

PerlinNoise2D::PerlinNoise2D()
{
	unsigned int seed = 42;

	m_permutation.resize(256);
	for (int i = 0; i < 256; ++i) {
		m_permutation[i] = static_cast<std::uint8_t>(i);
	}

	std::mt19937 generator(seed);
	std::shuffle(m_permutation.begin(), m_permutation.end(), generator);

	m_permutation.insert(m_permutation.end(), m_permutation.begin(), m_permutation.end());
}

float PerlinNoise2D::fade(float t) const
{
	return t * t * t * (t * (t * 6 - 15) + 10);
}

// Compute dot product of a pseudo-random gradient vector and the (x, y) distance from the corner.
float PerlinNoise2D::grad2(int hash, float x, float y) const
{
	// For 2D, we only need 2 bits from 'hash' for the gradient selection:
	// (±1,±1), (±1, 0), (0, ±1), etc. 
	// A simple approach is to mask with 3 (hash & 3) and pick from a small set of directions.
	int h = hash & 3;
    
	// This is one of the simplest gradient picks for 2D:
	//   h in [0..3] => possible gradients like (±1, ±1).
	// We'll pick signs based on bits.
	float u = (h & 2) == 0 ? x : -x;
	float v = (h & 1) == 0 ? y : -y;

	// Return dot product
	return u + v;
}

float PerlinNoise2D::sample(float x, float y) const
{
	// Find the integer coordinates of the cell containing (x, y).
	int xi = static_cast<int>(std::floor(x)) & 255;
	int yi = static_cast<int>(std::floor(y)) & 255;

	// Compute the fractional part of x and y.
	float xf = x - std::floor(x);
	float yf = y - std::floor(y);

	// Hash coordinates of the 4 corners.
	int aa = m_permutation[m_permutation[xi    ] + yi    ];
	int ab = m_permutation[m_permutation[xi    ] + (yi+1)];
	int ba = m_permutation[m_permutation[xi + 1] + yi    ];
	int bb = m_permutation[m_permutation[xi + 1] + (yi+1)];

	// Apply the fade curves to x and y.
	float u = fade(xf);
	float v = fade(yf);

	// Calculate the contribution from each of the four corners.
	float x1 = grad2(aa, xf    , yf    );
	float x2 = grad2(ba, xf - 1, yf    );
	float y1 = x1 + u * (x2 - x1);

	float x3 = grad2(ab, xf    , yf - 1);
	float x4 = grad2(bb, xf - 1, yf - 1);
	float y2 = x3 + u * (x4 - x3);

	// Interpolate the two results along y.
	float result = y1 + v * (y2 - y1);

	return result;
}
