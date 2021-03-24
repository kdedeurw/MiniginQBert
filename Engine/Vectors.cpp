#include "pch.h"
#include "Vectors.h"

//----------------------------------------
//				Vector2
//----------------------------------------

Vector2::Vector2()
	: Vector2{ 0.f, 0.f }
{}

Vector2::Vector2(float x, float y)
	: x{ std::move(x) }
	, y{ std::move(y) }
{}

Vector2::Vector2(const Vector2& vec2)
	: x{ vec2.x }
	, y{ vec2.y }
{}

//----------------------------------------
//				Vector3
//----------------------------------------


Vector3::Vector3()
	: Vector3{ 0.f, 0.f, 0.f }
{}

Vector3::Vector3(float x, float y, float z)
	: Vector2{ std::move(x), std::move(y) }
	, z{ std::move(z) }
{}

Vector3::Vector3(Vector2 vec2, float z)
	: Vector3{ std::move(vec2.x), std::move(vec2.y), std::move(z) }
{}

Vector3::Vector3(const Vector2& vec2, float z)
	: Vector3{ vec2.x, vec2.y, std::move(z) }
{}

Vector3::Vector3(const Vector3& vec3)
	: Vector3{ vec3.x, vec3.y, vec3.z }
{}

//----------------------------------------
//				Vector4
//----------------------------------------

Vector4::Vector4()
	: Vector4{ 0.f, 0.f, 0.f, 0.f }
{}

Vector4::Vector4(float x, float y, float z, float w)
	: Vector3{ std::move(x), std::move(y), std::move(z) }
	, w{ std::move(w) }
{}

Vector4::Vector4(const Vector2& vec2, float z, float w)
	: Vector4{ vec2.x, vec2.y, std::move(z), std::move(w) }
{}

Vector4::Vector4(Vector2 vec2, float z, float w)
	: Vector4{ std::move(vec2.x), std::move(vec2.y), std::move(z), std::move(w) }
{}

Vector4::Vector4(const Vector3& vec3, float w)
	: Vector4{ vec3.x, vec3.y, vec3.z, std::move(w) }
{}

Vector4::Vector4(Vector3 vec3, float w)
	: Vector4{ std::move(vec3.x), std::move(vec3.y), std::move(vec3.z), std::move(w) }
{}

Vector4::Vector4(const Vector4& vec4)
	: Vector4{ vec4.x, vec4.y, vec4.z, vec4.w }
{}

//----------------------------------------
//				VectorMath
//----------------------------------------

void operator+(Vector2& lhs, const float value)
{
	lhs.x += value;
	lhs.y += value;
}

Vector2 operator+(const Vector2& lhs, const float value)
{
	return Vector2{ lhs.x + value, lhs.y + value };
}

Vector2 operator+(const Vector2& lhs, const Vector2& rhs)
{
	return Vector2{ lhs.x + rhs.x, lhs.y + rhs.y };
}

Vector2 operator+(const Vector2& lhs, const Vector3& rhs)
{
	return Vector2{ lhs.x + rhs.x, lhs.y + rhs.y };
}

void operator+=(Vector2& lhs, const float value)
{
	lhs.x += value;
	lhs.y += value;
}

void operator+=(Vector2& lhs, const Vector2& rhs)
{
	lhs.x += rhs.x;
	lhs.y += rhs.y;
}

void operator+=(Vector2& lhs, const Vector3& rhs)
{
	lhs.x += rhs.x;
	lhs.y += rhs.y;
}

void operator-(Vector2& lhs, const float value)
{
	lhs.x -= value;
	lhs.y -= value;
}

Vector2 operator-(const Vector2& lhs, const float value)
{
	return Vector2{ lhs.x - value, lhs.y - value };
}

Vector2 operator-(const Vector2& lhs, const Vector2& rhs)
{
	return Vector2{ lhs.x - rhs.x, lhs.y - rhs.y };
}

Vector2 operator-(const Vector2& lhs, const Vector3& rhs)
{
	return Vector2{ lhs.x - rhs.x, lhs.y - rhs.y };
}

void operator-=(Vector2& lhs, const float value)
{
	lhs.x -= value;
	lhs.y -= value;
}

void operator-=(Vector2& lhs, const Vector2& rhs)
{
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
}

void operator-=(Vector2& lhs, const Vector3& rhs)
{
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
}

Vector2 operator*(const Vector2& lhs, const float scalar)
{
	return Vector2{ lhs.x * scalar, lhs.y * scalar };
}

void operator*=(Vector2& lhs, const float scalar)
{
	lhs.x *= scalar;
	lhs.y *= scalar;
}

Vector2 operator/(const Vector2& lhs, const float scalar)
{
	return Vector2{ lhs.x / scalar, lhs.y / scalar };
}

void operator/=(Vector2& lhs, const float scalar)
{
	lhs.x /= scalar;
	lhs.y /= scalar;
}

float Math2D::Magnitude(const Vector2& lhs)
{
	return sqrtf(powf(lhs.x, 2) + powf(lhs.y, 2));
}

float Math2D::Dot(const Vector2& lhs, const Vector2& rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y;
}

float Math2D::Cross(const Vector2& lhs, const Vector2& rhs)
{
	return lhs.x * rhs.y - lhs.y * rhs.x;
}

