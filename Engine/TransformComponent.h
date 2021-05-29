#pragma once
#include "Component.h"
#include "Transform.h"

class TransformComponent final : public Component
{
	using BYTE = unsigned char;
	enum TransformChange : BYTE
	{
		None = 0,
		Translation = 1,
		Rotation = 2,
		Scale = 4,
	};
public:
	TransformComponent();

	~TransformComponent() = default;
	TransformComponent(const TransformComponent& other) = default;
	TransformComponent(TransformComponent&& other) = default;
	TransformComponent& operator=(const TransformComponent& other) = default;
	TransformComponent& operator=(TransformComponent&& other) = default;

	void Initialize() override;
	void Render() const override;
	void Update() override;

	Transform& GetLocal();
	Transform& GetWorld();

	void Translate(float x, float y);
	void Translate(const Vector2& vec2);
	void Translate(const Vector3& vec3);
	void SetPosition(float x, float y, float z = 0.f);
	void SetPosition(const Vector2& vec2, float z = 0.f);
	void SetPosition(const Vector3& vec3);

	void Rotate(float r);
	void SetRotation(float r);

	void SetScale(float x, float y);
	void SetScale(const Vector2& scale);

private:
	BYTE m_TransformChanges;
	Transform m_Local;
	Transform m_World;

	void CalculateWorldPosition();
	void CalculateWorldRotation();
	void CalculateWorldScale();

	void CalculateFwdRight();
};