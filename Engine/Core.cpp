//Base includes
#include "pch.h"
#include "Core.h"
#include <chrono>
#include <thread>
#include "Transform.h"
#include "KeyboardMouseListener.h"
#include "ControllerListener.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Scene.h"

//SDL includes
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

//Project includes
#include "Timer.h"
#include "Components.h"
#include "GameState.h"
#include "ServiceLocator.h"
#include "SoundSystem.h"

using namespace Engine2D;

Core::Core(const char* pTitle, int w, int h, int msPF)
	: m_QuitKey{ Key::Escape }
	, m_WindowInfo{ pTitle, w, h, msPF }
	, m_pWindow{}
	, m_AudioThread{}
{
	//set random seed
	srand((unsigned int)time(NULL));

	// tell the resource manager where he can find the game data
	ResourceManager::GetInstance().Init("../Resources/");

	GameState::GetInstance().pWindowInfo = &m_WindowInfo;
}

Core::~Core()
{}

void Core::Init()
{
	InitializeSDL();

	ServiceLocator::RegisterSoundSystem(new SoundSystem{});
}

void Core::InitializeSDL()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	m_pWindow = SDL_CreateWindow(
		m_WindowInfo.pTitle,
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		m_WindowInfo.Width,
		m_WindowInfo.Height,
		SDL_WINDOW_OPENGL
	);
	if (!m_pWindow) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(m_pWindow);
}

void Core::Cleanup()
{
	SDL_DestroyWindow(m_pWindow);
	m_pWindow = nullptr;

	delete ServiceLocator::GetSoundSystem();

	SDL_Quit();
}

bool Core::ProcessInputs()
{
	KeyboardMouseListener& kbml = KeyboardMouseListener::GetInstance();
	ControllerListener& cl = ControllerListener::GetInstance();

	kbml.ProcessInput();
	cl.ProcessInput();

	//return kbml.IsQuit();
	return kbml.IsPressed(m_QuitKey);
}

void Core::Run()
{
	SceneManager& sm = SceneManager::GetInstance();
	Renderer& renderer = Renderer::GetInstance();
	GameState& gs = GameState::GetInstance();

	sm.Initialize();

	Timer<high_resolution_clock> timer{}; //start point automatically resets to Time::Now();
	while (!m_IsQuit)
	{
		gs.DeltaTime = timer.GetElapsedTimeMs<float>();
		timer.ResetStartTime();
		m_IsQuit = ProcessInputs();
		sm.Update();
		renderer.Render();
	}

	Cleanup();
}

void Core::ForceQuit()
{
	m_IsQuit = true;
}

void Core::AddFPSScene(float x, float y) const
{
	SceneManager& sm = SceneManager::GetInstance();
	GlobalMemoryPools& mp = GlobalMemoryPools::GetInstance();

	Scene& scene = sm.CreateUIScene();
	GameObject* pGo = scene.CreateGameObject();
	FPSComponent* pFPS = mp.CreateComponent<FPSComponent>();
	pGo->AddComponent(pFPS);
	pGo->GetTransform().SetPosition(x, y);
}

void Core::AddDemoScene() const
{
	Scene& scene = SceneManager::GetInstance().CreateScene("Demo");
	GlobalMemoryPools& gm = GlobalMemoryPools::GetInstance();
	GameState& gs = GameState::GetInstance();
	WindowInfo* pWi = gs.pWindowInfo;

	//create background
	GameObject* pGo = scene.CreateGameObject();
	Texture2DComponent* pTex = gm.CreateComponent<Texture2DComponent>();
	pTex->SetTexture("background.jpg");
	pGo->AddComponent(pTex);
	pGo->GetTransform().Translate(pWi->Width / 2.f, pWi->Height / 2.f);

	//create logo
	pGo = scene.CreateGameObject();
	pTex = gm.CreateComponent<Texture2DComponent>();
	pTex->SetTexture("logo.png");
	pGo->AddComponent(pTex);
	pGo->GetTransform().Translate(pWi->Width / 2.f, pWi->Height / 2.f);

	//create text
	pGo = scene.CreateGameObject();
	Font* pFont = ResourceManager::GetInstance().LoadFont("Lingua.otf", 36);
	TextComponent* pTc = gm.CreateComponent<TextComponent>();
	pTc->SetText("Programming 4 Assignment");
	pTc->SetFont(pFont);
	pGo->AddComponent(pTc);

	pTc->Initialize(); //create texture data
	int w{}, h{};
	SDL_QueryTexture(pTc->GetTextureData()->GetSDLTexture(), nullptr, nullptr, &w, &h);

	pGo->GetTransform().Translate(pWi->Width / 2.f, (float)pWi->Height - h);

	pGo = scene.CreateGameObject();
	pTc = gm.CreateComponent<TextComponent>();
	pTc->SetText("Press Esc to exit");
	pTc->SetFont(pFont);
	pGo->AddComponent(pTc);

	pTc->Initialize(); //create texture data
	SDL_QueryTexture(pTc->GetTextureData()->GetSDLTexture(), nullptr, nullptr, &w, &h);

	pGo->GetTransform().Translate(w / 2.f, h / 2.f);
}

