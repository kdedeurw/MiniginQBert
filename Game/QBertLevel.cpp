#include "pch.h"
#include "QBertLevel.h"
#include "QBertTile.h"
#include "QBertPlayer.h"
#include "QBertGameObserver.h"
#include "QBertLevelReader.h"
#include "QBertCharacterMovement.h"
#include "QBertRedBall.h"
#include "QBertCoily.h"
#include "QBertSlickSam.h"
#include "QBertSpinningDisk.h"

QBertLevel::QBertLevel()
	: m_CurrRound{ 0 }
	, m_CurrLevel{ 0 }
	, m_GameWin{}
	, m_CurrentTargetTiles{}
	, m_CurrentEnemySpawnDelay{}
	, m_CurrentEnemyId{}
	, m_CurrentRoundWinDelay{}
	, m_pObserver{}
	, m_pTiles{}
	, m_pEnemies{}
	, m_pPlayers{}
	, m_pDisks{}
	, m_LevelDatas{ QBertLevelReader::GetLevelData("../Resources/QBertLevels.json") }
{
	m_pTiles.reserve(28);
	m_pEnemies.reserve(10);
	m_pPlayers.reserve(2);
}

QBertLevel::~QBertLevel()
{
	if (m_pObserver)
		delete m_pObserver;
	m_pObserver = nullptr;
}

void QBertLevel::Initialize(bool forceInitialize)
{
	if (!forceInitialize && m_IsInitialized)
		return;

	//TODO: find workaround
	QBertCharacter::m_pLevel = this;

	m_CurrentEnemySpawnDelay = m_EnemySpawnDelay;

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
		}
		totalY -= tileSize / 4;
		totalX += tileSize / 2;
		--delimiter;
	}

	ConnectTiles();

	AddDiskToLevel(false);
	AddDiskToLevel(true);

	GameObject* pPlayer = CreatePlayer();
	GetGameObject()->AddChildObject(pPlayer);
	QBertCharacterMovement* pMovement = pPlayer->GetComponent<QBertCharacterMovement>();
	//TODO: fix for coop/versus
	pMovement->SetToTile(GetUpperTile());

	m_IsInitialized = true;
}

void QBertLevel::PostRender() const
{
#ifdef _DEBUG

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
	for (QBertSpinningDisk* pDisk : m_pDisks)
	{
		QBertBaseTile* pNeighbour = pDisk->GetNeighbours().pRightBottomNeighbour;
		if (pNeighbour)
		Renderer::GetInstance().DrawLine(pDisk->GetGameObject()->GetTransform().GetWorld().Position,
			pNeighbour->GetGameObject()->GetTransform().GetWorld().Position, RGBAColour{ 255, 0, 0, 128 });
		pNeighbour = pDisk->GetNeighbours().pLeftBottomNeighbour;
		if (pNeighbour)
			Renderer::GetInstance().DrawLine(pDisk->GetGameObject()->GetTransform().GetWorld().Position,
				pNeighbour->GetGameObject()->GetTransform().GetWorld().Position, RGBAColour{ 255, 0, 0, 128 });
	}

#endif
}

void QBertLevel::Update()
{
	GameState& gs = GameState::GetInstance();

	HandleGameEventsQueue();

	m_CurrentRoundWinDelay -= gs.DeltaTime;
	if (m_CurrentRoundWinDelay < 0.f)
		HandleEnemySpawning();

	HandleGameOver();
}

void QBertLevel::HandleGameEventsQueue()
{
	while (!m_Events.empty())
	{
		const GameEventRequest& req = m_Events.front();
		switch (req.Event)
		{
		case GameEvent::kill_enemy:
			RemoveEnemy(dynamic_cast<QBertCharacter*>(req.pEntity));
			break;
		case GameEvent::kill_all_enemies:
			ClearAllEnemies();
			break;
		case GameEvent::kill_player:
			break;
		case GameEvent::kill_all_players:
			ClearAllPlayers();
			break;
		case GameEvent::kill_disk:
			RemoveDisk(dynamic_cast<QBertSpinningDisk*>(req.pEntity));
			break;
		case GameEvent::kill_all_enemies_non_coily:
			ClearAllButCoily();
			break;
		}
		m_Events.pop();
	}
}

