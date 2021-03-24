#include "pch.h"
#include "GameObject.h"
#include "Vectors.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Components.h"
#include "Transform.h"
#include "Component.h"
#include "Math2D.h"
#include "Subject.h"

GameObject::GameObject(Transform& worldTransform, Transform& localTransform)
	: m_WorldTransform{ worldTransform }
	, m_LocalTransform{ localTransform }
	, m_pComponents{}
	, m_pChildren{}
	, m_pParent{}
	, m_IsActive{ true }
	, m_TransformMask{ TransformMask::All }
	, m_Id{ UINT32_MAX }
{}

GameObject::~GameObject()
{
	m_pParent = nullptr;

	m_pChildren.clear();

	for (Component* pComponent : m_pComponents)
		delete pComponent;
	m_pComponents.clear();
}

void GameObject::Initialize()
{
	std::memcpy(&m_WorldTransform, &m_LocalTransform, sizeof(Transform)); //copy POD data

	for (Component* pComponent : m_pComponents)
	{
		pComponent->Initialize();
	}
}

void GameObject::Update()
{
	if (!m_IsActive)
		return;

	for (Component* pComponent : m_pComponents)
	{
		pComponent->Update();
	}

	if (!m_pParent)
	{
		std::memcpy(&m_WorldTransform, &m_LocalTransform, sizeof(Transform));
	}
	else
	{
		switch (m_TransformMask)
		{
		case TransformMask::All:
			CalculateAllMask();
			break;
		case TransformMask::NoScale:
			CalculateNoScaleMask();
			break;
		case TransformMask::NoRotation:
			CalculateNoRotationMask();
			break;
		case TransformMask::NoScaleNoRot:
			CalculateNoScaleNoRotMask();
			break;
		default:
			CalculateAllMask();
			break;
		}
	}
}

void GameObject::Render() const
{
	if (!m_IsActive)
		return;

	for (Component* pComponent : m_pComponents)
	{
		pComponent->Render();
	}
}

void GameObject::AddComponent(Component* pComponent)
{
	if (std::find(m_pComponents.begin(), m_pComponents.end(), pComponent) == m_pComponents.end())
	{
		m_pComponents.push_back(pComponent);
		pComponent->m_pGameObject = this;
	}
}

void GameObject::RemoveComponent(Component* pComponent)
{
	const std::vector<Component*>::iterator it = std::find(m_pComponents.begin(), m_pComponents.end(), pComponent);
	if (it != m_pComponents.end())
	{
		m_pComponents.erase(it);
		(*it)->m_pGameObject = nullptr;
	}
}

void GameObject::AddChildObject(GameObject* pChild)
{
	if (std::find(m_pChildren.begin(), m_pChildren.end(), pChild) == m_pChildren.end())
	{
		m_pChildren.push_back(pChild);
		pChild->m_pParent = this;
	}
}

void GameObject::RemoveChildObject(GameObject* pChild)
{
	const std::vector<GameObject*>::iterator it = std::find(m_pChildren.begin(), m_pChildren.end(), pChild);
	if (it != m_pChildren.end())
	{
		m_pChildren.erase(it);
		pChild->m_pParent = nullptr;
	}
}

void GameObject::CalculateAllMask() const
{
	//reset world
	std::memset(&m_WorldTransform, 0, sizeof(Transform)); //scale will be 0,0
	Vector2& finalPos{ m_WorldTransform.GetPosition() };
	Vector2& finalScale{ m_WorldTransform.GetScale() };
	float& finalRot{ m_WorldTransform.GetRotation() };
	GameObject* pParent{ m_pParent };

	//TODO: these values are 0?
	//calculate pivot rotational point
	const float parentRot = -pParent->GetLocalTransform().GetRotation() * Math2D::ToRadians;
	const float sinCalc{ sinf(parentRot) };
	const float cosCalc{ cosf(parentRot) };
	const float newX{ finalPos.x * cosCalc - finalPos.y * sinCalc };
	finalPos.y = finalPos.y * cosCalc + finalPos.x * sinCalc;
	finalPos.x = newX;

	finalRot += pParent->GetLocalTransform().GetRotation();

	//add every parent's transform ontop
	do
	{
		finalPos += pParent->GetLocalTransform().GetPosition();
		finalScale += pParent->GetLocalTransform().GetScale();
		finalScale -= 1.f;
		finalRot += pParent->GetLocalTransform().GetRotation();

		pParent = pParent->m_pParent;
	}
	while (pParent);
	
	//add own local transform
	m_WorldTransform.GetPosition() += m_LocalTransform.GetPosition();
	m_WorldTransform.GetScale() += m_LocalTransform.GetScale();
	//m_WorldTransform.GetScale() -= 1.f; //counter starting scale 0,0
	m_WorldTransform.GetRotation() += m_LocalTransform.GetRotation();
}

