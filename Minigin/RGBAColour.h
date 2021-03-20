#pragma once

using uint8 = unsigned char;
using uint = unsigned int;
struct Vector2;
struct Vector3;
struct Vector4;

struct RGBAColour
{
	RGBAColour(int r = 255, int g = 255, int b = 255, int a = 255);
	RGBAColour(uint r, uint g, uint b, uint a);
	RGBAColour(const Vector3& vec3);
	RGBAColour(const Vector4& vec4);

	uint8 r;
	uint8 g;
	uint8 b;
	uint8 a;
};

RGBAColour operator+(const RGBAColour& lhs, const RGBAColour& rhs);
RGBAColour operator+(const RGBAColour& colour, const Vector4& RGBA);
RGBAColour operator+(const RGBAColour& colour, const Vector3& RGB);

void operator+=(RGBAColour& lhs, const RGBAColour& rhs);
void operator+=(RGBAColour& colour, const Vector4& RGBA);
void operator+=(RGBAColour& colour, const Vector3& RGB);

RGBAColour operator-(const RGBAColour& lhs, const RGBAColour& rhs);
RGBAColour operator-(const RGBAColour& colour, const Vector4& RGBA);
RGBAColour operator-(const RGBAColour& colour, const Vector3& RGB);

void operator-=(RGBAColour& lhs, const RGBAColour& rhs);
void operator-=(RGBAColour& colour, const Vector4& RGBA);
void operator-=(RGBAColour& colour, const Vector3& RGB);