#include "pch.h"
#include "RGBAColour.h"
#include "Vectors.h"
#include "Math2D.h"

RGBAColour::RGBAColour(int r, int g, int b, int a)
	: r{ (uint8)Math2D::Clamp(r, 0, Math2D::MaxColourRange) }
	, g{ (uint8)Math2D::Clamp(g, 0, Math2D::MaxColourRange) }
	, b{ (uint8)Math2D::Clamp(b, 0, Math2D::MaxColourRange) }
	, a{ (uint8)Math2D::Clamp(a, 0, Math2D::MaxColourRange) }
{}

RGBAColour::RGBAColour(uint r, uint g, uint b, uint a)
	: r{ (uint8)Math2D::Clamp(r, 0, Math2D::MaxColourRange) }
	, g{ (uint8)Math2D::Clamp(g, 0, Math2D::MaxColourRange) }
	, b{ (uint8)Math2D::Clamp(b, 0, Math2D::MaxColourRange) }
	, a{ (uint8)Math2D::Clamp(a, 0, Math2D::MaxColourRange) }
{}

RGBAColour::RGBAColour(const Vector3& vec3)
	: RGBAColour{ (int)vec3.x, (int)vec3.y, (int)vec3.z }
{}

RGBAColour::RGBAColour(const Vector4& vec4)
	: RGBAColour{ (int)vec4.x, (int)vec4.y, (int)vec4.z, (int)vec4.w }
{}

RGBAColour operator+(const RGBAColour& lhs, const RGBAColour& rhs)
{
	uint8 r = (uint8)Math2D::Clamp(lhs.r + rhs.r, 0, Math2D::MaxColourRange);
	uint8 g = (uint8)Math2D::Clamp(lhs.g + rhs.g, 0, Math2D::MaxColourRange);
	uint8 b = (uint8)Math2D::Clamp(lhs.b + rhs.b, 0, Math2D::MaxColourRange);
	uint8 a = (uint8)Math2D::Clamp(lhs.a + rhs.a, 0, Math2D::MaxColourRange);
	return RGBAColour{ r, g, b, a };
}
RGBAColour operator+(const RGBAColour& colour, const Vector4& RGBA)
{
	uint8 r = (uint8)Math2D::Clamp(colour.r + RGBA.x, 0.f, Math2D::MaxColourRange);
	uint8 g = (uint8)Math2D::Clamp(colour.g + RGBA.y, 0.f, Math2D::MaxColourRange);
	uint8 b = (uint8)Math2D::Clamp(colour.b + RGBA.z, 0.f, Math2D::MaxColourRange);
	uint8 a = (uint8)Math2D::Clamp(colour.a + RGBA.w, 0.f, Math2D::MaxColourRange);
	return RGBAColour{ r, g, b, a };
}
RGBAColour operator+(const RGBAColour& colour, const Vector3& RGB)
{
	uint8 r = (uint8)Math2D::Clamp(colour.r + RGB.x, 0.f, Math2D::MaxColourRange);
	uint8 g = (uint8)Math2D::Clamp(colour.g + RGB.y, 0.f, Math2D::MaxColourRange);
	uint8 b = (uint8)Math2D::Clamp(colour.b + RGB.z, 0.f, Math2D::MaxColourRange);
	return RGBAColour{ r, g, b, colour.a };
}

void operator+=(RGBAColour& lhs, const RGBAColour& rhs)
{
	lhs.r = (uint8)Math2D::Clamp(lhs.r + rhs.r, 0, Math2D::MaxColourRange);
	lhs.g = (uint8)Math2D::Clamp(lhs.g + rhs.g, 0, Math2D::MaxColourRange);
	lhs.b = (uint8)Math2D::Clamp(lhs.b + rhs.b, 0, Math2D::MaxColourRange);
	lhs.a = (uint8)Math2D::Clamp(lhs.a + rhs.a, 0, Math2D::MaxColourRange);
}
void operator+=(RGBAColour& colour, const Vector4& RGBA)
{
	colour.r = (uint8)Math2D::Clamp(colour.r + RGBA.x, 0.f, Math2D::MaxColourRange);
	colour.g = (uint8)Math2D::Clamp(colour.g + RGBA.y, 0.f, Math2D::MaxColourRange);
	colour.b = (uint8)Math2D::Clamp(colour.b + RGBA.z, 0.f, Math2D::MaxColourRange);
	colour.a = (uint8)Math2D::Clamp(colour.a + RGBA.w, 0.f, Math2D::MaxColourRange);
}
void operator+=(RGBAColour& colour, const Vector3& RGB)
{
	colour.r = (uint8)Math2D::Clamp(colour.r + RGB.x, 0.f, Math2D::MaxColourRange);
	colour.g = (uint8)Math2D::Clamp(colour.g + RGB.y, 0.f, Math2D::MaxColourRange);
	colour.b = (uint8)Math2D::Clamp(colour.b + RGB.z, 0.f, Math2D::MaxColourRange);
}

