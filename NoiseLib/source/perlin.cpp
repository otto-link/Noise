#include "perlin.h"

#include <cmath>
#include <cstdint>

#include "utils.h"

const float unit = 1.f / sqrt(2);
const float Gradients[][2] = {
	{ unit, unit },
	{ -unit, unit },
	{ unit, -unit },
	{ -unit, -unit },
	{ 1, 0 },
	{ -1, 0 },
	{ 0, 1 },
	{ 0, -1 }
};

const uint8_t HashTable[256] = {
	151, 160, 137,  91,  90,  15, 131,  13, 201,  95,  96,  53, 194, 233,   7, 225, 140,  36, 103,  30,  69, 
	142,   8,  99,  37, 240,  21,  10,  23, 190,   6, 148, 247, 120, 234,  75,   0,  26, 197,  62,  94, 252, 
	203, 117,  35,  11,  32,  57, 177,  33,  88, 237, 149,  56,  87, 174,  20, 125, 136, 171, 168,  68, 175, 
	 74, 165,  71, 134, 139,  48,  27, 166,  77, 146, 158, 231,  83, 111, 229, 122,  60, 211, 133, 230, 220, 
	105,  92,  41,  55,  46, 245,  40, 244, 102, 143,  54,  65,  25,  63, 161,   1, 216,  80,  73, 209,  76, 
	187, 208,  89,  18, 169, 200, 196, 135, 130, 116, 188, 159,  86, 164, 100, 109, 198, 173, 186,   3, 132,
	 64,  52, 217, 226, 250, 124, 123,   5, 202,  38, 147, 118, 126, 255,  82,  85, 212, 207, 206,  59, 227, 
	 47,  16,  58,  17, 182, 189,  28,  42, 223, 183, 170, 213, 119, 248, 152,   2,  44, 154, 163,  70, 221, 
	153, 101, 155, 167,  43, 172,   9, 129,  22,  39, 253,  19,  98, 108, 110,  79, 113, 224, 232, 178, 185, 
	112, 104, 218, 246,  97, 228, 251,  34, 242, 193, 238, 210, 144,  12, 191, 179, 162, 241,  81,  51, 145, 
	235, 249,  14, 239, 107,  49, 192, 214,  31, 181, 199, 106, 157, 184,  84, 204, 176, 115, 121,  50,  45, 
	127,   4, 150, 254, 138, 236, 205,  93, 222, 114,  67,  29,  24,  72, 243, 141, 128, 195,  78,  66, 215,
	 61, 156, 180, 219
};

// Computes the dot product of the distance and gradient vectors.
float dotGridGradient(int ix, int iy, float x, float y)
{
	const int mx = robust_mod(ix, 256);
	const int my = robust_mod(iy, 256);

	const int index = robust_mod(mx + HashTable[my], 256);

	// Index of the gradient
	int g = HashTable[index] % 8;

	// Compute the distance vector
	float dx = x - (float)ix;
	float dy = y - (float)iy;

	// Compute the dot-product
	return (dx * Gradients[g][0] + dy * Gradients[g][1]);
}

// Compute Perlin noise at coordinates x, y
float Perlin(float x, float y)
{
	// Determine grid cell coordinates
	int x0 = int(floor(x));
	int x1 = x0 + 1;
	int y0 = int(floor(y));
	int y1 = y0 + 1;

	// Determine interpolation weights
	// Could also use higher order polynomial/s-curve here
	float sx = x - floor(x);
	float sy = y - floor(y);

	float s = dotGridGradient(x0, y0, x, y);
	float t = dotGridGradient(x1, y0, x, y);
	float u = dotGridGradient(x0, y1, x, y);
	float v = dotGridGradient(x1, y1, x, y);

	// Interpolate between grid point gradients
	float ix0 = lerp(s, t, smoother(sx));
	float ix1 = lerp(u, v, smoother(sx));
	return lerp(ix0, ix1, smoother(sy));
}
