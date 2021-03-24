#include "pch.h"
#include "Math2D.h"
#include "Vectors.h"

namespace Math2D
{
	int Clamp(int value, int min, int max)
	{
		if (value > max)
			return max;
		else if (value < min)
			return min;
		return value;
	}
	float Clamp(float value, float min, float max)
	{
		if (value > max)
			return max;
		else if (value < min)
			return min;
		return value;
	}
	float LERP(float v0, float v1, float t)
	{
		return v0 + t * (v1 - v0);
	}
	Vector2 LERP(const Vector2& v0, const Vector2& v1, float t)
	{
		return Vector2{ LERP(v0.x, v1.x, t), LERP(v0.y, v1.y, t) };
	}
	int RandomInt(int min, int max)
	{
		return rand() % max + min;
	}
}