RGBAColour operator-(const RGBAColour& lhs, const RGBAColour& rhs)
{
	uint8 r = (uint8)Math2D::Clamp(lhs.r - rhs.r, 0, Math2D::MaxColourRange);
	uint8 g = (uint8)Math2D::Clamp(lhs.g - rhs.g, 0, Math2D::MaxColourRange);
	uint8 b = (uint8)Math2D::Clamp(lhs.b - rhs.b, 0, Math2D::MaxColourRange);
	uint8 a = (uint8)Math2D::Clamp(lhs.a - rhs.a, 0, Math2D::MaxColourRange);
	return RGBAColour{ r, g, b, a };
}
RGBAColour operator-(const RGBAColour& colour, const Vector4& RGBA)
{
	uint8 r = (uint8)Math2D::Clamp(colour.r - RGBA.x, 0.f, Math2D::MaxColourRange);
	uint8 g = (uint8)Math2D::Clamp(colour.g - RGBA.y, 0.f, Math2D::MaxColourRange);
	uint8 b = (uint8)Math2D::Clamp(colour.b - RGBA.z, 0.f, Math2D::MaxColourRange);
	uint8 a = (uint8)Math2D::Clamp(colour.a - RGBA.w, 0.f, Math2D::MaxColourRange);
	return RGBAColour{ r, g, b, a };
}
RGBAColour operator-(const RGBAColour& colour, const Vector3& RGB)
{
	uint8 r = (uint8)Math2D::Clamp(colour.r - RGB.x, 0.f, Math2D::MaxColourRange);
	uint8 g = (uint8)Math2D::Clamp(colour.g - RGB.y, 0.f, Math2D::MaxColourRange);
	uint8 b = (uint8)Math2D::Clamp(colour.b - RGB.z, 0.f, Math2D::MaxColourRange);
	return RGBAColour{ r, g, b, colour.a };
}

void operator-=(RGBAColour& lhs, const RGBAColour& rhs)
{
	lhs.r = (uint8)Math2D::Clamp(lhs.r - rhs.r, 0, Math2D::MaxColourRange);
	lhs.g = (uint8)Math2D::Clamp(lhs.g - rhs.g, 0, Math2D::MaxColourRange);
	lhs.b = (uint8)Math2D::Clamp(lhs.b - rhs.b, 0, Math2D::MaxColourRange);
	lhs.a = (uint8)Math2D::Clamp(lhs.a - rhs.a, 0, Math2D::MaxColourRange);
}
void operator-=(RGBAColour& colour, const Vector4& RGBA)
{
	colour.r = (uint8)Math2D::Clamp(colour.r - RGBA.x, 0.f, Math2D::MaxColourRange);
	colour.g = (uint8)Math2D::Clamp(colour.g - RGBA.y, 0.f, Math2D::MaxColourRange);
	colour.b = (uint8)Math2D::Clamp(colour.b - RGBA.z, 0.f, Math2D::MaxColourRange);
	colour.a = (uint8)Math2D::Clamp(colour.a - RGBA.w, 0.f, Math2D::MaxColourRange);
}
void operator-=(RGBAColour& colour, const Vector3& RGB)
{
	colour.r = (uint8)Math2D::Clamp(colour.r - RGB.x, 0.f, Math2D::MaxColourRange);
	colour.g = (uint8)Math2D::Clamp(colour.g - RGB.y, 0.f, Math2D::MaxColourRange);
	colour.b = (uint8)Math2D::Clamp(colour.b - RGB.z, 0.f, Math2D::MaxColourRange);
}