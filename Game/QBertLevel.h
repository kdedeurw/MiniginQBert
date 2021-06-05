#pragma once
#include "Component.h"

class QBertTile;
class QBertCharacter;
class QBertPlayer;
class QBertGameObserver;
struct QBertLevelData;
enum class TileState : int;
enum class QBertCharacterType : int;
class QBertLevel final : public Component
{
	enum class TileAlteration
	{
		None,
		Next,
		Previous,
	};
public:
	QBertLevel();
	~QBertLevel();

	void Initialize() override;
	void Render() const override {};
	void PostRender() const override;
	void Update() override;

	void ResetTiles();
	void RemoveEnemy(QBertCharacter* pEnemy);
	void ClearAllEnemies();

	void MoveOnTile(QBertCharacter* pCharacter, int tileId);
	QBertTile* GetTile(int tileId) const;
	QBertTile* GetUpperTile() const { return m_pTiles[m_AmountOfTiles - 1]; };
	QBertTile* GetLastTowTile() const { return m_pTiles[m_LowerRowSize - 1]; }

private:
	short m_CurrRound, m_CurrLevel, m_CurrentTargetTiles;
	const short m_MaxRounds = 4;
	const short m_MaxLevels = 3;
	const int m_AmountOfTiles = 3;
	const int m_LowerRowSize = 2;
	unsigned int m_CurrentEnemyId;
	float m_CurrentEnemySpawnDelay;
	const float m_EnemySpawnDelay = 3.f;
	QBertGameObserver* m_pObserver;
	QBertPlayer* m_pPlayer;
	std::vector<QBertTile*> m_pTiles;
	std::vector<QBertCharacter*> m_pEnemies;
	const std::vector<QBertLevelData>& m_LevelDatas;

	GameObject* CreateTile();
	GameObject* CreatePlayer();
	GameObject* CreateEnemy(char type);
	GameObject* CreateSpinningDisk();

	TileAlteration GetCharacterTileAlteration(QBertCharacterType type);

	void HandleEnemySpawning();
	TileState EvaluateNextState(TileState state);
	TileState EvaluatePreviousState(TileState state);
	void CommenceNextRound();

	void OnRoundWin();
	bool OnLevelWin();
	void OnGameWin();

	void ConnectTiles();
	int GetLowerRowMin(int currRowSize) const;
	int GetLowerRowMax(int currRowSize) const;
	int GetUpperRowMin(int currRowSize) const;
	int GetUpperRowMax(int currRowSize) const;
};