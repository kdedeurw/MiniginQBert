#pragma once
#include "Component.h"
#include "QBertLevelReader.h"
#include "QBertTile.h"

class QBertGameObserver;
class QBertLevel final : public Component
{
public:
	QBertLevel();
	~QBertLevel();

	void Initialize() override;
	void Render() const override {};
	void PostRender() const override;
	void Update() override;

	void JumpOnTile(int tileId);
	QBertTile* GetTile(int tileId) const;
	QBertTile* GetUpperTile() const { return m_pTiles[m_MaxTiles - 1]; };
	QBertTile* GetLastTowTile() const { return m_pTiles[m_MaxTileRow - 1]; }

private:
	short m_Round, m_Level, m_TargetTiles;
	const int m_MaxTiles = 28;
	const int m_MaxTileRow = 7;
	QBertGameObserver* m_pObserver;
	std::vector<QBertTile*> m_pTiles;
	QBertLevelReader m_LevelReader;

	GameObject* CreateTile();
	GameObject* CreatePlayer();
	GameObject* CreateEnemy();
	GameObject* CreateSpinningDisk();

	TileState EvaluateNextState(TileState state);

	void ConnectTiles();
	int GetLowerRowMin(int currRowSize) const;
	int GetLowerRowMax(int currRowSize) const;
	int GetUpperRowMin(int currRowSize) const;
	int GetUpperRowMax(int currRowSize) const;
};