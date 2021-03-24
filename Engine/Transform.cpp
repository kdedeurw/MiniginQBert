#include "pch.h"
#include "Transform.h"

Transform::Transform()
	: m_Position{}
	, m_Rotation{}
	, m_Scale{ 1.f, 1.f }
	, m_Forward{ 1.f, 0.f }
	, m_Right{ 0.f, -1.f }
{}

Transform::Transform(float x, float y, float z, float r)
	: m_Position{ std::move(x), std::move(y), std::move(z) }
	, m_Rotation{ std::move(r) }
{}

Transform::Transform(Vector2 vec2, float z, float r)
	: Transform{ std::move(vec2.x), std::move(vec2.y), std::move(z), std::move(r) }
{}

Transform::Transform(Vector3 vec3, float r)
	: Transform{ std::move(vec3.x), std::move(vec3.y), std::move(vec3.z), std::move(r) }
{}

void Transform::Translate(float x, float y)
{
	m_Position.x += x;
	m_Position.y += y;
}

void Transform::Translate(const Vector2& vec2)
{
	m_Position += vec2;
}

void Transform::Translate(const Vector3& vec3)
{
	m_Position += vec3;
}

void Transform::SetPosition(float x, float y, float z)
{
	m_Position.x = std::move(x);
	m_Position.y = std::move(y);
	m_Position.z = std::move(z);
}

void Transform::SetPosition(Vector2 vec2, float z)
{
	m_Position = std::move(Vector3{ std::move(vec2.x), std::move(vec2.y), std::move(z) });
}

void Transform::SetPosition(Vector3 vec3)
{
	m_Position = std::move(vec3);
}

void Transform::Rotate(float r)
{
	m_Rotation += r;

	CalculateFwdRght();
}

void Transform::SetRotation(float r)
{
	m_Rotation = std::move(r);

	CalculateFwdRght();
}

void Transform::SetScale(float x, float y)
{
	m_Scale.x = std::move(x);
	m_Scale.y = std::move(y);
}

void Transform::CalculateFwdRght()
{
	//WorldRight
	//m_Forward.x = 1.f * cos - 0.f * sin;
	//m_Forward.y = 1.f * sin + 0.f * cos;

	//const float cos = cosf(m_Transform.Rotation);
	//const float sin = sinf(m_Transform.Rotation);

	m_Forward.x = cosf(m_Rotation);
	m_Forward.y = sinf(m_Rotation);
	//m_Right = Math2D::OrthogonalR(m_Forward);
	m_Right.x = m_Forward.y;
	m_Right.y = -m_Forward.x;
}