float Math2D::OrientOnWorld(const Vector2& lhs)
{
	return Dot(Vector2{ 1.f, 0.f }, lhs);
}

float Math2D::Normalize(Vector2& lhs)
{
	const float magnitude = Magnitude(lhs);
	lhs /= magnitude;
	return magnitude;
}

Vector2 Math2D::GetNormalized(const Vector2& lhs)
{
	const float magnitude = Magnitude(lhs);
	return Vector2{ lhs.x / magnitude, lhs.y / magnitude };
}

Vector2 Math2D::OrthogonalL(const Vector2& vec2)
{
	return Vector2{ -vec2.y, vec2.x };
}

Vector2 Math2D::OrthogonalR(const Vector2& vec2)
{
	return Vector2{ vec2.y, -vec2.x };
}

Vector2 Math2D::Reflect(const Vector2& vec2, const Vector2& surfaceNormal)
{
	return vec2 - (surfaceNormal * Math2D::Dot(vec2, surfaceNormal)) * 2.f;
}

Vector3 operator+(const Vector3& lhs, const Vector3& rhs)
{
	return Vector3{ lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z };
}

Vector3 operator+(const Vector3& lhs, const Vector2& rhs)
{
	return Vector3{ lhs.x + rhs.x, lhs.y + rhs.y, lhs.z };
}

void operator+=(Vector3& lhs, const Vector3& rhs)
{
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	lhs.z += rhs.z;
}

void operator+=(Vector3& lhs, const Vector2& rhs)
{
	lhs.x += rhs.x;
	lhs.y += rhs.y;
}

Vector3 operator-(const Vector3& lhs, const Vector3& rhs)
{
	return Vector3{ lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z };
}

Vector3 operator-(const Vector3& lhs, const Vector2& rhs)
{
	return Vector3{ lhs.x - rhs.x, lhs.y - rhs.y, lhs.z };
}

void operator-=(Vector3& lhs, const Vector3& rhs)
{
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	lhs.z -= rhs.z;
}

void operator-=(Vector3& lhs, const Vector2& rhs)
{
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
}

Vector3 operator*(const Vector3& lhs, const float scalar)
{
	return Vector3{ lhs.x * scalar, lhs.y * scalar, lhs.z * scalar };
}

void operator*=(Vector3& lhs, const float scalar)
{
	lhs.x *= scalar;
	lhs.y *= scalar;
	lhs.z *= scalar;
}

Vector3 operator/(const Vector3& lhs, const float scalar)
{
	return Vector3{ lhs.x / scalar, lhs.y / scalar, lhs.z / scalar };
}

void operator/=(Vector3& lhs, const float scalar)
{
	lhs.x /= scalar;
	lhs.y /= scalar;
	lhs.z /= scalar;
}

float Math2D::Magnitude(const Vector3& lhs)
{
	return sqrtf(powf(lhs.x, 2) + powf(lhs.y, 2) + powf(lhs.z, 2));
}

float Math2D::Dot(const Vector3& lhs, const Vector3& rhs)
{
	return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z;
}

Vector3 Math2D::Cross(const Vector3& lhs, const Vector3& rhs)
{
	return Vector3{ lhs.y * rhs.z - lhs.z * rhs.y, lhs.z * rhs.x - lhs.x * rhs.z, lhs.x * rhs.y - lhs.y * rhs.x };
}

float Math2D::Normalize(Vector3& lhs)
{
	const float magnitude = Magnitude(lhs);
	lhs /= magnitude;
	return magnitude;
}

Vector3 Math2D::GetNormalized(const Vector3& lhs)
{
	const float magnitude = Magnitude(lhs);
	return Vector3{ lhs.x / magnitude, lhs.y / magnitude, lhs.z / magnitude };
}

Vector4 operator+(const Vector4& lhs, const Vector4& rhs)
{
	return Vector4{ lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w };
}

void operator+=(Vector4& lhs, const Vector4& rhs)
{
	lhs.x += rhs.x;
	lhs.y += rhs.y;
	lhs.z += rhs.z;
	lhs.w += rhs.w;
}

Vector4 operator-(const Vector4& lhs, const Vector4& rhs)
{
	return Vector4{ lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w };
}

void operator-=(Vector4& lhs, const Vector4& rhs)
{
	lhs.x -= rhs.x;
	lhs.y -= rhs.y;
	lhs.z -= rhs.z;
	lhs.w -= rhs.w;
}

Vector4 operator*(const Vector4& lhs, const float scalar)
{
	return Vector4{ lhs.x * scalar, lhs.y * scalar, lhs.z * scalar, lhs.w * scalar };
}

void operator*=(Vector4& lhs, const float scalar)
{
	lhs.x *= scalar;
	lhs.y *= scalar;
	lhs.z *= scalar;
	lhs.w *= scalar;
}

Vector4 operator/(const Vector4& lhs, const float scalar)
{
	return Vector4{ lhs.x / scalar, lhs.y / scalar, lhs.z / scalar, lhs.w / scalar };
}

void operator/=(Vector4& lhs, const float scalar)
{
	lhs.x /= scalar;
	lhs.y /= scalar;
	lhs.z /= scalar;
	lhs.w /= scalar;
}