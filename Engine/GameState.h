#pragma once
#include "SingletonRef.h"
#include "Timer.h"
#include "GlobalInput.h"
#include "GlobalMemoryPools.h"
#include "Camera.h"

class Scene;
struct WindowInfo;
class GameState final : public SingletonRef<GameState> //placeholder name
{
public:
	~GameState() = default;

	GameState(const GameState& other) = delete;
	GameState(GameState&& other) = delete;
	GameState& operator=(const GameState& other) = delete;
	GameState& operator=(GameState&& other) = delete;

	float DeltaTime{};
	Scene* pGameScene{};
	Scene* pUIScene{}; //TODO: keep? vs SceneManager
	WindowInfo* pWindowInfo{};
	Timer<high_resolution_clock> Timer{};
	GlobalInput& GlobalInput{ GlobalInput::GetInstance() };
	GlobalMemoryPools& GlobalMemoryPools{ GlobalMemoryPools::GetInstance() };
	Camera Camera{};

private:
	friend static GameState& SingletonRef<GameState>::GetInstance();
	GameState() = default;
};