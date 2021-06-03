#pragma once
#include "Component.h"
#include <vector>
#include "QBertLevelReader.h"
#include "QBertTile.h"

class QBertLevel final : public Component
{
public:
	QBertLevel();
	~QBertLevel();

	void Initialize() override;
	void Render() const override;
	void Update() override;

private:
	short m_Round, m_Level;
	const int m_MaxTiles = 28;
	const int m_TileRow = 7;
	std::vector<QBertTile*> m_pTiles;
	QBertLevelReader m_LevelReader;

	State m_CurrentState = State::DefaultState;
	float m_TestTimer = 1.f;

	GameObject* CreateTile();
	GameObject* CreatePlayer();

	void ConnectTiles();
	int GetLowerRow(int currRowSize) const;
	int GetUpperRow(int currRowSize) const;
};