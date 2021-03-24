#include "pch.h"
#include "Scene.h"
#include "GameObject.h"
#include "GlobalMemoryPools.h"
#include "MemoryAllocator.h"
#include <algorithm>

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(std::string name)
	: m_Name{ std::move(name) }
	, m_GameObjects{}
	, m_AmountOfGameObjects{}
{}

Scene::~Scene()
{
	for (GameObject* pGameObject : m_GameObjects)
	{
		delete pGameObject;
	}
	m_GameObjects.clear();
}

GameObject* Scene::CreateGameObject()
{
	GameObject* pGameObject = GlobalMemoryPools::GetInstance().CreateGameObject();
	pGameObject->m_Id = m_IdCounter;
	++m_IdCounter;
	m_GameObjects.push_back(pGameObject);
	return pGameObject;
}

void Scene::RemoveGameObject(GameObject* pGameObject)
{
	const auto it = std::find(m_GameObjects.begin(), m_GameObjects.end(), pGameObject);
	if (it != m_GameObjects.end())
	{
		m_GameObjects.erase(it);
		delete (*it);
	}
}

void Scene::Initialize()
{
	for (GameObject* pGameObject : m_GameObjects)
	{
		pGameObject->Initialize();
	}
}

void Scene::Update()
{
	const size_t size = m_GameObjects.size();
	if (m_AmountOfGameObjects != size) //detected change
	{
		std::sort(m_GameObjects.begin(), m_GameObjects.end(), [](GameObject* pObj, GameObject* pObj2)->bool { return pObj->GetWorldTransform().GetPosition().z > pObj2->GetWorldTransform().GetPosition().z; });
		m_AmountOfGameObjects = size;
	}
	for (size_t i{}; i < size; ++i)
	{
		//safe to add objects during run-time
		m_GameObjects[i]->Update();
	}
}

void Scene::Render() const
{
	for (GameObject* pObject : m_GameObjects)
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