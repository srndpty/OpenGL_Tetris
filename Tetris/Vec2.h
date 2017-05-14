#pragma once

template<typename T>
struct Vec2
{
	T x, y;

public:
	Vec2() = default;

	constexpr Vec2(T a, T b)
		: x(a)
		, y(b)
	{
	}

	Vec2 operator*(T a)
	{
		return{ x * a, y * a };
	}
};

using Vec2f = Vec2<float>;
using Vec2i = Vec2<int>;
