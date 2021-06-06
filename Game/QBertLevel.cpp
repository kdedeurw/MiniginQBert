#include "pch.h"
#include "QBertLevel.h"
#include "QBertTile.h"
#include "QBertPlayer.h"
#include "QBertGameObserver.h"
#include "QBertLevelReader.h"
#include "QBertCharacterMovement.h"
#include "QBertRedBall.h"

QBertLevel::QBertLevel()
	: m_CurrRound{}
	, m_CurrLevel{}
	, m_CurrentTargetTiles{}
	, m_CurrentEnemySpawnDelay{}
	, m_CurrentEnemyId{}
	, m_pObserver{}
	, m_pTiles{}
	, m_pPlayer{}
	, m_LevelDatas{ QBertLevelReader::GetLevelData("../Resources/QBertLevels.json") }
{
	m_pTiles.reserve(28);
	m_pEnemies.reserve(10);
}

QBertLevel::~QBertLevel()
{
}

void QBertLevel::Initialize()
{
	//TODO: find workaround
	QBertCharacter::m_pLevel = this;

	QBertLevelReader::GetLevelData("../Resources/QBertLevels.json");

	m_CurrentEnemySpawnDelay = m_EnemySpawnDelay;

	m_pObserver = GlobalMemoryPools::GetInstance().CreateOnStack<QBertGameObserver>();

	const float tileSize = QBertTile::GetTextureSize();
	const Vector2& scale = GetGameObject()->GetTransform().GetWorld().Scale;
	int delimiter = m_LowerRowSize;
	float totalX{ tileSize };
	float totalY{};

	//TODO: store positions in json?
	for (int j{}; j < m_LowerRowSize; ++j)
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

	GameObject* pPlayer = CreatePlayer();
	GetGameObject()->AddChildObject(pPlayer);
	pPlayer->Initialize();
}

void QBertLevel::PostRender() const
{
	for (QBertTile* pTile : m_pTiles)
	{
		const Vector2& scale = pTile->GetGameObject()->GetTransform().GetWorld().Scale;
		const QBertTile::Neighbours& neighBours = pTile->GetNeighbours();
		if (neighBours.pLeftTopNeighbour)
		{
			Renderer::GetInstance().DrawLine(pTile->GetGameObject()->GetTransform().GetWorld().Position,
				neighBours.pLeftTopNeighbour->GetGameObject()->GetTransform().GetWorld().Position, RGBAColour{ 255, 0, 0, 128 });
		}
		if (neighBours.pRightTopNeighbour)
		{
			Renderer::GetInstance().DrawLine(pTile->GetGameObject()->GetTransform().GetWorld().Position,
				neighBours.pRightTopNeighbour->GetGameObject()->GetTransform().GetWorld().Position, RGBAColour{ 255, 0, 0, 128 });
		}
		if (neighBours.pRightBottomNeighbour)
		{
			Renderer::GetInstance().DrawLine(pTile->GetGameObject()->GetTransform().GetWorld().Position,
				neighBours.pRightBottomNeighbour->GetGameObject()->GetTransform().GetWorld().Position, RGBAColour{ 255, 0, 0, 128 });
		}
		if (neighBours.pLeftBottomNeighbour)
		{
			Renderer::GetInstance().DrawLine(pTile->GetGameObject()->GetTransform().GetWorld().Position,
				neighBours.pLeftBottomNeighbour->GetGameObject()->GetTransform().GetWorld().Position, RGBAColour{ 255, 0, 0, 128 });
		}

		Font* pFont = ResourceManager::GetInstance().LoadFont("Lingua.otf", 24);
		const Vector2& pos = pTile->GetGameObject()->GetTransform().GetWorld().Position;
		Renderer::GetInstance().DrawTextImmediate(std::to_string(pTile->GetId()), pFont, pos.x, pos.y, 1.f, 1.f, RGBAColour{ 255, 255, 255, 128 });

		QBertCharacter* pCharacter = pTile->GetCurrentCharacter();
		if (pCharacter)
			Renderer::GetInstance().DrawTextImmediate(typeid(pCharacter->GetType()).name(), pFont, pos.x, pos.y, 1.f, 1.f, RGBAColour{ 0, 0, 255, 128 });

		Renderer::GetInstance().DrawPoint(pTile->GetGameObject()->GetTransform().GetWorld().Position, scale.x, RGBAColour{ 0, 255, 0, 128 });
	}
}

