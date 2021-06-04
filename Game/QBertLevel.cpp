#include "pch.h"
#include "QBertLevel.h"
#include "QBertTile.h"
#include "QBertPlayer.h"
#include "QBertGameObserver.h"

QBertLevel::QBertLevel()
	: m_Round{}
	, m_Level{}
	, m_TargetTiles{}
	, m_pObserver{}
	, m_LevelReader{}
	, m_pTiles{}
{
	m_pTiles.reserve(28);
}

QBertLevel::~QBertLevel()
{
}

void QBertLevel::Initialize()
{
	//TODO: fix
	QBertEntity::m_pLevel = this;

	const std::vector<QBertLevelData>& levels = m_LevelReader.ReadLevelDatas("../Resources/QBertLevels.json");
	levels;

	m_pObserver = GlobalMemoryPools::GetInstance().CreateOnStack<QBertGameObserver>();

	const Vector2& scale = m_pGameObject->GetTransform().GetWorld().Scale;
	int delimiter = m_MaxTileRow;
	const float tileSize = QBertTile::GetTextureSize();
	float totalX{ tileSize };
	float totalY{};

	for (int j{}; j < m_MaxTileRow; ++j)
	{
		for (int i{}; i < delimiter; ++i)
		{
			GameObject* pTile = CreateTile();
			pTile->GetTransform().SetPosition((tileSize * i + totalX) * scale.x, (tileSize + tileSize * j + totalY) * scale.y);

			GetGameObject()->AddChildObject(pTile);
			pTile->Initialize();
		}
		totalY -= tileSize / 4;
		totalX += tileSize / 2;
		--delimiter;
	}

	ConnectTiles();

	const float playerSize = QBertPlayer::GetTextureSize();
	GameObject* pPlayer = CreatePlayer();
	pPlayer->GetTransform().SetPosition(GetUpperTile()->GetGameObject()->GetTransform().GetWorld().Position);
	GetGameObject()->AddChildObject(pPlayer);
	pPlayer->Initialize();
}

void QBertLevel::PostRender() const
{
	for (QBertTile* pTile : m_pTiles)
	{
		const Vector2& scale = pTile->m_pGameObject->GetTransform().GetWorld().Scale;
		const QBertTile::Neighbours& neighBours = pTile->GetNeighbours();
		if (neighBours.pLeftTopNeighbour)
		{
			Renderer::GetInstance().DrawLine(pTile->m_pGameObject->GetTransform().GetWorld().Position,
				neighBours.pLeftTopNeighbour->m_pGameObject->GetTransform().GetWorld().Position, RGBAColour{ 255, 0, 0, 128 });
		}
		if (neighBours.pRightTopNeighbour)
		{
			Renderer::GetInstance().DrawLine(pTile->m_pGameObject->GetTransform().GetWorld().Position,
				neighBours.pRightTopNeighbour->m_pGameObject->GetTransform().GetWorld().Position, RGBAColour{ 255, 0, 0, 128 });
		}
		if (neighBours.pRightBottomNeighbour)
		{
			Renderer::GetInstance().DrawLine(pTile->m_pGameObject->GetTransform().GetWorld().Position,
				neighBours.pRightBottomNeighbour->m_pGameObject->GetTransform().GetWorld().Position, RGBAColour{ 255, 0, 0, 128 });
		}
		if (neighBours.pLeftBottomNeighbour)
		{
			Renderer::GetInstance().DrawLine(pTile->m_pGameObject->GetTransform().GetWorld().Position,
				neighBours.pLeftBottomNeighbour->m_pGameObject->GetTransform().GetWorld().Position, RGBAColour{ 255, 0, 0, 128 });
		}

		Font* pFont = ResourceManager::GetInstance().LoadFont("Lingua.otf", 24);
		const Vector2& pos = pTile->GetGameObject()->GetTransform().GetWorld().Position;
		Renderer::GetInstance().RenderTextImmediate(std::to_string(pTile->GetTileId()), pFont, pos.x, pos.y, 1.f, 1.f, RGBAColour{ 255, 255, 255, 128 });

		Renderer::GetInstance().DrawPoint(pTile->m_pGameObject->GetTransform().GetWorld().Position, scale.x, RGBAColour{ 0, 255, 0, 128 });
	}
}

void QBertLevel::Update()
{
}

void QBertLevel::JumpOnTile(int tileId)
{
	tileId = Math2D::Clamp(tileId, 0, m_MaxTiles);

	QBertTile* pTile = m_pTiles[tileId];

	TileState state = pTile->GetState();
	state = EvaluateNextState(state);
	pTile->UpdateState(state);
}

