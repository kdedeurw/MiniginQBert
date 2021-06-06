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

void QBertScene()
{
	//GlobalMemoryPools& gm = GlobalMemoryPools::GetInstance();
	Scene& scene = SceneManager::GetInstance().CreateScene("QBertScene");
	GameObject* pGo = scene.CreateGameObject();

	QBertLevel* pLevel = new QBertLevel{};
	pGo->GetTransform().SetScale({2.f, 2.f});
	pGo->AddComponent(pLevel);
}

int main(int, char* [])
{
	Engine2D::Core application{};
	application.Init();
	//application.AddDemoScene();
	application.AddFPSScene();

	QBertScene();

	application.Run();

	return 0;
}