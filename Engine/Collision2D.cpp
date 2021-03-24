#define NOMINMAX
#include "pch.h"
#include "Collision2D.h"
#include <algorithm>

using namespace Collision2D;

Circle2D::Circle2D(float x, float y, float radius)
	: center{ x, y }
	, radius{ radius }
{}

Circle2D::Circle2D(const Vector2& center, float radius)
	: Circle2D{ center.x, center.y, radius }
{}

Circle2D::Circle2D(const Vector3& vec3)
	: Circle2D{ vec3.x, vec3.y, vec3.z }
{}

//-------------------------------------------------------------

bool Collision2D::IsPointInBox(const Vector2& p, const Vector4& r)
{
	return (p.x >= r.x && p.x <= r.x + r.z &&
		p.y >= r.y && p.y <= r.y + r.w);
}

bool Collision2D::IsPointInCircle(const Vector2& p, const Circle2D& c)
{
	const float squaredDist = (p.x - c.center.x) * (p.x - c.center.x) + (p.y - c.center.y) * (p.y - c.center.y);
	const float squaredRadius = c.radius * c.radius;
	return squaredRadius >= squaredDist;
}

bool Collision2D::IsPointInCircle(const Vector2& lhs, const Vector2& rhs, float radius)
{
	if (lhs.x <= rhs.x + radius && lhs.x >= rhs.x - radius && lhs.y <= rhs.y + radius && lhs.y >= rhs.y - radius)
		return true;

	return false;
}

bool Collision2D::IsOverlapping(const Vector2& a, const Vector2& b, const Vector4& r)
{
	// if one of the line segment end points is in the rect
	if (Collision2D::IsPointInBox(a, r) || Collision2D::IsPointInBox(b, r))
		return true;

	HitInfo2D hitInfo{};
	Vector2 vertices[]{ Vector2 {r.x, r.y},
		Vector2{ r.x + r.z, r.y },
		Vector2{ r.x + r.z, r.y + r.w },
		Vector2{ r.x, r.y + r.w } };

	return Collision2D::Raycast(vertices, 4, a, b, hitInfo);
}

bool Collision2D::IsOverlapping(const Vector4& r1, const Vector4& r2)
{
	// If one rectangle is on left side of the other
	if ((r1.x + r1.z) < r2.x || (r2.x + r2.z) < r1.x)
		return false;

	// If one rectangle is under the other
	if (r1.y > (r2.y + r2.w) || r2.y > (r1.y + r1.w))
		return false;

	return true;
}

bool Collision2D::IsOverlapping(const Vector4& r, const Circle2D& c)
{
	// Is center of circle in the rectangle?
	if (Collision2D::IsPointInBox(c.center, r))
		return true;

	// Check line segments
	if (Collision2D::DistPointLineSegment(c.center, Vector2{ r.x, r.y }, Vector2{ r.x, r.y + r.w }) <= c.radius) return true;
	if (Collision2D::DistPointLineSegment(c.center, Vector2{ r.x, r.y }, Vector2{ r.x + r.z, r.y }) <= c.radius) return true;
	if (Collision2D::DistPointLineSegment(c.center, Vector2{ r.x + r.z, r.y + r.w }, Vector2{ r.x, r.y + r.w }) <= c.radius) return true;
	if (Collision2D::DistPointLineSegment(c.center, Vector2{ r.x + r.z, r.y + r.w }, Vector2{ r.x + r.z, r.y }) <= c.radius) return true;
	return false;
}

bool Collision2D::IsOverlapping(const Circle2D& c1, const Circle2D& c2)
{
	// squared distance between centers
	const float xDistance{ c1.center.x - c2.center.x };
	const float yDistance{ c1.center.y - c2.center.y };
	const float squaredDistance = xDistance * xDistance + yDistance * yDistance;

	const float squaredTouchingDistance = (c1.radius + c2.radius) * (c1.radius + c2.radius);
	if (squaredDistance < squaredTouchingDistance)
		return true;

	return false;
}

bool Collision2D::IsOverlapping(const Vector2& a, const Vector2& b, const Circle2D& c)
{
	return Collision2D::DistPointLineSegment(c.center, a, b) <= c.radius;
}

bool Collision2D::IsOverlapping(const std::vector<Vector2>& vertices, const Circle2D& c)
{
	return Collision2D::IsOverlapping(vertices.data(), vertices.size(), c);
}