void QBertLevel::Update()
{
	HandleEnemySpawning();
}

void QBertLevel::ClearAllEnemies()
{
	for (QBertCharacter* pEnemy : m_pEnemies)
	{
		GetGameObject()->RemoveChildObject(pEnemy->GetGameObject(), true);
	}
	m_pEnemies.clear();
}

void QBertLevel::MoveOnTile(QBertCharacter* pCharacter, int tileId)
{
	QBertTile* pTile = GetTile(tileId);
	pTile->EnterCharacter(pCharacter);

	const TileState state = pTile->GetState();
	const TileAlteration tileAlteration = GetCharacterTileAlteration(pTile->GetCurrentCharacter()->GetType());
	if (tileAlteration == TileAlteration::Next)
	{
		pTile->SetState(EvaluateNextState(state));
	}
	else if (tileAlteration == TileAlteration::Previous)
	{
		pTile->SetState(EvaluatePreviousState(state));
	}

	if (m_CurrentTargetTiles == m_AmountOfTiles)
		OnRoundWin();
}

QBertTile* QBertLevel::GetTile(int tileId) const
{
	tileId = Math2D::Clamp(tileId, 0, m_AmountOfTiles);
	return m_pTiles[tileId];
}

void QBertLevel::ResetTiles()
{
	m_CurrentTargetTiles = 0;
	QBertTile::m_TextureId = m_CurrLevel + m_CurrRound;
	for (QBertTile* pTile : m_pTiles)
		pTile->Reset();
}

void QBertLevel::RemoveEnemy(QBertCharacter* pEnemy)
{
	const auto it = std::find(m_pEnemies.begin(), m_pEnemies.end(), pEnemy);
	if (it != m_pEnemies.end())
	{
		m_pEnemies.erase(it);
		//TODO: what a mess
		GetGameObject()->RemoveChildObject(pEnemy->GetGameObject());
		GetGameObject()->GetScene().RemoveGameObject(pEnemy->GetGameObject());
	}
}

TileState QBertLevel::EvaluateNextState(TileState state)
{
	const QBertLevelData& currLevelData = m_LevelDatas[m_CurrLevel];
	TileState targetState = static_cast<TileState>(currLevelData.Steps);

	switch (state)
	{
	case TileState::DefaultState:
	{
		if (targetState == TileState::IntermediateState)
			++m_CurrentTargetTiles;

		return TileState::IntermediateState;
	}

	case TileState::IntermediateState:
	{
		if (targetState == TileState::TargetState)
		{
			++m_CurrentTargetTiles;
			return TileState::TargetState;
		}
		else if (currLevelData.CanGoBack)
		{
			--m_CurrentTargetTiles;
			return TileState::DefaultState;
		}
		return TileState::IntermediateState;
	}

	case TileState::TargetState:
	{
		if (currLevelData.CanGoBack)
		{
			//if (targetState == TileState::IntermediateState)
			//{
			//	--m_CurrentTargetTiles;
			//	return TileState::DefaultState;
			//}
			//else
			//{
				--m_CurrentTargetTiles;
				return TileState::IntermediateState;
			//}
		}
		return TileState::TargetState;
	}
	}

	return TileState::DefaultState;
}

TileState QBertLevel::EvaluatePreviousState(TileState state)
{
	switch (state)
	{
	case TileState::DefaultState:
		return TileState::DefaultState;

	case TileState::IntermediateState:
		--m_CurrentTargetTiles;
		return TileState::DefaultState;

	case TileState::TargetState:
		--m_CurrentTargetTiles;
		return TileState::IntermediateState;
	}

	return TileState::DefaultState;
}

void QBertLevel::HandleEnemySpawning()
{
	if (m_pEnemies.size() == 3)
		return;

	GameState& gs = GameState::GetInstance();

	m_CurrentEnemySpawnDelay -= gs.DeltaTime;
	if (m_CurrentEnemySpawnDelay <= 0.f)
	{
		const QBertLevelData& levelData = m_LevelDatas[m_CurrLevel];
		const std::string& enemies = levelData.Rounds[m_CurrRound].Enemies;
		GameObject* pEnemy = CreateEnemy(enemies[m_CurrentEnemyId]);
		GetGameObject()->AddChildObject(pEnemy);
		pEnemy->Initialize();

		++m_CurrentEnemyId;
		if (m_CurrentEnemyId >= enemies.size())
			m_CurrentEnemyId = 0;

		m_CurrentEnemySpawnDelay = m_EnemySpawnDelay;
	}
}