void QBertLevel::ClearAllEnemies()
{
	for (QBertCharacter* pEnemy : m_pEnemies)
	{
		RemoveEnemy(pEnemy);
	}
	m_pEnemies.clear();
	for (QBertTile* pTile : m_pTiles)
	{
		pTile->LeaveCharacter();
	}
	m_CurrentEnemySpawnDelay = m_EnemySpawnDelay;
}

void QBertLevel::ClearAllButCoily()
{
	std::vector<QBertCharacter*> pCoilies{};
	for (QBertCharacter* pEnemy : m_pEnemies)
	{
		if (pEnemy->GetType() == QBertCharacterType::Coily)
			pCoilies.push_back(pEnemy);
		else
			RemoveEnemy(pEnemy);
	}
	m_pEnemies.swap(pCoilies);
	for (QBertTile* pTile : m_pTiles)
	{
		pTile->LeaveCharacter();
	}
	m_CurrentEnemySpawnDelay = m_EnemySpawnDelay;
}

void QBertLevel::ClearAllPlayers()
{
	for (QBertPlayer* pPlayer : m_pPlayers)
	{
		pPlayer->GetMovement()->GetCurrentTile()->LeaveCharacter();
		RemoveEntity(pPlayer);
	}
	m_pPlayers.clear();
}

void QBertLevel::RemoveDisk(QBertSpinningDisk* pDisk)
{
	const auto it = std::find(m_pDisks.begin(), m_pDisks.end(), pDisk);
	if (it != m_pDisks.end())
	{
		(*it)->Disconnect();
		m_pDisks.erase(it);
		RemoveEntity(pDisk);
	}
}

void QBertLevel::MoveOnTile(QBertCharacter* pCharacter, int tileId)
{
	QBertTile* pTile = GetTile(tileId);
	if (!pTile)
	{
		QBertSpinningDisk* pDisk = GetDisk(tileId);
		pDisk->TryEnter(pCharacter);
	}
	else
	{
		if (pTile->TryEnter(pCharacter))
		{
			switch (pCharacter->GetTileAlteration())
			{
			case::QBertTileAlteration::None:
				break;
			case::QBertTileAlteration::Next:
				pTile->SetState(EvaluateNextState(pTile->GetState()));
				break;
			case::QBertTileAlteration::Previous:
				pTile->SetState(EvaluatePreviousState(pTile->GetState()));
				break;
			}
		}

		if (m_CurrentTargetTiles == m_AmountOfTiles)
			OnRoundWin();
	}
}

QBertTile* QBertLevel::GetTile(int tileId) const
{
	if (tileId < 0 || tileId >= m_AmountOfTiles)
		return nullptr;
	return m_pTiles[tileId];
}

QBertSpinningDisk* QBertLevel::GetDisk(int id)
{
	return m_pDisks[id - m_AmountOfTiles];
}

void QBertLevel::Reset()
{
	ResetAllTiles();
	ClearAllEnemies();
	for (QBertPlayer* pPlayer : m_pPlayers)
	{
		pPlayer->Respawn();
	}
	m_CurrentEnemySpawnDelay = m_EnemySpawnDelay;
	m_CurrentEnemyId = 0;
	m_CurrLevel = 0;
	m_CurrRound = 0;
}

void QBertLevel::QueueEvent(Component* pEntity, GameEvent event)
{
	GameEventRequest req;
	req.Event = event;
	req.pEntity = pEntity;
	m_Events.push(std::move(req));
}

void QBertLevel::ResetAllTiles()
{
	m_CurrentTargetTiles = 0;
	QBertTile::m_TextureId = m_CurrLevel + m_CurrRound;
	for (QBertTile* pTile : m_pTiles)
		pTile->Reset();
}

void QBertLevel::RemoveEntity(Component* pEntity)
{
	//TODO: what a mess
	GetGameObject()->RemoveChildObject(pEntity->GetGameObject());
	GetGameObject()->GetScene().RemoveGameObject(pEntity->GetGameObject());
}

