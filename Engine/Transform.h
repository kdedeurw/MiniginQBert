#pragma once
#include "Vectors.h"

struct Transform final
{
	Transform();
	Transform(float x, float y, float z, float r = 0.0f);
	Transform(Vector2 vec2, float z, float r = 0.0f);
	Transform(Vector3 vec3, float r = 0.0f);

	Vector3 Position;
	Vector2 Scale;
	float Rotation;
	Vector2 Forward;
	Vector2 Right;
};