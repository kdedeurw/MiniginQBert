#include "pch.h"
#include "QBertCoily.h"
#include "QBertEvents.h"
#include "QBertLevel.h"
#include "QBertCharacterMovement.h"
#include "QBertPlayer.h"
#include "QBertTile.h"

const float QBertCoily::m_TextureHeight = 32.f;

QBertCoily::QBertCoily()
	: m_IsDescending{ true }
{
}

QBertCoily::~QBertCoily()
{
}

void QBertCoily::Initialize(bool forceInitialize)
{
	if (!forceInitialize && m_IsInitialized)
		return;

	QBertCharacter::Initialize();

	//init chain is not in right order
	//GetMovement()->AssignCharacter(this);

	if (!m_pSprite)
	{
		m_pSprite = GetGameObject()->GetComponent<SpriteComponent>();

		m_pSprite->SetTexture("QBert/Sprites.png");
		m_pSprite->SetTextureSize({ m_TextureSize, m_TextureSize });
		m_pSprite->SetInitTexOffset({ m_TextureSize * 4, m_TextureSize });

		m_pSprite->SetMaxFrames(1, 0);
		m_pSprite->SetPlayOnce(true);
	}

	//translate sprite above cube
	TransformComponent& trans = GetGameObject()->GetTransform();
	Vector4& dstRct = m_pSprite->GetTexture2D()->GetDestRect();
	dstRct.y = m_TextureSize * trans.GetWorld().Scale.y;

	SetRestDelay(0.5f);

	m_CurrentRestDelay = m_RestDelay;

	m_IsInitialized = true;
}

void QBertCoily::Render() const
{
}

void QBertCoily::Update()
{
	if (GetMovement()->IsMoving())
		return;

	GameState& gs = GameState::GetInstance();
	m_CurrentRestDelay -= gs.DeltaTime;
	if (m_CurrentRestDelay > 0.f)
		return;

	m_CurrentRestDelay = m_RestDelay;
	HandleAI();
}

void QBertCoily::Kill(bool hasFallen)
{
	hasFallen;
	//TODO: combine?
	GetLevel()->QueueEvent(this, GameEvent::kill_enemy);
	GetSubject()->Notify(GetGameObject(), QBertEvent::event_coily_killed);
}

void QBertCoily::HandleAI()
{
	QBertCharacterMovement* pMovement = GetMovement();
	if (pMovement->IsMoving())
		return;

	if (m_IsDescending)
		QBertRedBall::HandleAI();
	else
		HandleCoilyAI(pMovement);
}

