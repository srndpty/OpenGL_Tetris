#pragma once

struct Vec2
{
	float x, y;

public:
	Vec2() = default;

	Vec2(float a, float b)
		: x(a)
		, y(b)
	{
	}

	Vec2 operator*(float a)
	{
		return{ x * a, y * a };
	}
};
