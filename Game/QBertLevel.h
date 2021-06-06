#pragma once
#include "Component.h"
#include <queue>

enum class GameEvent
{
	kill_enemy,
	kill_all_enemies,
	kill_player,
	kill_all_players,
};

class QBertTile;
class QBertCharacter;
class QBertPlayer;
class QBertGameObserver;
struct QBertLevelData;
enum class TileState : int;
enum class QBertCharacterType : int;
class QBertLevel final : public Component
{
public:
	QBertLevel();
	~QBertLevel();

	void Initialize(bool forceInitialize = false) override;
	void Render() const override {};
	void PostRender() const override;
	void Update() override;

	void QueueEvent(QBertCharacter* pCharacter, GameEvent event);
	void ResetAllTiles();
	void MoveOnTile(QBertCharacter* pCharacter, int tileId);

	int GetAmountOfTiles() const { return m_AmountOfTiles; }
	int GetLowerRowSize() const { return m_LowerRowSize; }
	QBertTile* GetTile(int tileId) const;
	QBertTile* GetUpperTile() const { return m_pTiles[m_AmountOfTiles - 1]; }
	QBertTile* GetLastTowTile() const { return m_pTiles[m_LowerRowSize - 1]; }
	const std::vector<QBertPlayer*>& GetPlayers() const { return m_pPlayers; }

private:
	short m_CurrRound, m_CurrLevel, m_CurrentTargetTiles;
	const short m_MaxRounds = 4;
	const short m_MaxLevels = 3;
	const int m_AmountOfTiles = 28;
	const int m_LowerRowSize = 7;
	unsigned int m_CurrentEnemyId;
	float m_CurrentEnemySpawnDelay;
	const float m_EnemySpawnDelay = 1.f;
	QBertGameObserver* m_pObserver;
	const std::vector<QBertLevelData>& m_LevelDatas;
	std::vector<QBertPlayer*> m_pPlayers;
	std::vector<QBertTile*> m_pTiles;
	std::vector<QBertCharacter*> m_pEnemies;

	struct GameEventRequest
	{
		GameEvent Event;
		QBertCharacter* pCharacter;
	};
	std::queue<GameEventRequest> m_Events;

	//Handlers
	void HandleGameEvents();
	void HandleEnemySpawning();

	//Cleaners
	void RemoveEnemy(QBertCharacter* pEnemy);
	void ClearAllEnemies();
	void ClearAllPlayers();

	//Fabricators
	GameObject* CreateTile();
	GameObject* CreatePlayer();
	GameObject* CreateEnemy(char type);
	GameObject* CreateSpinningDisk();

	//Helpers
	TileState EvaluateNextState(TileState state);
	TileState EvaluatePreviousState(TileState state);
	void CommenceNextRound();

	void OnRoundWin();
	bool OnLevelWin();
	void OnGameWin();

	//Tile Helpers
	void ConnectTiles();
	int GetLowerRowMin(int currRowSize) const;
	int GetLowerRowMax(int currRowSize) const;
	int GetUpperRowMin(int currRowSize) const;
	int GetUpperRowMax(int currRowSize) const;
};