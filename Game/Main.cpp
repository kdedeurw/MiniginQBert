#include "pch.h"

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#include <vld.h>
#endif

#include "Math2D.h"
#include "Vectors.h"
#include "Collision2D.h"
#include "RGBAColour.h"

#include "Minigin.h"
//#include <SDL.h>
#include "SceneManager.h"
#include "ResourceManager.h"
#include "GlobalMemoryPools.h"
#include "GlobalInput.h"
#include "GameState.h"
#include "GameObject.h"
#include "Components.h"
#include "Transform.h"
#include "WindowInfo.h"

//project includes
#include "Subject.h"
#include "QBertGameObserver.h"
#include "QBertPlayer.h"
#include <iostream>

void QBertAssignment()
{
	GlobalMemoryPools& gm = GlobalMemoryPools::GetInstance();
	Scene& qBertScene = SceneManager::GetInstance().CreateScene("QBertScene");

	QBertGameObserver* pObserver = gm.CreateOnStack<QBertGameObserver>();
	//Player 1
	GameObject* pPlayer = qBertScene.CreateGameObject();
	pPlayer->GetLocalTransform().Translate(100.f, 350.f);

	TextComponent* pLivesDisplay = gm.CreateComponent<TextComponent>();
	pLivesDisplay->SetFont(ResourceManager::GetInstance().LoadFont("Lingua.otf", 24));
	pLivesDisplay->SetColour(RGBAColour{ 0, 0, 255, 255 });
	pLivesDisplay->GetOffset() = Vector2{ 0.f, 25.f };
	TextComponent* pScoreDisplay = gm.CreateComponent<TextComponent>();
	pScoreDisplay->SetFont(ResourceManager::GetInstance().LoadFont("Lingua.otf", 24));
	pScoreDisplay->SetColour(RGBAColour{ 0, 0, 255, 255 });
	pScoreDisplay->GetOffset() = Vector2{ 0.f, -25.f };

	pPlayer->AddComponent(pLivesDisplay);
	pPlayer->AddComponent(pScoreDisplay);

	QBertPlayer* pQBertPlayerComponent = gm.CreateComponent<QBertPlayer>();
	pQBertPlayerComponent->SetKillButton(Key::Space);
	pQBertPlayerComponent->SetName("QBert 1");
	pQBertPlayerComponent->SetRandomPointsButton(Key::One);
	pQBertPlayerComponent->SetLivesDisplay(pLivesDisplay);
	pQBertPlayerComponent->SetScoreDisplay(pScoreDisplay);

	Subject* pSubject = gm.CreateSubject(1);
	pSubject->AddObserver(pObserver);
	pQBertPlayerComponent->SetSubject(pSubject);

	pPlayer->AddComponent(pQBertPlayerComponent);

	//Player 2
	pPlayer = qBertScene.CreateGameObject();
	pPlayer->GetLocalTransform().Translate(100.f, 150.f);

	pLivesDisplay = gm.CreateComponent<TextComponent>();
	pLivesDisplay->SetFont(ResourceManager::GetInstance().LoadFont("Lingua.otf", 24));
	pLivesDisplay->SetColour(RGBAColour{ 255, 0, 0, 255 });
	pLivesDisplay->GetOffset() = Vector2{ 0.f, 25.f };
	pScoreDisplay = gm.CreateComponent<TextComponent>();
	pScoreDisplay->SetFont(ResourceManager::GetInstance().LoadFont("Lingua.otf", 24));
	pScoreDisplay->SetColour(RGBAColour{ 255, 0, 0, 255 });
	pScoreDisplay->GetOffset() = Vector2{ 0.f, -25.f };

	pPlayer->AddComponent(pLivesDisplay);
	pPlayer->AddComponent(pScoreDisplay);

	pQBertPlayerComponent = gm.CreateComponent<QBertPlayer>();
	pQBertPlayerComponent->SetKillButton(Key::Enter);
	pQBertPlayerComponent->SetName("QBert 2");
	pQBertPlayerComponent->SetRandomPointsButton(Key::Two);
	pQBertPlayerComponent->SetLivesDisplay(pLivesDisplay);
	pQBertPlayerComponent->SetScoreDisplay(pScoreDisplay);

	pSubject = gm.CreateSubject(1);
	pSubject->AddObserver(pObserver);
	pQBertPlayerComponent->SetSubject(pSubject);

	pPlayer->AddComponent(pQBertPlayerComponent);

	std::cout << "\nHow to play:\nPress Space to kill Player 1\nPress Enter to kill Player 2\nPress 1 for a random point event for Player 1\nPress 2 for a random point event for Player 2\n\n";
}

int main(int, char* [])
{
	srand((unsigned int)time(NULL));

	dae::Minigin game{};
	game.StartUp();
	game.AddDemoScene();
	game.AddFPSScene();

	//GlobalMemoryPools& gm = GlobalMemoryPools::GetInstance();
	//Scene& scene = SceneManager::GetInstance().CreateScene("QBertScene");
	//GameObject* pGo = scene.CreateGameObject();

	game.Run();

	return 0;
}