void QBertLevel::RemoveEnemy(QBertCharacter* pEnemy)
{
	const auto it = std::find(m_pEnemies.begin(), m_pEnemies.end(), pEnemy);
	if (it != m_pEnemies.end())
	{
		pEnemy->GetMovement()->GetCurrentTile()->LeaveCharacter();

		m_pEnemies.erase(it);
		RemoveEntity(pEnemy);
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
	if (m_pEnemies.size() == 5)
		return;

	GameState& gs = GameState::GetInstance();

	m_CurrentEnemySpawnDelay -= gs.DeltaTime;
	if (m_CurrentEnemySpawnDelay <= 0.f)
	{
		const QBertLevelData& levelData = m_LevelDatas[m_CurrLevel];
		const std::string& enemies = levelData.Rounds[m_CurrRound].Enemies;
		GameObject* pEnemy = CreateEnemy(enemies[m_CurrentEnemyId]);
		if (pEnemy)
		{
			GetGameObject()->AddChildObject(pEnemy);

			QBertCharacterMovement* pMovement = pEnemy->GetComponent<QBertCharacterMovement>();
			//TODO: invisible spawn tile, no connections???
			//TODO: spawn on other tiles (rows)
			pMovement->SetToTile(GetUpperTile());
		}

		++m_CurrentEnemyId;
		if (m_CurrentEnemyId >= enemies.size())
			m_CurrentEnemyId = 0;

		m_CurrentEnemySpawnDelay = m_EnemySpawnDelay;
	}
}

void QBertLevel::HandleGameOver()
{
	if (!m_pObserver)
		return;

	const auto& stats = m_pObserver->GetStats(PlayerId::Player1);
	if (stats.Lives <= 0)
		OnGameOver();

	OnGameWin();
}

void QBertLevel::CommenceNextRound()
{
	ResetAllTiles();
	ClearAllEnemies();

	for (QBertPlayer* pPlayer : m_pPlayers)
		pPlayer->Respawn(true);

	m_CurrentEnemySpawnDelay = m_EnemySpawnDelay * 1.5f;

	AddDiskToLevel(true);
	AddDiskToLevel(false);
}

void QBertLevel::AddDiskToLevel(bool isLeft)
{
	GameObject* pDisk = CreateSpinningDisk();

	QBertSpinningDisk* pDiskComp = pDisk->GetComponent<QBertSpinningDisk>();

	int id{};
	if (isLeft)
		id = GetLowerRowMin(m_LowerRowSize / 2);
	else
		id = GetLowerRowMax(m_LowerRowSize / 2);

	QBertTile* pNeighbour = GetTile(id);
	pNeighbour->AddSpinningDisk(pDiskComp, isLeft);
	const Transform& neighbourWorld = pNeighbour->GetGameObject()->GetTransform().GetWorld();
	Vector2 pos = neighbourWorld.Position;

	const float tileSize = QBertTile::GetTextureSize();

	if (isLeft)
	{
		pos.x -= (tileSize / 2.f) * neighbourWorld.Scale.x;
		pos.y += (tileSize / 2.f) * neighbourWorld.Scale.x;
	}
	else
	{
		pos.x += (tileSize / 2.f) * neighbourWorld.Scale.x;
		pos.y += (tileSize / 2.f) * neighbourWorld.Scale.x;
	}

	pDisk->GetTransform().SetPosition(pos);

	GetGameObject()->AddChildObject(pDisk);
}

void QBertLevel::OnRoundWin()
{
	for (QBertSpinningDisk* pDisk : m_pDisks)
	{
		m_pObserver->GetStats(PlayerId::Player1).Score += 25;
		m_pObserver->OnNotify(GetGameObject(), QBertEvent::event_spinning_pad_used);
		RemoveDisk(pDisk);
	}
	m_pDisks.clear();

	++m_CurrRound;
	std::cout << "Round " << m_CurrRound << " completed!\n";
	if (m_CurrRound >= m_MaxRounds)
	{
		if (OnLevelWin())
			return;
	}
	m_pObserver->OnNotify(GetGameObject(), QBertEvent::event_round_complete);
	CommenceNextRound();

	m_CurrentRoundWinDelay = m_RoundWinDelay;
}

bool QBertLevel::OnLevelWin()
{
	++m_CurrLevel;
	m_CurrRound = 0;
	std::cout << "Level " << m_CurrLevel << " completed!\n";
	if (m_CurrLevel >= m_MaxLevels)
	{
		m_GameWin = true;
		return true;
	}
	return false;
}

void QBertLevel::OnGameWin()
{
	if (!m_GameWin)
		return;

	m_GameWin = false;

	std::cout << "Game won!\n";
	m_CurrLevel = 0;
	m_CurrRound = 0;

	ResetAllTiles();
	ClearAllEnemies();

	for (QBertPlayer* pPlayer : m_pPlayers)
		pPlayer->Respawn(true);

	SceneManager::GetInstance().ToggleScene("QBertScene", false);
	SceneManager::GetInstance().ToggleScene("MainMenuScene", false);
}

void QBertLevel::OnGameOver()
{
	SceneManager::GetInstance().ToggleScene("QBertScene", false);
	Reset();
	CommenceNextRound();
	SceneManager::GetInstance().ToggleScene("GameOverScene", true);
}

GameObject* QBertLevel::CreateTile()
{
	//GlobalMemoryPools& gm = GlobalMemoryPools::GetInstance();
	GameObject* pGo = GetGameObject()->GetScene().CreateGameObject();

	Texture2DComponent* pTexture = new Texture2DComponent{};
	pGo->AddComponent(pTexture);

	QBertTile* pTile = new QBertTile{};
	pGo->AddComponent(pTile);

	pTile->GetSubject()->AddObserver(m_pObserver);

	pTile->m_Id = static_cast<short>(m_pTiles.size());

	m_pTiles.push_back(pTile);

	return pGo;
}

GameObject* QBertLevel::CreatePlayer()
{
	//GlobalMemoryPools& gm = GlobalMemoryPools::GetInstance();
	GameObject* pGo = GetGameObject()->GetScene().CreateGameObject();

	Texture2DComponent* pTexture = new Texture2DComponent{};
	pGo->AddComponent(pTexture);

	QBertCharacterMovement* pMovement = new QBertCharacterMovement{};
	pGo->AddComponent(pMovement);

	QBertPlayer* pPlayer = new QBertPlayer{};
	pGo->AddComponent(pPlayer);

	pMovement->AssignCharacter(pPlayer);
	
	pPlayer->GetSubject()->AddObserver(m_pObserver);

	m_pPlayers.push_back(pPlayer);

	return pGo;
}

GameObject* QBertLevel::CreateEnemy(char type)
{
	//GlobalMemoryPools& gm = GlobalMemoryPools::GetInstance();
	GameObject* pGo = GetGameObject()->GetScene().CreateGameObject();

	QBertCharacter* pEnemy{};
	switch (type)
	{
	case 'R':
		pEnemy = new QBertRedBall{};
		break;
	case 'C':
		pEnemy = new QBertCoily{};
		break;
	case 'U':
		pEnemy = new QBertRedBall{};
		break;
	case 'W':
		pEnemy = new QBertRedBall{};
		break;
	case 'G':
		pEnemy = new QBertRedBall{};
		break;
	case 'S':
		pEnemy = new QBertSlickSam{};
		break;
	default:
		std::cout << "Unsupported enemy: \'" << type << "\'\n";
		break;
	}

	SpriteComponent* pSprite = new SpriteComponent{};
	pGo->AddComponent(pSprite, false);

	QBertCharacterMovement* pMovement = new QBertCharacterMovement{};
	pGo->AddComponent(pMovement, false);

	m_pEnemies.push_back(pEnemy);
	pGo->AddComponent(pEnemy, false);

	pEnemy->GetSubject()->AddObserver(m_pObserver);
	
	pMovement->AssignCharacter(pEnemy);

	return pGo;
}

GameObject* QBertLevel::CreateSpinningDisk()
{
	//GlobalMemoryPools& gm = GlobalMemoryPools::GetInstance();
	GameObject* pGo = GetGameObject()->GetScene().CreateGameObject();

	SpriteComponent* pSprite = new SpriteComponent{};
	pGo->AddComponent(pSprite);

	QBertSpinningDisk* pDisk = new QBertSpinningDisk{};
	pGo->AddComponent(pDisk);

	pDisk->GetSubject()->AddObserver(m_pObserver);

	pDisk->m_Id = static_cast<short>(m_pTiles.size() + m_pDisks.size());

	m_pDisks.push_back(pDisk);

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