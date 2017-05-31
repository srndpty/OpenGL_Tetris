#pragma once

#include <cstdint>

class Random
{
private:
	uint32_t x = 123456789;
	uint32_t y = 362436069;
	uint32_t z = 521288629;
	uint32_t w = 88675123;

public:
	// special
	Random();
	Random(uint32_t aX, uint32_t aY, uint32_t aZ, uint32_t aW);
	~Random();

	// normal
	uint32_t operator ()();
	uint32_t operator ()(uint32_t max);
	uint32_t operator ()(uint32_t min, uint32_t max);

	uint32_t Next();
	// exclusive
	uint32_t Next(uint32_t max);
	// inclusive
	uint32_t Next(uint32_t min, uint32_t max);
};