bool Collision2D::IsOverlapping(const Vector2* vertices, size_t nrVertices, const Circle2D& c)
{
	// Check points in circle
	for (size_t i{ 0 }; i < nrVertices; ++i)
	{
		if (Collision2D::IsPointInCircle(vertices[i], c))
			return true;
	}

	// Check overlapping line segments with circle
	for (size_t i{ 0 }; i < nrVertices; ++i)
	{
		if (Collision2D::DistPointLineSegment(c.center, vertices[i], vertices[(i + 1) % nrVertices]) <= c.radius)
			return true;
	}

	// No overlapping line segments, verify whether circle is inside polygon
	if (Collision2D::IsPointInPolygon(c.center, vertices, nrVertices))
		return true;

	return false;
}

bool Collision2D::IsPointInPolygon(const Vector2& p, const std::vector<Vector2>& vertices)
{
	return Collision2D::IsPointInPolygon(p, vertices.data(), vertices.size());
}

bool Collision2D::IsPointInPolygon(const Vector2& p, const Vector2* vertices, size_t nrVertices)
{
	//if (nrVertices < 2)
	//	return false;

	// 1. First do a simple test with axis aligned bounding box around the polygon
	float xMin{ vertices[0].x };
	float xMax{ vertices[0].x };
	float yMin{ vertices[0].y };
	float yMax{ vertices[0].y };
	for (size_t idx{ 1 }; idx < nrVertices; ++idx)
	{
		if (xMin > vertices[idx].x)
			xMin = vertices[idx].x;
		if (xMax < vertices[idx].x)
			xMax = vertices[idx].x;
		if (yMin > vertices[idx].y)
			yMin = vertices[idx].y;
		if (yMax < vertices[idx].y)
			yMax = vertices[idx].y;
	}
	if (p.x < xMin || p.x > xMax || p.y < yMin || p.y > yMax)
		return false;

	// 2. Draw a virtual ray from anywhere outside the polygon to the point 
	//    and count how often it hits any side of the polygon. 
	//    If the number of hits is even, it's outside of the polygon, if it's odd, it's inside.
	int numberOfIntersectionPoints{ 0 };
	Vector2 p2{ xMax + 10.0f, p.y }; // Horizontal line from point to point outside polygon (p2)

	// Count the number of intersection points
	float lambda1{}, lambda2{};
	for (size_t i{ 0 }; i < nrVertices; ++i)
	{
		if (Collision2D::IntersectLineSegments(vertices[i], vertices[(i + 1) % nrVertices], p, p2, lambda1, lambda2))
		{
			if (lambda1 > 0 && lambda1 <= 1 && lambda2 > 0 && lambda2 <= 1)
			{
				++numberOfIntersectionPoints;
			}
		}
	}
	if (numberOfIntersectionPoints % 2 == 0)
		return false;
	else
		return true;
}

bool Collision2D::IntersectLineSegments(const Vector2& p1, const Vector2& p2, const Vector2& q1, const Vector2& q2, float& outLambda1, float& outLambda2, float epsilon)
{
	const Vector2 p1p2{ p1 - p2 };
	const Vector2 q1q2{ q1 - q2 };

	// cross product to determine if parallel
	const float denom = Math2D::Cross(p1p2, q1q2);

	// Don't divide by zero
	if (std::abs(denom) > epsilon)
	{
		const Vector2 p1q1{ p1 - q1 };

		const float num1 = Math2D::Cross(p1q1, q1q2);
		const float num2 = Math2D::Cross(p1q1, p1p2);
		outLambda1 = num1 / denom;
		outLambda2 = num2 / denom;

		return true;
	}
	else // are parallel
	{
		//connect start points
		const Vector2 p1q1{ p1 - q1 };

		// cross product to determine if segments and the line connecting their start points are parallel, 
		// if so, than they are on a line
		// if not, then there is no intersection
		const float denom2 = Math2D::Cross(p1q1, q1q2);
		if (std::abs(denom2) > epsilon)
			return false;

		// check the 4 conditions
		outLambda1 = 0;
		outLambda2 = 0;
		if (Collision2D::IsPointOnLineSegment(p1, q1, q2)) return true;
		if (Collision2D::IsPointOnLineSegment(p2, q1, q2)) return true;
		if (Collision2D::IsPointOnLineSegment(q1, p1, p2)) return true;
		if (Collision2D::IsPointOnLineSegment(q2, p1, p2)) return true;
	}
	return false;
}
bool Collision2D::Raycast(const std::vector<Vector2>& vertices, const Vector2& rayP1, const Vector2& rayP2, HitInfo2D& hitInfo)
{
	return Collision2D::Raycast(vertices.data(), vertices.size(), rayP1, rayP2, hitInfo);
}