QBertTile* QBertLevel::GetTile(int tileId) const
{
	tileId = Math2D::Clamp(tileId, 0, m_MaxTiles);
	return m_pTiles[tileId];
}

TileState QBertLevel::EvaluateNextState(TileState state)
{
	//TODO: level difficulty

	switch (state)
	{
	case TileState::DefaultState:
		return TileState::IntermediateState;
	case TileState::IntermediateState:
		++m_TargetTiles;
		return TileState::TargetState;
	case TileState::TargetState:
		--m_TargetTiles;
		return TileState::IntermediateState;
	}
	return TileState::DefaultState;
}

GameObject* QBertLevel::CreateTile()
{
	GlobalMemoryPools& gm = GlobalMemoryPools::GetInstance();
	GameObject* pGo = GetGameObject()->GetScene().CreateGameObject();

	Texture2DComponent* pTexture = gm.CreateComponent<Texture2DComponent>();
	pGo->AddComponent(pTexture);

	QBertTile* pTile = gm.CreateComponent<QBertTile>();
	pGo->AddComponent(pTile);

	pTile->GetSubject()->AddObserver(m_pObserver);

	pTile->m_Id = static_cast<short>(m_pTiles.size());

	m_pTiles.push_back(pTile);

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
	
	pPlayer->GetSubject()->AddObserver(m_pObserver);

	return pGo;
}

GameObject* QBertLevel::CreateEnemy()
{
	return nullptr;
}

GameObject* QBertLevel::CreateSpinningDisk()
{
	return nullptr;
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

	int currRowSize = m_MaxTileRow;
	int relativeTile = 0;
	for (int currTileIdx{}; currTileIdx < m_MaxTiles; ++currTileIdx)
	{
		QBertTile* pTile = m_pTiles[currTileIdx];

		const int lowerRowMin = GetLowerRowMin(currRowSize);
		const int lowerRowMax = GetLowerRowMax(currRowSize);
		const int upperRowMin = GetUpperRowMin(currRowSize);
		const int upperRowMax = GetUpperRowMax(currRowSize);

		int leftTopIdx = currTileIdx + currRowSize - 1;
		if (leftTopIdx >= upperRowMin && leftTopIdx <= upperRowMax)
			pTile->m_Neighbours.pLeftTopNeighbour = m_pTiles[leftTopIdx];

		int rightTopIdx = currTileIdx + currRowSize;
		if (rightTopIdx >= upperRowMin && rightTopIdx <= upperRowMax)
			pTile->m_Neighbours.pRightTopNeighbour = m_pTiles[rightTopIdx];

		int rightBottomIdx = currTileIdx - currRowSize;
		if (rightBottomIdx >= lowerRowMin && rightBottomIdx <= lowerRowMax)
			pTile->m_Neighbours.pRightBottomNeighbour = m_pTiles[rightBottomIdx];

		int leftBottomIdx = currTileIdx - currRowSize - 1;
		if (leftBottomIdx >= lowerRowMin && leftBottomIdx <= lowerRowMax)
			pTile->m_Neighbours.pLeftBottomNeighbour = m_pTiles[leftBottomIdx];

		//currRowSize = 7 > 6 > 5 > 4 > 3 > 2 > 1
		++relativeTile;
		if (relativeTile >= currRowSize)
		{
			--currRowSize;
			relativeTile = 0;
		}
	}
}

int QBertLevel::GetLowerRowMin(int currRowSize) const
{
	int total{};
	int rowSize = m_MaxTileRow;

	currRowSize = Math2D::Clamp(currRowSize + 1, 0, m_MaxTileRow);
	while (rowSize > currRowSize)
	{
		total += rowSize;
		--rowSize;
	}
	return total;
}

int QBertLevel::GetLowerRowMax(int currRowSize) const
{
	int total{};
	int rowSize = m_MaxTileRow;

	while (rowSize > currRowSize)
	{
		total += rowSize;
		--rowSize;
	}
	--total;
	return total;
}

int QBertLevel::GetUpperRowMin(int currRowSize) const
{
	int total{};
	int rowSize = m_MaxTileRow;

	while (rowSize >= currRowSize)
	{
		total += rowSize;
		--rowSize;
	}
	return total;
}

int QBertLevel::GetUpperRowMax(int currRowSize) const
{
	int total{};
	int rowSize = m_MaxTileRow;

	currRowSize = Math2D::Clamp(currRowSize - 1, 0, currRowSize);
	while (rowSize >= currRowSize)
	{
		total += rowSize;
		--rowSize;
	}
	--total;
	return total;
}