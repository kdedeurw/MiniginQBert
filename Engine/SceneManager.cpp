#include "pch.h"
#include "SceneManager.h"
#include "Scene.h"
#include "GameState.h"
#include "MemoryAllocator.h"

SceneManager::SceneManager()
	: m_pAllScenes{}
	, m_pActiveScenes{}
	, m_pUIScene{}
{}

SceneManager::~SceneManager()
{
	for (Scene* pScene : m_pAllScenes)
	{
		delete pScene;
	}
	m_pAllScenes.clear();
	m_pActiveScenes.clear();
	if (m_pUIScene)
		delete m_pUIScene;
	m_pUIScene = nullptr;
}

void SceneManager::Initialize()
{
	for (Scene* pScene : m_pAllScenes)
	{
		pScene->Initialize();
	}
	if (m_pUIScene)
		m_pUIScene->Initialize();
}

void SceneManager::Update()
{
	for(Scene* pScene : m_pActiveScenes)
	{
		pScene->Update();
	}
	if (m_pUIScene)
		m_pUIScene->Update();
}

void SceneManager::Render() const
{
	for (const Scene* pScene : m_pActiveScenes)
	{
		pScene->Render();
	}
	if (m_pUIScene)
		m_pUIScene->Render();
}

Scene& SceneManager::CreateUIScene()
{
	if (m_pUIScene)
		return *m_pUIScene;

	m_pUIScene = GlobalMemoryPools::GetInstance().CreateScene("UI");
	GameState::GetInstance().pUIScene = m_pUIScene;

	return *m_pUIScene;
}

Scene& SceneManager::CreateScene(const std::string& name, bool isActive)
{
	Scene* pScene = GlobalMemoryPools::GetInstance().CreateScene(name.c_str());
	m_pAllScenes.push_back(pScene);
	if (isActive)
	{
		m_pActiveScenes.push_back(pScene);
		GameState::GetInstance().pGameScene = pScene;
	}
	return *pScene;
}

void SceneManager::ToggleScene(Scene& scene, bool isActive)
{
	for (Scene* pScene : m_pAllScenes)
	{
		if (*pScene == scene)
		{
			const auto it = std::find(m_pActiveScenes.begin(), m_pActiveScenes.end(), pScene);
			if (isActive)
			{
				if (it == m_pActiveScenes.end())
				{
					m_pActiveScenes.push_back(pScene);
					GameState::GetInstance().pGameScene = pScene;
				}
			}
			else
			{
				if (it != m_pActiveScenes.end())
				{
					m_pActiveScenes.erase(it);
					if (!m_pActiveScenes.empty())
						GameState::GetInstance().pGameScene = m_pActiveScenes.back();
					else
						GameState::GetInstance().pGameScene = nullptr;
				}
			}
			return;
		}
	}
}

void SceneManager::ToggleScene(const std::string& name, bool isActive)
{
	for (Scene* pScene : m_pAllScenes)
	{
		if (pScene->m_Name == name)
		{
			const auto it = std::find(m_pActiveScenes.begin(), m_pActiveScenes.end(), pScene);
			if (isActive)
			{
				if (it == m_pActiveScenes.end())
				{
					m_pActiveScenes.push_back(pScene);
					GameState::GetInstance().pGameScene = pScene;
				}
			}
			else
			{
				if (it != m_pActiveScenes.end())
				{
					m_pActiveScenes.erase(it);
					if (!m_pActiveScenes.empty())
						GameState::GetInstance().pGameScene = m_pActiveScenes.back();
					else
						GameState::GetInstance().pGameScene = nullptr;
				}
			}
			return;
		}
	}
}