void GameObject::CalculateNoScaleMask() const
{
	std::memset(&m_WorldTransform, 0, sizeof(Transform));
	Vector2& finalPos{ m_WorldTransform.GetPosition() };
	float& finalRot{ m_WorldTransform.GetRotation() };
	GameObject* pParent{ m_pParent };

	const float parentRot = -pParent->GetLocalTransform().GetRotation() * Math2D::ToRadians;
	const float sinCalc{ sinf(parentRot) };
	const float cosCalc{ cosf(parentRot) };
	const float newX{ finalPos.x * cosCalc - finalPos.y * sinCalc };
	finalPos.y = finalPos.y * cosCalc + finalPos.x * sinCalc;
	finalPos.x = newX;

	finalRot += pParent->GetLocalTransform().GetRotation();

	do
	{
		finalPos += pParent->GetLocalTransform().GetPosition();
		finalRot += pParent->GetLocalTransform().GetRotation();

		pParent = pParent->m_pParent;
	}
	while (pParent);

	m_WorldTransform.GetPosition() += m_LocalTransform.GetPosition();
	m_WorldTransform.GetScale() += m_LocalTransform.GetScale();
	m_WorldTransform.GetRotation() += m_LocalTransform.GetRotation();
}

void GameObject::CalculateNoRotationMask() const
{
	std::memset(&m_WorldTransform, 0, sizeof(Transform));
	Vector2& finalPos{ m_WorldTransform.GetPosition() };
	Vector2& finalScale{ m_WorldTransform.GetScale() };
	GameObject* pParent{ m_pParent };

	//invert parent's rotation
	const float parentRot = -pParent->GetLocalTransform().GetRotation() * Math2D::ToRadians;
	const float sinCalc{ sinf(parentRot) };
	const float cosCalc{ cosf(parentRot) };
	//translate pivot point to origin

	//TODO: fix pivot offset according to m_Pivot from Texture2DComponent
	const Vector2& offset = pParent->GetLocalTransform().GetPosition();
	finalPos -= offset;
	//this offset will be added again below
	const float newX{ finalPos.x * cosCalc - finalPos.y * sinCalc };
	finalPos.y = finalPos.y * cosCalc + finalPos.x * sinCalc;
	finalPos.x = newX;

	//set rotation equal to parent
	m_WorldTransform.GetRotation() += pParent->GetLocalTransform().GetRotation();

	do
	{
		finalPos += pParent->GetLocalTransform().GetPosition();
		finalScale += pParent->GetLocalTransform().GetScale();
		finalScale -= 1.f;

		pParent = pParent->m_pParent;
	}
	while (pParent);

	//m_WorldTransform.GetPosition() += m_LocalTransform.GetPosition(); //already added in loop
	m_WorldTransform.GetScale() += m_LocalTransform.GetScale();
	m_WorldTransform.GetRotation() += m_LocalTransform.GetRotation();
}

void GameObject::CalculateNoScaleNoRotMask() const
{
	std::memset(&m_WorldTransform, 0, sizeof(Transform));
	Vector2& finalPos{ m_WorldTransform.GetPosition() };
	GameObject* pParent{ m_pParent };

	do
	{
		finalPos += pParent->GetLocalTransform().GetPosition();

		pParent = pParent->m_pParent;
	}
	while (pParent);

	m_WorldTransform.GetPosition() += m_LocalTransform.GetPosition();
	m_WorldTransform.GetScale() += m_LocalTransform.GetScale();
	m_WorldTransform.GetRotation() += m_LocalTransform.GetRotation();
}