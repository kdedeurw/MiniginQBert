#pragma once
#include "Vectors.h"

class Transform final
{
public:
	Transform();
	Transform(float x, float y, float z, float r = 0.0f);
	Transform(Vector2 vec2, float z, float r = 0.0f);
	Transform(Vector3 vec3, float r = 0.0f);

	~Transform() = default;
	Transform(const Transform& other) = default;
	Transform(Transform&& other) = default;
	Transform& operator=(const Transform& other) = default;
	Transform& operator=(Transform&& other) = default;

	Vector3& GetPosition() { return m_Position; }
	float& GetRotation() { return m_Rotation; };
	Vector2& GetScale() { return m_Scale; };
	Vector2& GetForward() { return m_Forward; };
	Vector2& GetRight() { return m_Right; };

	void Translate(float x, float y);
	void Translate(const Vector2& vec2);
	void Translate(const Vector3& vec3);
	void SetPosition(float x, float y, float z = 0.f);
	void SetPosition(Vector2 vec2, float z = 0.f);
	void SetPosition(Vector3 vec3);

	void Rotate(float r);
	void SetRotation(float r);

	void SetScale(float x, float y);
	void SetScale(Vector2 scale) { m_Scale = std::move(scale); };

private:
	Vector3 m_Position;
	Vector2 m_Scale;
	float m_Rotation;
	Vector2 m_Forward;
	Vector2 m_Right;

	void CalculateFwdRght();
};