#include "pch.h"
#include "Camera.h"
#include "Math2D.h"

Camera::Camera()
	: m_Position{ 0.f, 0.f }
	, m_Scale{ 1.f, 1.f }
	, m_Rotation{ 0.f }
	, m_PerpendicularForward{ 0.f, 1.f }
	, m_Right{ 1.f, 0.f }
{}

Camera::~Camera()
{}

Vector2& Camera::GetPosition()
{
	return m_Position;
}

Vector2& Camera::GetScale()
{
	return m_Scale;
}

float& Camera::GetRotation()
{
	return m_Rotation;
}

Vector2& Camera::GetForward()
{
	return m_PerpendicularForward;
}

Vector2& Camera::GetRight()
{
	return m_Right;
}

void Camera::SetRotation(float degrees)
{
	m_Rotation = degrees;
	CalculateFwdRight();
}

void Camera::Translate(float x, float y)
{
	m_Position.x += x;
	m_Position.y += y;
}

void Camera::Translate(const Vector2& trans)
{
	m_Position += trans;
}

void Camera::Scale(float x, float y)
{
	m_Scale.x += x;
	m_Scale.y += y;
}

void Camera::Scale(const Vector2& scale)
{
	m_Scale += scale;
}

void Camera::Rotate(float degrees)
{
	m_Rotation += degrees;
	CalculateFwdRight();
}

void Camera::CalculateFwdRight()
{
	const float correctedAngleRad = (90.f + m_Rotation) * Math2D::ToRadians;
	m_PerpendicularForward.x = cosf(correctedAngleRad);
	m_PerpendicularForward.y = sinf(correctedAngleRad);
	m_Right.x = m_PerpendicularForward.y;
	m_Right.y = -m_PerpendicularForward.x;
}