#pragma once

struct Vector2
{
	Vector2();
	//move constructor
	Vector2(float x, float y);
	//copy constructor
	Vector2(const Vector2& vec2);
	virtual ~Vector2() = default;

	float x;
	float y;
};

struct Vector3 : public Vector2
{
	Vector3();
	//move constructor
	Vector3(float x, float y, float z = 0.f);
	//move constructor
	Vector3(Vector2 vec2, float z = 0.f);
	//copy constructor
	Vector3(const Vector2& vec2, float z = 0.f);
	//copy constructor
	Vector3(const Vector3& vec3);
	virtual ~Vector3() = default;

	float z;
};

struct Vector4 final : public Vector3
{
	Vector4();
	//copy constructor
	Vector4(float x, float y, float z, float w = 1.f);
	//copy constructor
	Vector4(const Vector2& vec2, float z, float w = 1.f);
	//move constructor
	Vector4(Vector2 vec2, float z, float w = 1.f);
	//copy constructor
	Vector4(const Vector3& vec3, float w = 1.f);
	//move constructor
	Vector4(Vector3 vec3, float w = 1.f);
	//copy constructor
	Vector4(const Vector4& vec4);
	virtual ~Vector4() = default;

	float w;
};

//----------------------------------------
//				VectorMath
//----------------------------------------

//Vector2
void operator+(Vector2& lhs, const float value);
Vector2 operator+(const Vector2& lhs, const float value);
Vector2 operator+(const Vector2& lhs, const Vector2& rhs);
Vector2 operator+(const Vector2& lhs, const Vector3& rhs);
void operator+=(Vector2& lhs, const float value);
void operator+=(Vector2& lhs, const Vector2& rhs);
void operator+=(Vector2& lhs, const Vector3& rhs);

void operator-(Vector2& lhs, const float value);
Vector2 operator-(const Vector2& lhs, const float value);
Vector2 operator-(const Vector2& lhs, const Vector2& rhs);
Vector2 operator-(const Vector2& lhs, const Vector3& rhs);
void operator-=(Vector2& lhs, const float value);
void operator-=(Vector2& lhs, const Vector2& rhs);
void operator-=(Vector2& lhs, const Vector3& rhs);

Vector2 operator*(const Vector2& lhs, const float scalar);
void operator*=(Vector2& lhs, const float scalar);

Vector2 operator/(const Vector2& lhs, const float scalar);
void operator/=(Vector2& lhs, const float scalar);

namespace Math2D
{
	float Magnitude(const Vector2& lhs);
	float Dot(const Vector2& lhs, const Vector2& rhs);
	float Cross(const Vector2& lhs, const Vector2& rhs);
	//gives an angle in degrees oriented to the world's right vector
	float OrientOnWorld(const Vector2& lhs);
	float Normalize(Vector2& lhs);
	Vector2 GetNormalized(const Vector2& lhs);
	Vector2 OrthogonalL(const Vector2& vec2);
	Vector2 OrthogonalR(const Vector2& vec2);
	Vector2 Reflect(const Vector2& vec2, const Vector2& surfaceNormal);
}

//Vector3
Vector3 operator+(const Vector3& lhs, const Vector3& rhs);
Vector3 operator+(const Vector3& lhs, const Vector2& rhs);
void operator+=(Vector3& lhs, const Vector3& rhs);
void operator+=(Vector3& lhs, const Vector2& rhs);

Vector3 operator-(const Vector3& lhs, const Vector3& rhs);
Vector3 operator-(const Vector3& lhs, const Vector2& rhs);
void operator-=(Vector3& lhs, const Vector3& rhs);
void operator-=(Vector3& lhs, const Vector2& rhs);

Vector3 operator*(const Vector3& lhs, const float scalar);
void operator*=(Vector3& lhs, const float scalar);

Vector3 operator/(const Vector3& lhs, const float scalar);
void operator/=(Vector3& lhs, const float scalar);

namespace Math2D
{
	float Magnitude(const Vector3& lhs);
	float Dot(const Vector3& lhs, const Vector3& rhs);
	Vector3 Cross(const Vector3& lhs, const Vector3& rhs);
	float Normalize(Vector3& lhs);
	Vector3 GetNormalized(const Vector3& lhs);
}

//Vector4
Vector4 operator+(const Vector4& lhs, const Vector4& rhs);
void operator+=(Vector4& lhs, const Vector4& other);

Vector4 operator-(const Vector4& lhs, const Vector4& rhs);
void operator-=(Vector4& lhs, const Vector4& other);

Vector4 operator*(const Vector4& lhs, const float scalar);
void operator*=(Vector4& lhs, const float scalar);

Vector4 operator/(const Vector4& lhs, const float scalar);
void operator/=(Vector4& lhs, const float scalar);