#include "pch.h"
#include "QBertGameOverMenu.h"
#include "QBertGameObserver.h"
#include "QBertLevel.h"

QBertGameOverMenu::QBertGameOverMenu()
	: m_pObserver{}
	, m_pLevel{}
{
}

QBertGameOverMenu::~QBertGameOverMenu()
{
	m_pObserver = nullptr;
	m_pLevel = nullptr;
}

void QBertGameOverMenu::Initialize(bool forceInitialize)
{
	forceInitialize;
}

void QBertGameOverMenu::Render() const
{
}

void QBertGameOverMenu::Update()
{
	GameState& gs = GameState::GetInstance();

	if (gs.KeyboardMouseListener.IsPressed(Key::Enter))
	{
		SceneManager::GetInstance().ToggleScene(GetGameObject()->GetScene(), false);
		SceneManager::GetInstance().ToggleScene("MainMenuScene", true);
		m_pObserver->Reset();
		m_pLevel->Reset();
	}
}