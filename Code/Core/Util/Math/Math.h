#pragma once

#include "Vector.h"
#include "Matrix.h"

enum class eAxisType : int8
{
	x,
	y,
	z
};

namespace Math
{
	constexpr float toRadian = 180.f / 3.14f;
	constexpr float toDegree = 3.14f / 180.f;

	template<typename T>
	static T Square(T num)
	{
		return T * T;
	}

	template<typename T>
	static constexpr T Abs(T num)
	{
		return (static_cast<T>(0) <= num) ? num : -num;
	}

	template<typename T>
	static constexpr T Max(T a, T b)
	{
		return (b <= a) ? a : b;
	}

	template<typename T>
	static constexpr T Min(T a, T b)
	{
		return (a <= b) ? a : b;
	}
}
