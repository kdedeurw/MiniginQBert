#include "pch.h"
#include "Scene.h"
#include "GameObject.h"
#include "GlobalMemoryPools.h"
#include "MemoryAllocator.h"
#include <algorithm>

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(std::string name)
	: m_Name{ std::move(name) }
	, m_pGameObjects{}
{}

Scene::~Scene()
{
	for (GameObject* pGameObject : m_pGameObjects)
	{
		delete pGameObject;
	}
	m_pGameObjects.clear();
}

GameObject* Scene::CreateGameObject()
{
	GameObject* pGameObject = GlobalMemoryPools::GetInstance().CreateGameObject();
	pGameObject->m_Id = m_IdCounter;
	++m_IdCounter;
	m_pGameObjects.push_back(pGameObject);
	return pGameObject;
}

void Scene::RemoveGameObject(GameObject* pGameObject)
{
	const auto it = std::find(m_pGameObjects.begin(), m_pGameObjects.end(), pGameObject);
	if (it != m_pGameObjects.end())
	{
		m_pGameObjects.erase(it);
		delete (*it);
	}
}

void Scene::Initialize()
{
	for (GameObject* pGameObject : m_pGameObjects)
	{
		pGameObject->Initialize();
	}
}

void Scene::Update()
{
	std::sort(m_pGameObjects.begin(), m_pGameObjects.end(), [](GameObject* pObj, GameObject* pObj2)->bool { return pObj->GetWorldTransform().GetPosition().z > pObj2->GetWorldTransform().GetPosition().z; });
	const size_t size = m_pGameObjects.size();
	for (size_t i{}; i < size; ++i)
	{
		//safe to add objects during run-time
		m_pGameObjects[i]->Update();
	}
}

void Scene::Render() const
{
	for (GameObject* pObject : m_pGameObjects)
	{
		pObject->Render();
	}
}

bool Scene::operator==(const Scene& other) const
{
	return m_Name == other.m_Name;
}

bool Scene::operator==(Scene* pOther) const
{
	return this->m_Name == pOther->m_Name;
}