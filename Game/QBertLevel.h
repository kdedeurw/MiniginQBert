#pragma once
#include "Component.h"
#include <queue>

enum class GameEvent
{
	kill_enemy,
	kill_all_enemies,
	kill_all_enemies_non_coily,
	kill_player,
	kill_all_players,
	kill_disk,
};

class QBertTile;
class QBertCharacter;
class QBertPlayer;
class QBertGameObserver;
struct QBertLevelData;
enum class TileState : int;
enum class QBertCharacterType : int;
class QBertSpinningDisk;
class QBertLevel final : public Component
{
public:
	QBertLevel();
	~QBertLevel();

	void Initialize(bool forceInitialize = false) override;
	void Render() const override {};
	void PostRender() const override;
	void Update() override;

	void Reset();
	void QueueEvent(Component* pEntity, GameEvent event);
	void ResetAllTiles();
	void MoveOnTile(QBertCharacter* pCharacter, int tileId);

	void SetObserver(QBertGameObserver* pObserver) { m_pObserver = pObserver; }

	int GetAmountOfTiles() const { return m_AmountOfTiles; }
	int GetLowerRowSize() const { return m_LowerRowSize; }
	QBertTile* GetTile(int tileId) const;
	QBertSpinningDisk* GetDisk(int id);
	QBertTile* GetUpperTile() const { return m_pTiles[m_AmountOfTiles - 1]; }
	QBertTile* GetLastTowTile() const { return m_pTiles[m_LowerRowSize - 1]; }
	const std::vector<QBertPlayer*>& GetPlayers() const { return m_pPlayers; }

private:
	bool m_GameWin;
	short m_CurrRound, m_CurrLevel, m_CurrentTargetTiles;
	const short m_MaxRounds = 4;
	const short m_MaxLevels = 3;
	const int m_AmountOfTiles = 28;
	const int m_LowerRowSize = 7;
	unsigned int m_CurrentEnemyId;
	float m_CurrentEnemySpawnDelay;
	const float m_EnemySpawnDelay = 7.5f;
	float m_CurrentRoundWinDelay;
	const float m_RoundWinDelay = 5.f;
	QBertGameObserver* m_pObserver;
	const std::vector<QBertLevelData>& m_LevelDatas;
	std::vector<QBertPlayer*> m_pPlayers;
	std::vector<QBertTile*> m_pTiles;
	std::vector<QBertCharacter*> m_pEnemies;
	std::vector<QBertSpinningDisk*> m_pDisks;

	struct GameEventRequest
	{
		GameEvent Event;
		Component* pEntity;
	};
	std::queue<GameEventRequest> m_Events;

	//Handlers
	void HandleGameEventsQueue();
	void HandleEnemySpawning();
	void HandleGameOver();

	//Cleaners
	void RemoveEntity(Component* pEntity);
	void RemoveEnemy(QBertCharacter* pEnemy);
	void ClearAllEnemies();
	void ClearAllButCoily();
	void ClearAllPlayers();
	void RemoveDisk(QBertSpinningDisk* pDisk);

	//Fabricators
	GameObject* CreateTile();
	GameObject* CreatePlayer();
	GameObject* CreateEnemy(char type);
	GameObject* CreateSpinningDisk();

	//Helpers
	TileState EvaluateNextState(TileState state);
	TileState EvaluatePreviousState(TileState state);
	void CommenceNextRound();
	void AddDiskToLevel(bool isLeft);

	void OnRoundWin();
	bool OnLevelWin();
	void OnGameWin();
	void OnGameOver();

	//Tile Helpers
	void ConnectTiles();
	int GetLowerRowMin(int currRowSize) const;
	int GetLowerRowMax(int currRowSize) const;
	int GetUpperRowMin(int currRowSize) const;
	int GetUpperRowMax(int currRowSize) const;
};