#include "pch.h"
#include "QBertLevel.h"
#include "GlobalMemoryPools.h"
#include "Components.h"
#include "GameState.h"
#include "QBertTile.h"
#include "QBertPlayer.h"

QBertLevel::QBertLevel()
	: m_Round{}
	, m_Level{}
	, m_LevelReader{ "Resources/QBertLevels.txt" }
	, m_pTiles{}
{
	m_pTiles.reserve(28);
}

QBertLevel::~QBertLevel()
{
}

void QBertLevel::Initialize()
{
	//TODO: read level from file
	//const QBertLevelData& level = m_LevelReader.ReadNextLevel();

	//TODO: link tiles together

	const Vector2 scale{ 2.f, 2.f };
	int delimiter = m_TileRow;
	const float tileSize = QBertTile::GetTextureSize();
	float totalX{ tileSize };
	float totalY{};

	for (int j{}; j < m_TileRow; ++j)
	{
		for (int i{}; i < delimiter; ++i)
		{
			GameObject* pTile = CreateTile();
			pTile->GetTransform().SetPosition((tileSize * i + totalX) * scale.x, (tileSize + tileSize * j + totalY) * scale.y);
			pTile->GetTransform().SetScale(scale);

			GetGameObject()->AddChildObject(pTile);
			m_pTiles.push_back(pTile->GetComponent<QBertTile>());
		}
		totalY -= tileSize / 4;
		totalX += tileSize / 2;
		--delimiter;
	}

	ConnectTiles();

	const float playerSize = QBertPlayer::GetTextureSize();
	GameObject* pPlayer = CreatePlayer();
	pPlayer->GetTransform().SetPosition((tileSize * 3 + tileSize / 2 + playerSize / 2) * scale.x, (tileSize * 6 + playerSize / 2) * scale.y);
	pPlayer->GetTransform().SetScale(scale);
	GetGameObject()->AddChildObject(pPlayer);
}

void QBertLevel::Render() const
{
}

void QBertLevel::Update()
{
	//TODO: update tiles/store player's and enemies' pos
	m_TestTimer -= GameState::GetInstance().DeltaTime;
	if (m_TestTimer <= 0.f)
	{
		for (QBertTile* pTile : m_pTiles)
		{
			pTile->UpdateState(m_CurrentState);
		}

		int nextState = ((int)m_CurrentState + 1);
		if (nextState > 2)
			nextState = 0;
		m_CurrentState = (State)nextState;
		m_TestTimer = 1.f;
	}
}

GameObject* QBertLevel::CreateTile()
{
	GlobalMemoryPools& gm = GlobalMemoryPools::GetInstance();
	GameObject* pGo = GetGameObject()->GetScene().CreateGameObject();

	Texture2DComponent* pTexture = gm.CreateComponent<Texture2DComponent>();
	pGo->AddComponent(pTexture);

	QBertTile* pTile = gm.CreateComponent<QBertTile>();
	pGo->AddComponent(pTile);

	pGo->Initialize();

	return pGo;
}

GameObject* QBertLevel::CreatePlayer()
{
	GlobalMemoryPools& gm = GlobalMemoryPools::GetInstance();
	GameObject* pGo = GetGameObject()->GetScene().CreateGameObject();

	Texture2DComponent* pTexture = gm.CreateComponent<Texture2DComponent>();
	pGo->AddComponent(pTexture);

	QBertPlayer* pPlayer = gm.CreateComponent<QBertPlayer>();
	pGo->AddComponent(pPlayer);

	pGo->Initialize();

	return pGo;
}

void QBertLevel::ConnectTiles()
{
	//algorithm works only if tiles are connected from left bottom to right top:
	//  6
	// 4 5
	//1 2 3
	//LeftTop = i + row - 1
	//RightTop = i + row
	//RightBottom = i - currRowSize
	//LeftBottom = i - currRowSize - 1

	//int currRowSize = m_TileRow;
	//int relativeTile = 0;
	//for (int currTileIdx{}; currTileIdx < m_MaxTiles; ++currTileIdx)
	//{
	//	 QBertTile* pTile = m_pTiles[currTileIdx];
	//
	//	 const int lowerRowMin = GetLowerRow(currRowSize);
	//	 const int lowerRowMax = GetLowerRow(currRowSize);
	//	 const int upperRowMin = GetUpperRow(currRowSize);
	//	 const int upperRowMax = GetUpperRow(currRowSize);
	//
	//	 int leftTopIdx = currTileIdx + currRowSize - 1;
	//	 if (leftTopIdx < m_MaxTiles && leftTopIdx > upperRow)
	//	 {
	//		 pTile->m_pNeighbours[0] = m_pTiles[leftTopIdx];
	//	 }
	//
	//	 int rightTopIdx = currTileIdx + currRowSize;
	//	 if (rightTopIdx < m_MaxTiles && rightTopIdx > upperRow)
	//	 {
	//		 pTile->m_pNeighbours[1] = m_pTiles[rightTopIdx];
	//	 }
	//
	//	 int rightBottomIdx = currTileIdx - currRowSize;
	//	 if (rightBottomIdx >= 0 && rightBottomIdx < lowerRow)
	//	 {
	//		 pTile->m_pNeighbours[2] = m_pTiles[rightBottomIdx];
	//	 }
	//
	//	 int leftBottomIdx = currTileIdx - currRowSize - 1;
	//	 if (leftBottomIdx >= 0 && leftBottomIdx < lowerRow)
	//	 {
	//		 pTile->m_pNeighbours[3] = m_pTiles[leftBottomIdx];
	//	 }
	//
	//	 //currRowSize = 7 > 6 > 5 > 4 > 3 > 2 > 1
	//	 ++relativeTile;
	//	 if (relativeTile >= currRowSize)
	//	 {
	//		 --currRowSize;
	//		 relativeTile = 0;
	//	 }
	//}
}

int QBertLevel::GetLowerRow(int currRowSize) const
{
	int total{};
	int rowSize = m_TileRow;

	while (rowSize > currRowSize)
	{
		total += rowSize;
		--rowSize;
	}
	return total;
}

int QBertLevel::GetUpperRow(int currRowSize) const
{
	int total{};
	int rowSize = m_TileRow;

	while (rowSize >= currRowSize)
	{
		total += rowSize;
		--rowSize;
	}
	--total;
	return total;
}