bool Collision2D::Raycast(const Vector2* vertices, const size_t nrVertices, const Vector2& rayP1, const Vector2& rayP2, HitInfo2D& hitInfo)
{
	//if (nrVertices == 0)
	//	return false;

	std::vector<HitInfo2D> hits;

	Vector4 r1, r2;
	// r1: minimal AABB rect enclosing the ray
	r1.x = std::min(rayP1.x, rayP2.x);
	r1.y = std::min(rayP1.y, rayP2.y);
	r1.z = std::max(rayP1.x, rayP2.x) - r1.x;
	r1.w = std::max(rayP1.y, rayP2.y) - r1.y;

	// Line-line intersections.
	for (size_t idx{ 0 }; idx <= nrVertices; ++idx)
	{
		// Consider line segment between 2 consecutive vertices
		// (modulo to allow closed polygon, last - first vertice)
		const Vector2 q1 = vertices[(idx + 0) % nrVertices];
		const Vector2 q2 = vertices[(idx + 1) % nrVertices];

		// r2: minimal AABB rect enclosing the 2 vertices
		r2.x = std::min(q1.x, q2.x);
		r2.y = std::min(q1.y, q2.y);
		r2.z = std::max(q1.x, q2.x) - r2.x;
		r2.w = std::max(q1.y, q2.y) - r2.y;

		if (Collision2D::IsOverlapping(r1, r2))
		{
			float lambda1{};
			float lambda2{};
			if (IntersectLineSegments(rayP1, rayP2, q1, q2, lambda1, lambda2))
			{
				if (lambda1 > 0 && lambda1 <= 1 && lambda2 > 0 && lambda2 <= 1)
				{
					HitInfo2D newHitInfo;
					newHitInfo.lambda = lambda1;
					newHitInfo.intersectPoint = { rayP1.x + ((rayP2.x - rayP1.x) * lambda1), rayP1.y + ((rayP2.y - rayP1.y) * lambda1) };
					newHitInfo.normal = Math2D::GetNormalized(Math2D::OrthogonalL(q2 - q1));
					hits.push_back(newHitInfo);
				}
			}
		}
	}

	if (hits.size() == 0)
		return false;

	// Get closest intersection point and copy it into the hitInfo parameter
	hitInfo = *std::min_element(hits.begin(), hits.end(),
		[](const HitInfo2D& first, const HitInfo2D& last)
		{
			return first.lambda < last.lambda;
		});

	return true;
}

bool  Collision2D::IsPointOnLineSegment(const Vector2& p, const Vector2& a, const Vector2& b)
{
	Vector2 ap{ a - p }, bp{ b - p };
	// If not on same line, return false
	if (abs(Math2D::Cross(ap, bp)) > 0.001f)
		return false;

	// Both vectors must point in opposite directions if p is between p1 and p2
	if (Math2D::Dot(ap, bp) > 0)
		return false;

	return true;
}

float Collision2D::DistPointLineSegment(const Vector2& p, const Vector2& a, const Vector2& b)
{
	Vector2 ab{ a - b };
	Vector2 ap{ a - p };
	Vector2 abNorm = Math2D::GetNormalized(ab);
	const float distToA = Math2D::Dot(abNorm, ap);
	// if distToA is negative, then the closest point is A
	// return the distance a, p
	if (distToA < 0)
		return Math2D::Magnitude(ap);

	// if distToA is > than dist(a,b) then the closest point is B
	// return the distance b, p
	const float distAB = Math2D::Magnitude(ab);
	if (distToA > distAB)
		return Math2D::Magnitude(b - p);

	//closest point is between A and B, calc intersection point
	Vector2 intersection = abNorm * Math2D::Dot(abNorm, ap) + Vector2{ a };
	return Math2D::Magnitude(p - intersection);
}