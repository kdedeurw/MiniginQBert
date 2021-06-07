#include "pch.h"

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#include <vld.h>
#endif

//_SDL_main
#include <SDL.h>

#include "Core.h"

//project includes
#include "Subject.h"
#include "QBertGameObserver.h"
#include "QBertPlayer.h"
#include "QBertLevel.h"
#include "QBertUI.h"
#include "QBertMainMenu.h"
#include "QbertGameOverMenu.h"

#include "CameraTester.h"
#include "SoundTester.h"

void SoundSystemAssignment(bool isDebugSound = false)
{
	//GlobalMemoryPools& gm = GlobalMemoryPools::GetInstance();
	Scene& soundScene = SceneManager::GetInstance().CreateScene("SoundScene");

	GameState& gs = GameState::GetInstance();
	const WindowInfo& wi = *gs.pWindowInfo;

	GameObject* pGo = soundScene.CreateGameObject();
	pGo->GetTransform().Translate(wi.Width / 2.f, wi.Height / 3.f);

	TextComponent* pTc = new TextComponent{};
	pTc->SetFont(ResourceManager::GetInstance().LoadFont("Lingua.otf", 24));
	pTc->SetColour(RGBAColour{ 255, 255, 255, 255 });
	//pTc->GetOffset() = Vector2{ 0.f, 25.f };
	pTc->SetText("Press 1 to play sound");
	pGo->AddComponent(pTc);

	SoundTester* pSt = new SoundTester{};
	pGo->AddComponent(pSt);
	
	pSt->SetDebugSoundInit(isDebugSound);
}

void CameraTestScene()
{
	//GlobalMemoryPools& gm = GlobalMemoryPools::GetInstance();
	Scene& scene = SceneManager::GetInstance().CreateScene("CameraTestScene");
	GameObject* pGo = scene.CreateGameObject();
	
	CameraTester* pCt = new CameraTester{};
	pGo->AddComponent(pCt);
}

void MenuScenes(QBertGameObserver* pObserver, QBertLevel* pLevel)
{
	Scene& mainMenuScene = SceneManager::GetInstance().CreateScene("MainMenuScene");
	GameObject* pGo = mainMenuScene.CreateGameObject();
	GameState& gs = GameState::GetInstance();

	//Main
	Texture2DComponent* pTexComp = new Texture2DComponent{};
	pTexComp->SetTexture("QBert/Menus.png");
	pTexComp->SetTextureSize({ 161.25f, 145.25f });
	pTexComp->SetTextureOffset({ 161.25f * 2.f, 145.25f });

	pGo->AddComponent(pTexComp);
	pGo->GetTransform().Translate(gs.pWindowInfo->Width / 2.f, gs.pWindowInfo->Height / 2.f);
	pGo->GetTransform().SetScale({3.f, 3.f});

	QBertMainMenu* pMainMenu = new QBertMainMenu{};
	pGo->AddComponent(pMainMenu);

	pGo = mainMenuScene.CreateGameObject();
	TextComponent* pTc = new TextComponent{};
	pTc->SetText("Press [Space] to start");
	pTc->SetColour(RGBAColour{0, 255, 0, 255});
	pGo->AddComponent(pTc);

	pTc = new TextComponent{};
	pTc->SetText("Q-E-A-D to move");
	pTc->SetColour(RGBAColour{ 0, 255, 0, 255 });
	pGo->AddComponent(pTc);
	pTc->SetOffset({ 0.f, -50.f });

	pGo->GetTransform().SetScale(2.5f, 2.5f);
	pGo->GetTransform().Translate(gs.pWindowInfo->Width / 2.f, gs.pWindowInfo->Height / 4.f);

	//GameOver
	Scene& gameOverScene = SceneManager::GetInstance().CreateScene("GameOverScene", false);
	pGo = gameOverScene.CreateGameObject();

	QBertGameOverMenu* pGameOverMenu = new QBertGameOverMenu{};
	pGo->AddComponent(pGameOverMenu);
	pGameOverMenu->SetObserver(pObserver);
	pGameOverMenu->SetLevel(pLevel);

	pTexComp = new Texture2DComponent{};
	pTexComp->SetTexture("QBert/Menus.png");
	pTexComp->SetTextureSize({ 161.25f, 145.25f });
	pTexComp->SetTextureOffset({ 161.25f * 2.f, 145.25f * 2.f });

	pGo->AddComponent(pTexComp);
	pGo->GetTransform().Translate(gs.pWindowInfo->Width / 2.f, gs.pWindowInfo->Height / 2.f);
	pGo->GetTransform().SetScale({ 3.f, 3.f });

	pGo = gameOverScene.CreateGameObject();
	pTc = new TextComponent{};
	pTc->SetText("Press [Enter] to restart");
	pTc->SetColour(RGBAColour{ 0, 0, 255, 255 });
	pTc->SetOffset({ -50.f, -60.f });
	pGo->AddComponent(pTc);
	pGo->GetTransform().Translate(gs.pWindowInfo->Width / 2.f, gs.pWindowInfo->Height / 2.f);
	pGo->GetTransform().SetScale({ 3.f, 3.f });

	QBertUI* pUI = new QBertUI{};
	pGo->AddComponent(pUI);
	pUI->SetObserver(pObserver);
	pTc = new TextComponent{};
	pTc->SetColour(RGBAColour{ 0, 255, 255, 255 });
	pUI->SetScoreText(pTc);
	pTc->SetOffset({ -75.f, -130.f });
	pGo->AddComponent(pTc);
	pGo->GetTransform().SetScale({ 2.f, 2.f });
}

void QBertScene(QBertGameObserver* pObserver, QBertLevel* pLevel)
{
	//GlobalMemoryPools& gm = GlobalMemoryPools::GetInstance();
	Scene& scene = SceneManager::GetInstance().CreateScene("QBertScene", false);
	GameObject* pGo = scene.CreateGameObject();

	pGo->GetTransform().SetScale({2.f, 2.f});
	pGo->AddComponent(pLevel);

	pGo = scene.CreateGameObject();
	QBertUI* pUI = new QBertUI{};
	pGo->AddComponent(pUI);

	pGo = scene.CreateGameObject();
	TextComponent* pTc = new TextComponent{};
	pUI->SetScoreText(pTc);
	pGo->AddComponent(pTc);
	pTc = new TextComponent{};
	pUI->SetLivesText(pTc);
	pGo->AddComponent(pTc);

	pUI->SetObserver(pObserver); //hand over ownership
	pLevel->SetObserver(pObserver);
}

int main(int, char* [])
{
	Engine2D::Core application{};
	application.Init();
	//application.AddDemoScene();
	application.AddFPSScene();

	QBertGameObserver* pObserver = new QBertGameObserver{};
	QBertLevel* pLevel = new QBertLevel{};
	try
	{
		MenuScenes(pObserver, pLevel);
		QBertScene(pObserver, pLevel);
	}
	catch (std::exception& e)
	{
		std::cout << "Exception thrown during User Scene Init: " << e.what() << '\n';
	}

	application.Run();

	return 0;
}