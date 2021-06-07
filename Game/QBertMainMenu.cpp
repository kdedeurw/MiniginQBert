#include "pch.h"
#include "QBertMainMenu.h"

QBertMainMenu::QBertMainMenu()
{
}

QBertMainMenu::~QBertMainMenu()
{
}

void QBertMainMenu::Initialize(bool forceInitialize)
{
	forceInitialize;
}

void QBertMainMenu::Render() const
{
}

void QBertMainMenu::Update()
{
	GameState& gs = GameState::GetInstance();

	if (gs.KeyboardMouseListener.IsPressed(Key::Space))
	{
		SceneManager::GetInstance().ToggleScene(GetGameObject()->GetScene(), false);
		SceneManager::GetInstance().ToggleScene("QBertScene", true);
	}
}