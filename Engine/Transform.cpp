#include "pch.h"
#include "Transform.h"

Transform::Transform()
	: Transform{ 0.f, 0.f, 0.f, 0.f }
{}

Transform::Transform(float x, float y, float z, float r)
	: Position{ std::move(x), std::move(y), std::move(z) }
	, Rotation{ std::move(r) }
	, Scale{ 1.f, 1.f }
	, Forward{ 1.f, 0.f }
	, Right{ 0.f, -1.f }
{}

Transform::Transform(Vector2 vec2, float z, float r)
	: Transform{ std::move(vec2.x), std::move(vec2.y), std::move(z), std::move(r) }
{}

Transform::Transform(Vector3 vec3, float r)
	: Transform{ std::move(vec3.x), std::move(vec3.y), std::move(vec3.z), std::move(r) }
{}