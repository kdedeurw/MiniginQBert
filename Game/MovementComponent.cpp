#include "pch.h"
#include "MovementComponent.h"
#include "GameState.h"
#include "Math2D.h"

MovementComponent::MovementComponent()
	: m_DesiredVelocity{}
	, m_AngularVelocity{}
	, m_Weight{}
{
}

MovementComponent::~MovementComponent()
{
}

void MovementComponent::Initialize(bool)
{
}

void MovementComponent::Render() const
{
}

void MovementComponent::Update()
{
	TransformComponent& transform = GetGameObject()->GetTransform();
	//Vector2& pos = transform.GetPosition();

	const float deltaTime = GameState::GetInstance().DeltaTime;

	transform.Translate(m_DesiredVelocity * deltaTime);
	transform.Rotate(m_AngularVelocity * deltaTime);

	m_CurrentVelocity = Math2D::LERP(m_CurrentVelocity, m_DesiredVelocity, deltaTime / m_Weight);
}

float MovementComponent::GetAngularVelocity() const
{
	return m_AngularVelocity;
}

const Vector2& MovementComponent::GetVelocity() const
{
	return m_DesiredVelocity;
}

void MovementComponent::AddAngularVelocity(float vel)
{
	m_AngularVelocity += vel;
}

void MovementComponent::AddVelocity(const Vector2& vel)
{
	m_DesiredVelocity += vel;
}

void MovementComponent::SetDesiredVelocity(const Vector2& vel)
{
	m_DesiredVelocity = vel;
}

void MovementComponent::SetCurrentVelocity(const Vector2& vel)
{
	m_CurrentVelocity = vel;
}

void MovementComponent::SetAngularVelocity(float vel)
{
	m_AngularVelocity = vel;
}

void MovementComponent::AutoOrient()
{
	TransformComponent& transform = GetGameObject()->GetTransform();
	transform.SetRotation(Math2D::OrientOnWorld(m_CurrentVelocity));
}