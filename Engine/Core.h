#pragma once
#include "WindowInfo.h"
#include <thread>

struct SDL_Window;
enum class Key;
namespace Engine2D
{
	class Core final
	{
	public:
		Core(const char* pTitle = "2DAE01 Dedeurwaerder Kristof", int w = 640, int h = 480, int msPF = 16);
		~Core();

		void Init();
		void Run();
		void AddFPSScene(float x = 0.f, float y = 480.f) const;
		void AddDemoScene() const;
		void ForceQuit();
		void SetForceQuitKey(Key key);
		void SetRandSeed(unsigned int seed);

	private:
		bool m_IsQuit = false;
		Key m_QuitKey;
		SDL_Window* m_pWindow = nullptr;
		WindowInfo m_WindowInfo;
		std::thread m_AudioThread;

		void InitializeSDL();
		void Cleanup();

		bool ProcessInputs();
	};
}