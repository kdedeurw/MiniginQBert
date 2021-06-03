#pragma once
#include <math.h>

struct Vector2;
struct Vector3;
struct Vector4;

namespace Math2D
{
	static const float Pi = 3.14159265359f;
	static const float ToDegrees = 180.f / Pi;
	static const float ToRadians = Pi / 180.f;
	const static size_t MaxColourRange = 255;

	int Clamp(int value, int min = 0, int max = 1);
	float Clamp(float value, float min = 0.f, float max = 1.f);
	float LERP(float v0, float v1, float t);
	Vector2 LERP(const Vector2& v0, const Vector2& v1, float t);

	int RandomInt(int min, int max);
}