void QBertCoily::HandleCoilyAI(QBertCharacterMovement* pMovement)
{
	float closestMag = FLT_MAX;
	QBertPlayer* pClosestPlayer{};
	const Transform& world = GetGameObject()->GetTransform().GetWorld();

	const std::vector<QBertPlayer*>& pPlayers = GetLevel()->GetPlayers();
	if (pPlayers.size() == 0)
		return;

	for (QBertPlayer* pPlayer : pPlayers)
	{
		Vector2 coilyToPlayer = pPlayer->GetGameObject()->GetTransform().GetWorld().Position - world.Position;
		const float mag = Math2D::Magnitude(coilyToPlayer);
		if (mag < closestMag)
		{
			closestMag = mag;
			pClosestPlayer = pPlayer;
		}
	}
	const Vector2& closestPlayerPos = pClosestPlayer->GetGameObject()->GetTransform().GetWorld().Position;

	const QBertBaseTile::Neighbours& neighbours = pMovement->GetCurrentTile()->GetNeighbours();
	Vector2 coilyToNeighbour{};
	MoveDirection moveDir = MoveDirection::TopLeft;
	QBertBaseTile* pNeighbour{ neighbours.pLeftTopNeighbour };
	float newMag{};

	bool canMove{};
	//start at Top Left
	if (pNeighbour)
	{
		QBertCharacter* pTileCharacter{ pNeighbour->GetCurrentCharacter() };
		//only move when QBert's on the target tile OR tile is empty
		if (!pTileCharacter || pTileCharacter->GetType() == QBertCharacterType::QBert)
		{
			coilyToNeighbour = pNeighbour->GetGameObject()->GetTransform().GetWorld().Position - closestPlayerPos;
			closestMag = Math2D::Magnitude(coilyToNeighbour);
			newMag = closestMag;
			m_pSprite->SetInitTexOffset({ m_TextureSize * 2, m_TextureHeight });
			canMove = true;
		}
	}

	pNeighbour = neighbours.pRightTopNeighbour;
	if (pNeighbour)
	{
		QBertCharacter* pTileCharacter{ pNeighbour->GetCurrentCharacter() };
		//only move when QBert's on the target tile OR tile is empty
		if (!pTileCharacter || pTileCharacter->GetType() == QBertCharacterType::QBert)
		{
			//if Top Right is closer
			coilyToNeighbour = pNeighbour->GetGameObject()->GetTransform().GetWorld().Position - closestPlayerPos;
			newMag = Math2D::Magnitude(coilyToNeighbour);
			//if it can't currently move, move towards this dir, otherwise check if tile is closer
			if (!canMove || newMag < closestMag)
			{
				closestMag = newMag;
				moveDir = MoveDirection::TopRight;
				m_pSprite->SetInitTexOffset({ 0.f, m_TextureHeight });
				canMove = true;
			}
		}
	}

	pNeighbour = neighbours.pLeftBottomNeighbour;
	if (pNeighbour)
	{
		QBertCharacter* pTileCharacter{ pNeighbour->GetCurrentCharacter() };
		//only move when QBert's on the target tile OR tile is empty
		if (!pTileCharacter || pTileCharacter->GetType() == QBertCharacterType::QBert)
		{
			//if Bottom Left is closer
			coilyToNeighbour = pNeighbour->GetGameObject()->GetTransform().GetWorld().Position - closestPlayerPos;
			newMag = Math2D::Magnitude(coilyToNeighbour);
			//if it can't currently move, move towards this dir, otherwise check if tile is closer
			if (!canMove || newMag < closestMag)
			{
				closestMag = newMag;
				moveDir = MoveDirection::BottomLeft;
				m_pSprite->SetInitTexOffset({ m_TextureSize * 6, m_TextureHeight });
				canMove = true;
			}
		}
	}

	pNeighbour = neighbours.pRightBottomNeighbour;
	if (pNeighbour)
	{
		QBertCharacter* pTileCharacter{ pNeighbour->GetCurrentCharacter() };
		//only move when QBert's on the target tile OR tile is empty
		if (!pTileCharacter || pTileCharacter->GetType() == QBertCharacterType::QBert)
		{
			//if Bottom Right is closer, just move
			coilyToNeighbour = pNeighbour->GetGameObject()->GetTransform().GetWorld().Position - closestPlayerPos;
			//if it can't currently move, move towards this dir, otherwise check if tile is closer
			if (!canMove || Math2D::Magnitude(coilyToNeighbour) < closestMag)
			{
				moveDir = MoveDirection::BottomRight;
				m_pSprite->SetInitTexOffset({ m_TextureSize * 4, m_TextureHeight });
				canMove = true;
			}
		}
	}

	if (canMove)
		pMovement->TryMoveTo(moveDir);
}

void QBertCoily::HasMoved()
{
	Vector2 texOffset = m_pSprite->GetTextureOffset();
	texOffset.x += m_TextureSize;
	m_pSprite->SetInitTexOffset(texOffset);
}

void QBertCoily::HasLanded()
{
	Vector2 texOffset = m_pSprite->GetTextureOffset();
	texOffset.x -= m_TextureSize;
	m_pSprite->SetInitTexOffset(texOffset);

	if (!m_IsDescending)
		return;

	const int currTileId = GetMovement()->GetCurrentTile()->GetId();
	if (currTileId < GetLevel()->GetLowerRowSize())
	{
		m_IsDescending = false;
		m_pSprite->SetTextureSize({ m_TextureSize, m_TextureHeight });
		m_pSprite->SetInitTexOffset({ 0.f, m_TextureHeight });
		m_CurrentRestDelay = m_RestDelay * 2.f;
	}
}