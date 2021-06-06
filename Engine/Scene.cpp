#include "pch.h"
#include "Scene.h"
#include "GameObject.h"
#include <algorithm>

unsigned int Scene::m_IdCounter = 0;

Scene::Scene(std::string name)
	: m_Name{ std::move(name) }
	, m_GameObjects{}
{}

void Scene::SortGameObjects()
{
	std::sort(m_GameObjects.begin(), m_GameObjects.end(), [](GameObject* pObj, GameObject* pObj2)->bool
		{
			return pObj->GetTransform().GetWorld().Position.z > pObj2->GetTransform().GetWorld().Position.z;
		});
}

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
	//GameObject* pGameObject = GlobalMemoryPools::GetInstance().CreateGameObject();
	GameObject* pGameObject = new GameObject{};
	pGameObject->m_Id = m_IdCounter;
	++m_IdCounter;
	m_GameObjects.push_back(pGameObject);
	pGameObject->m_pScene = this;

	SortGameObjects();

	return pGameObject;
}

void Scene::RemoveGameObject(GameObject* pGameObject, bool isDelete)
{
	const auto it = std::find(m_GameObjects.begin(), m_GameObjects.end(), pGameObject);
	if (it != m_GameObjects.end())
	{
		m_GameObjects.erase(it);
		if (isDelete)
			delete pGameObject;

		SortGameObjects();
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
	for (GameObject* pGameObject : m_GameObjects)
	{
		pGameObject->Update();
	}
}

void Scene::Render() const
{
	for (GameObject* pObject : m_GameObjects)
	{
		pObject->Render();
	}
}

void Scene::PostRender() const
{
	for (GameObject* pObject : m_GameObjects)
	{
		pObject->PostRender();
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