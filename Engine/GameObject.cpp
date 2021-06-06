#include "pch.h"
#include "GameObject.h"
#include "Vectors.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "Components.h"
#include "TransformComponent.h"
#include "Component.h"
#include "Math2D.h"
#include "Subject.h"

GameObject::GameObject(TransformComponent& transform)
	: m_Transform{ transform }
	, m_pComponents{}
	, m_pChildren{}
	, m_pParent{}
	, m_IsActive{ true }
	, m_IsRendered{ true }
	, m_Id{ UINT32_MAX }
{
	AddComponent(&transform);
}

GameObject::~GameObject() noexcept
{
	m_pParent = nullptr;

	m_pChildren.clear();

	for (Component* pComponent : m_pComponents)
		delete pComponent;
	m_pComponents.clear();
}

void GameObject::Initialize()
{
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
		if (!m_pComponents.size())
			break;
		pComponent->Update();
	}
}

void GameObject::Render() const
{
	if (!m_IsRendered)
		return;

	for (Component* pComponent : m_pComponents)
	{
		if (!m_pComponents.size())
			break;
		pComponent->Render();
	}
}

void GameObject::PostRender() const
{
	if (!m_IsRendered)
		return;

	for (Component* pComponent : m_pComponents)
	{
		if (!m_pComponents.size())
			break;
		pComponent->PostRender();
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

void GameObject::RemoveComponent(Component* pComponent, bool isDelete)
{
	const std::vector<Component*>::iterator it = std::find(m_pComponents.begin(), m_pComponents.end(), pComponent);
	if (it != m_pComponents.end())
	{
		m_pComponents.erase(it);
		if (isDelete)
			delete *it;
		else
			(*it)->m_pGameObject = nullptr;
	}
}

void GameObject::AddChildObject(GameObject* pChild)
{
	if (std::find(m_pChildren.begin(), m_pChildren.end(), pChild) == m_pChildren.end())
	{
		m_pChildren.push_back(pChild);
		pChild->m_pParent = this;
		pChild->GetTransform().Initialize();
	}
}

void GameObject::RemoveChildObject(GameObject* pChild, bool isDelete)
{
	const std::vector<GameObject*>::iterator it = std::find(m_pChildren.begin(), m_pChildren.end(), pChild);
	if (it != m_pChildren.end())
	{
		m_pChildren.erase(it);
		if (isDelete)
			delete pChild;
		else
		{
			pChild->m_pParent = nullptr;
			pChild->GetTransform().Initialize();
		}
	}
}