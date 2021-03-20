#pragma once
#include <vector>
#include "Vectors.h"

//struct Vector2;
//struct Vector3;
//struct Vector4;

struct HitInfo2D
{
	float lambda;
	Vector2 intersectPoint;
	Vector2 normal;
};

struct Circle2D
{
	Circle2D(float x, float y, float radius);
	Circle2D(const Vector2& center, float radius);
	Circle2D(const Vector3& vec3);

	Vector2 center;
	float radius;
};

namespace Collision2D
{
	bool IsPointInBox(const Vector2& p, const Vector4& r);
	bool IsPointInCircle(const Vector2& p, const Circle2D& c);
	bool IsPointInCircle(const Vector2& lhs, const Vector2& rhs, float radius);
	bool IsPointInPolygon(const Vector2& p, const std::vector<Vector2>& vertices);
	bool IsPointInPolygon(const Vector2& p, const Vector2* vertices, size_t nrVertices);

	bool IsOverlapping(const Vector2& a, const Vector2& b, const Circle2D& c);
	bool IsOverlapping(const Vector2& a, const Vector2& b, const Vector4& r);

	bool IsOverlapping(const Vector4& r1, const Vector4& r2);
	bool IsOverlapping(const Vector4& r, const Circle2D& c);
	bool IsOverlapping(const Circle2D& c1, const Circle2D& c2);
	bool IsOverlapping(const std::vector<Vector2>& vertices, const Circle2D& c);
	bool IsOverlapping(const Vector2* vertices, size_t nrVertices, const Circle2D& c);
	bool Raycast(const Vector2* vertices, const size_t nrVertices, const Vector2& rayP1, const Vector2& rayP2, HitInfo2D& hitInfo);
	bool Raycast(const std::vector<Vector2>& vertices, const Vector2& rayP1, const Vector2& rayP2, HitInfo2D& hitInfo);

	bool IntersectLineSegments(const Vector2& p1, const Vector2& p2, const Vector2& q1, const Vector2& q2, float& outLambda1, float& outLambda2, float epsilon = 1e-6);
	float DistPointLineSegment(const Vector2& p, const Vector2& a, const Vector2& b);
	bool IsPointOnLineSegment(const Vector2& p, const Vector2& a, const Vector2& b);
}