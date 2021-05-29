#include "TransformComponent.h"
#include "GameObject.h"
#include "Math2D.h"

TransformComponent::TransformComponent()
	: m_TransformChanges{ TransformChange::None }
	, m_Local{}
	, m_World{}
{
}

void TransformComponent::Initialize()
{
}

void TransformComponent::Render() const
{
}

void TransformComponent::Update()
{
	if (m_TransformChanges == TransformChange::None)
		return;

	if (m_TransformChanges & TransformChange::Translation)
	{
		CalculateWorldPosition();
	}
	if (m_TransformChanges & TransformChange::Rotation)
	{
		CalculateWorldRotation();
		CalculateFwdRight();
	}
	if (m_TransformChanges & TransformChange::Scale)
	{
		CalculateWorldScale();
	}

	m_TransformChanges = TransformChange::None;
}

Transform& TransformComponent::GetLocal()
{
	return m_Local;
}

Transform& TransformComponent::GetWorld()
{
	Update(); //recalculate world transform if changed
	return m_World;
}

void TransformComponent::Translate(float x, float y)
{
	m_Local.Position.x += x;
	m_Local.Position.y += y;
	m_TransformChanges |= TransformChange::Translation;
}

void TransformComponent::Translate(const Vector2& vec2)
{
	m_Local.Position += vec2;
	m_TransformChanges |= TransformChange::Translation;
}

void TransformComponent::Translate(const Vector3& vec3)
{
	m_Local.Position += vec3;
	m_TransformChanges |= TransformChange::Translation;
}

void TransformComponent::SetPosition(float x, float y, float z)
{
	m_Local.Position.x = x;
	m_Local.Position.y = y;
	m_Local.Position.z = z;
	m_TransformChanges |= TransformChange::Translation;
}

void TransformComponent::SetPosition(const Vector2& vec2, float z)
{
	m_Local.Position.x = vec2.x;
	m_Local.Position.y = vec2.y;
	m_Local.Position.z = z;
	m_TransformChanges |= TransformChange::Translation;
}

void TransformComponent::SetPosition(const Vector3& vec3)
{
	m_Local.Position = vec3;
	m_TransformChanges |= TransformChange::Translation;
}

void TransformComponent::Rotate(float r)
{
	m_Local.Rotation += r;
	m_TransformChanges |= TransformChange::Rotation;
}

void TransformComponent::SetRotation(float r)
{
	m_Local.Rotation = r;
	m_TransformChanges |= TransformChange::Rotation;
}

void TransformComponent::SetScale(float x, float y)
{
	m_Local.Scale.x = x; 
	m_Local.Scale.y = y;
	m_TransformChanges |= TransformChange::Scale;
}

void TransformComponent::SetScale(const Vector2& scale)
{
	m_Local.Scale = scale;
	m_TransformChanges |= TransformChange::Scale;
}

void TransformComponent::CalculateWorldPosition()
{
	GameObject* pParent{ &m_pGameObject->GetParent() };

	if (!pParent)
	{
		m_World.Position = m_Local.Position;
		return;
	}

	m_World.Position.x = 0.f;
	m_World.Position.y = 0.f;
	Vector2& finalPos{ m_World.Position };

	//calculate pivot rotational point
	const float parentRot = -pParent->GetTransform().GetLocal().Rotation * Math2D::ToRadians;
	const float sinCalc{ sinf(parentRot) };
	const float cosCalc{ cosf(parentRot) };
	const float newX{ finalPos.x * cosCalc - finalPos.y * sinCalc };
	finalPos.y = finalPos.y * cosCalc + finalPos.x * sinCalc;
	finalPos.x = newX;

	finalPos += pParent->GetTransform().GetWorld().Position;

	finalPos += m_Local.Position;
}

void TransformComponent::CalculateWorldRotation()
{
	GameObject* pParent{ &m_pGameObject->GetParent() };

	if (!pParent)
	{
		m_World.Rotation = m_Local.Rotation;
		return;
	}

	float& finalRot{ m_World.Rotation };
	finalRot = 0.f;

	finalRot += pParent->GetTransform().GetLocal().Rotation;

	finalRot += pParent->GetTransform().GetWorld().Rotation;

	finalRot += m_Local.Rotation;
}

void TransformComponent::CalculateWorldScale()
{
	GameObject* pParent{ &m_pGameObject->GetParent() };

	if (!pParent)
	{
		m_World.Scale = m_Local.Scale;
		return;
	}

	Vector2& finalScale{ m_World.Scale };
	finalScale = m_Local.Scale;

	const Vector2& parentWorldScale = pParent->GetTransform().GetWorld().Scale;
	finalScale.x *= parentWorldScale.x;
	finalScale.y *= parentWorldScale.y;
}

void TransformComponent::CalculateFwdRight()
{
	//WorldRight
	//m_Forward.x = 1.f * cos - 0.f * sin;
	//m_Forward.y = 1.f * sin + 0.f * cos;

	//const float cos = cosf(m_Transform.Rotation);
	//const float sin = sinf(m_Transform.Rotation);

	m_Local.Forward.x = cosf(m_Local.Rotation);
	m_Local.Forward.y = sinf(m_Local.Rotation);
	//m_Right = Math2D::OrthogonalR(m_Forward);
	m_Local.Right.x = m_Local.Forward.y;
	m_Local.Right.y = -m_Local.Forward.x;

	m_World.Forward.x = cosf(m_World.Rotation);
	m_World.Forward.y = sinf(m_World.Rotation);

	m_World.Right.x = m_World.Forward.y;
	m_World.Right.y = -m_World.Forward.x;
}