void QBertLevel::CommenceNextRound()
{
	ResetTiles();
	ClearAllEnemies();

	m_pPlayer->Respawn();

	m_CurrentEnemySpawnDelay = m_EnemySpawnDelay;
}

QBertLevel::TileAlteration QBertLevel::GetCharacterTileAlteration(QBertCharacterType type)
{
	switch (type)
	{
	case QBertCharacterType::QBert:
		return TileAlteration::Next;

	case QBertCharacterType::RedBall:
	case QBertCharacterType::Coily:
	case QBertCharacterType::UggWrongway:
	case QBertCharacterType::GreenBall:
		return TileAlteration::None;

	case QBertCharacterType::SlickSam:
		return TileAlteration::Previous;
	}

	return TileAlteration::None;
}

void QBertLevel::OnRoundWin()
{
	++m_CurrRound;
	std::cout << "Round " << m_CurrRound << " completed!\n";
	if (m_CurrRound >= m_MaxRounds)
	{
		if (OnLevelWin())
			return;
	}
	m_pObserver->OnNotify(GetGameObject(), QBertEvent::event_round_complete);
	CommenceNextRound();
}

bool QBertLevel::OnLevelWin()
{
	++m_CurrLevel;
	m_CurrRound = 0;
	std::cout << "Level " << m_CurrLevel << " completed!\n";
	if (m_CurrLevel >= m_MaxLevels)
	{
		OnGameWin();
		return true;
	}
	return false;
}

void QBertLevel::OnGameWin()
{
	std::cout << "Game won!\n";
	m_CurrLevel = 0;
	m_CurrRound = 0;

	ResetTiles();
	ClearAllEnemies();

	m_pPlayer->Respawn();

	//TODO: return to main menu
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

	m_pPlayer = gm.CreateComponent<QBertPlayer>();

	pGo->AddComponent(m_pPlayer);

	QBertCharacterMovement* pMovement = gm.CreateComponent<QBertCharacterMovement>();
	pGo->AddComponent(pMovement);

	//TODO: fix for coop/versus
	pMovement->SetToTile(GetUpperTile());

	pMovement->AssignCharacter(m_pPlayer);
	
	m_pPlayer->GetSubject()->AddObserver(m_pObserver);

	return pGo;
}

GameObject* QBertLevel::CreateEnemy(char type)
{
	GlobalMemoryPools& gm = GlobalMemoryPools::GetInstance();
	GameObject* pGo = GetGameObject()->GetScene().CreateGameObject();

	Texture2DComponent* pTexture = gm.CreateComponent<Texture2DComponent>();
	pGo->AddComponent(pTexture);

	QBertCharacter* pEnemy{};

	switch (type)
	{
	case 'R':
		pEnemy = gm.CreateComponent<QBertRedBall>();
	break;
	case 'C':
		pEnemy = gm.CreateComponent<QBertRedBall>();
		break;
	case 'U':
		break;
	case 'W':
		break;
	case 'G':
		break;
	case 'S':
		break;
	default:
		std::cout << "Unsupported enemy: \'" << type << "\'\n";
		break;
	}

	m_pEnemies.push_back(pEnemy);
	pGo->AddComponent(pEnemy);

	QBertCharacterMovement* pMovement = gm.CreateComponent<QBertCharacterMovement>();
	pGo->AddComponent(pMovement);

	pEnemy->GetSubject()->AddObserver(m_pObserver);

	pMovement->AssignCharacter(pEnemy);

	//TODO: invisible spawn tile, no connections???

	//TODO: spawn on other tiles (rows)
	pMovement->SetToTile(GetUpperTile());

	return pGo;
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

	int currRowSize = m_LowerRowSize;
	int relativeTile = 0;
	for (int currTileIdx{}; currTileIdx < m_AmountOfTiles; ++currTileIdx)
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
	int rowSize = m_LowerRowSize;

	currRowSize = Math2D::Clamp(currRowSize + 1, 0, m_LowerRowSize);
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
	int rowSize = m_LowerRowSize;

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
	int rowSize = m_LowerRowSize;

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
	int rowSize = m_LowerRowSize;

	currRowSize = Math2D::Clamp(currRowSize - 1, 0, currRowSize);
	while (rowSize >= currRowSize)
	{
		total += rowSize;
		--rowSize;
	}
	--total;
	return total;
}