void Core::SetForceQuitKey(Key key)
{
	m_QuitKey = key;
}

void Engine2D::Core::SetRandSeed(unsigned int seed)
{
	srand(seed);
}

/*
//former test code from unfinished project I made in own implementation of Minigin Engine
void RTSGame::Archive()
{
	GameObject* pParent = GetParentChildTest();

	Scene& scene = m_SceneManager.CreateScene("InputTestScene");
	TextObject* pInputTo = m_GlobalMemoryPools.CreateTextObject("", m_ResourceManager.LoadFont("Lingua.otf", 36));
	pInputTo->GetTransform().SetPosition(320, 280);
	pInputTo->SetText("No input");
	scene.AddObject(pInputTo);
	TextObject* pTo = m_GlobalMemoryPools.CreateTextObject("", m_ResourceManager.LoadFont("Lingua.otf", 36));
	pTo->SetText("MouseButtons to change height");
	pTo->GetTransform().SetPosition(0.f, 50.f);
	scene.AddObject(pTo);
	pTo = m_GlobalMemoryPools.CreateTextObject("", m_ResourceManager.LoadFont("Lingua.otf", 36));
	pTo->SetText("Scrollwheel to change width");
	pTo->GetTransform().SetPosition(0.f, 100.f);
	scene.AddObject(pTo);
	pTo = m_GlobalMemoryPools.CreateTextObject("", m_ResourceManager.LoadFont("Lingua.otf", 36));
	pTo->SetText("WASD to move");
	pTo->GetTransform().SetPosition(0.f, 150.f);
	scene.AddObject(pTo);
	GameObject* pScaleTest = m_GlobalMemoryPools.CreateGameObject();
	Texture2DComponent* pTex = m_GlobalMemoryPools.CreateComponent<Texture2DComponent>();
	pTex->SetTexture("logo.png");
	pScaleTest->GetTransform().SetPosition(320, 200);
	pScaleTest->AddComponent(pTex);
	scene.AddObject(pScaleTest);

	Vector4& dest{ pTex->GetDestRect() };
	//Vector4& src{ pTex->GetSourceRect() };
	Vector2& scale{ pScaleTest->GetTransform().GetScale() };
	float& rot{ pScaleTest->GetTransform().GetRotation() };

	{
		float lag{};
		Timer<high_resolution_clock> timer{}; //start point automatically resets to Time::Now();
		while (!m_IsQuit)
		{
			//forced sleeptime
			//TODO: fix this goddamn time issue
			std::this_thread::sleep_for(16ms);
			//process all input
			m_GlobalInput.KeyboardMouseListener.ProcessInput();
			m_GlobalInput.ControllerListener.ProcessInput();
			m_IsQuit = m_GlobalInput.KeyboardMouseListener.IsQuit();
			//force quit
			m_IsQuit = m_GlobalInput.KeyboardMouseListener.IsPressed(Key::Escape);
			//get Time::Now()
			const auto currentTime = high_resolution_clock::now();
			//set global deltatime
			m_GameState.DeltaTime = timer.GetElapsedTime<float, milliseconds>();

			InputTest(pInputTo);

			pScaleTest->GetTransform().SetScale(scale);
			pScaleTest->GetTransform().SetRotation(rot);
			Texture2DComponent* pTest = pScaleTest->GetComponent<Texture2DComponent>();

			DestRectTest(dest);

			ScaleTest(scale);

			rot += 1.f;

			//pTest->SetSourceRect(src);
			pTest->SetDestRect(dest);

			ParentChildTest(pParent);

			lag += m_GameState.DeltaTime;
			while (lag >= m_WindowInfo.MsPerFrame)
			{
				m_GameState.DeltaTime = m_WindowInfo.MsPerFrame / 1000.f;
				m_SceneManager.Update();
				lag -= m_WindowInfo.MsPerFrame;
			}
			m_Renderer.Render();
			timer.ResetStartTime();

			const auto sleepTime = duration_cast<duration<float>>(currentTime + milliseconds(m_WindowInfo.MsPerFrame) - high_resolution_clock::now());
			std::this_thread::sleep_for(sleepTime);
		}
	}
}

void RTSGame::InputTest(TextObject* pTo)
{
	pTo->SetText("No input");
	if (m_GlobalInput.KeyboardMouseListener.IsPressed(Key::W))
	{
		pTo->SetText("W");
		if (m_GlobalInput.KeyboardMouseListener.IsPressed(Key::A))
			pTo->SetText("W + A");
		else if (m_GlobalInput.KeyboardMouseListener.IsPressed(Key::D))
			pTo->SetText("W + D");
		pTo->GetTransform().SetPosition(pTo->GetTransform().GetPosition() + Vector3{ 0.f, 1.f });
	}
	else if (m_GlobalInput.KeyboardMouseListener.IsPressed(Key::S))
	{
		pTo->SetText("S");
		if (m_GlobalInput.KeyboardMouseListener.IsPressed(Key::A))
			pTo->SetText("S + A");
		else if (m_GlobalInput.KeyboardMouseListener.IsPressed(Key::D))
			pTo->SetText("S + D");
		pTo->GetTransform().SetPosition(pTo->GetTransform().GetPosition() - Vector3{ 0.f, 1.f });
	}
	if (m_GlobalInput.KeyboardMouseListener.IsPressed(Key::A))
	{
		pTo->SetText("A");
		if (m_GlobalInput.KeyboardMouseListener.IsPressed(Key::W))
			pTo->SetText("W + A");
		else if (m_GlobalInput.KeyboardMouseListener.IsPressed(Key::S))
			pTo->SetText("S + A");
		pTo->GetTransform().SetPosition(pTo->GetTransform().GetPosition() - Vector3{ 1.f, 0.f });
	}
	else if (m_GlobalInput.KeyboardMouseListener.IsPressed(Key::D))
	{
		pTo->SetText("D");
		if (m_GlobalInput.KeyboardMouseListener.IsPressed(Key::W))
			pTo->SetText("W + D");
		else if (m_GlobalInput.KeyboardMouseListener.IsPressed(Key::S))
			pTo->SetText("S + D");
		pTo->GetTransform().SetPosition(pTo->GetTransform().GetPosition() + Vector3{ 1.f, 0.f });
	}
}

void RTSGame::ScaleTest(Vector2& scale)
{
	if (m_GlobalInput.KeyboardMouseListener.IsScrollWheelMove(ScrollWheel::Up))
		scale.x += 0.1f;
	else if (m_GlobalInput.KeyboardMouseListener.IsScrollWheelMove(ScrollWheel::Down))
		scale.x -= 0.1f;

	if (m_GlobalInput.KeyboardMouseListener.IsMouseButtonPressed(Mouse::LMB))
		scale.y += 0.1f;
	else if (m_GlobalInput.KeyboardMouseListener.IsMouseButtonPressed(Mouse::RMB))
		scale.y -= 0.1f;
}

GameObject* RTSGame::GetParentChildTest()
{
	Scene& scene = m_SceneManager.CreateScene("ParentChild");
	GameObject* pParent = m_GlobalMemoryPools.CreateGameObject();
	Texture2DComponent* pTex = m_GlobalMemoryPools.CreateComponent<Texture2DComponent>();
	pTex->SetTexture("logo.png");
	pParent->AddComponent(pTex);
	pParent->GetTransform().SetPosition(100.f, 100.f);
	GameObject* pChild = m_GlobalMemoryPools.CreateGameObject();
	pTex = m_GlobalMemoryPools.CreateComponent<Texture2DComponent>();
	pTex->SetTexture("logo.png");
	pChild->AddComponent(pTex);
	pChild->GetTransform().SetPosition(100.f, 100.f);
	GameObject* pPivot = m_GlobalMemoryPools.CreateGameObject();
	pTex = m_GlobalMemoryPools.CreateComponent<Texture2DComponent>();
	pTex->SetTexture("logo.png");
	pPivot->AddComponent(pTex);
	pPivot->GetTransform().SetPosition(100.f, 100.f);
	pPivot->AddChildObject(pChild);
	pParent->AddChildObject(pPivot);
	//pParent->AddChildObject(pChild);
	pPivot->GetTransform().SetScale(0.1f, 0.1f);
	pParent->GetTransform().SetRotation(90.f);
	pParent->GetTransform().SetScale(1.5f, 0.5f);
	pChild->GetTransform().SetScale(0.5f, 1.5f);
	pChild->GetTransform().SetRotation(180.f);
	scene.AddObject(pParent);
	return pParent;
}

void RTSGame::ParentChildTest(GameObject* pParent)
{
	pParent->GetTransform().SetPosition(m_GlobalInput.KeyboardMouseListener.GetMousePos() + Vector2{ 100.f, 0.f });
	pParent->GetTransform().Rotate(1.f); //rotate parent
	pParent->GetChildren().front()->GetTransform().Rotate(-2.f); //rotate pivot (child of parent)
	pParent->GetChildren().front()->GetChildren().front()->GetTransform().Rotate(3.f); //rotate child (child of child)
}

void RTSGame::DestRectTest(Vector4& dest)
{
	if (m_GlobalInput.KeyboardMouseListener.IsPressed(Key::A))
		dest.x -= 1.f;
	else if (m_GlobalInput.KeyboardMouseListener.IsPressed(Key::D))
		dest.x += 1.f;
	if (m_GlobalInput.KeyboardMouseListener.IsPressed(Key::W))
		dest.y += 1.f;
	else if (m_GlobalInput.KeyboardMouseListener.IsPressed(Key::S))
		dest.y -= 1.f;

	if (m_GlobalInput.KeyboardMouseListener.IsPressed(Key::ArrowLeft))
		dest.z -= 1.f;
	else if (m_GlobalInput.KeyboardMouseListener.IsPressed(Key::ArrowRight))
		dest.z += 1.f;
	if (m_GlobalInput.KeyboardMouseListener.IsPressed(Key::ArrowUp))
		dest.w += 1.f;
	else if (m_GlobalInput.KeyboardMouseListener.IsPressed(Key::ArrowDown))
		dest.w -= 1.f;
}
*/