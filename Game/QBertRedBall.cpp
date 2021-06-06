#include "pch.h"
#include "QBertRedBall.h"
#include "QBertEvents.h"
#include "QBertLevel.h"
#include "QBertCharacterMovement.h"
#include "QBertTile.h"

const float QBertRedBall::m_TextureSize = 16.f;

QBertRedBall::QBertRedBall()
	: m_pSprite{}
	, m_CurrentRestDelay{}
{
}

QBertRedBall::~QBertRedBall()
{
	m_pSprite = nullptr;
}

void QBertRedBall::Initialize(bool forceInitialize)
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
		m_pSprite->SetInitTexOffset({ 0.f, m_TextureSize });

		m_pSprite->SetMaxFrames(1, 0);
		m_pSprite->SetPlayOnce(true);
	}

	//translate sprite above cube
	TransformComponent& trans = GetGameObject()->GetTransform();
	Vector4& dstRct = m_pSprite->GetTexture2D()->GetDestRect();
	dstRct.y = m_TextureSize * trans.GetWorld().Scale.y;

	m_CurrentRestDelay = m_RestDelay;

	m_IsInitialized = true;
}

void QBertRedBall::Render() const
{
}

void QBertRedBall::Update()
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

void QBertRedBall::HandleAI()
{
	QBertCharacterMovement* pMovement = GetMovement();
	if (pMovement->IsMoving())
		return;

	bool canMove{};
	MoveDirection moveDir = MoveDirection::BottomLeft;
	QBertCharacter* pTileCharacter{};

	const QBertTile::Neighbours& neighbours = pMovement->GetCurrentTile()->GetNeighbours();
	//only move when QBert's on the target tile OR tile is empty
	if (neighbours.pLeftBottomNeighbour)
		pTileCharacter = neighbours.pLeftBottomNeighbour->GetCurrentCharacter();

	//if tile is at bottom of grid, RedBall will plummet to its death
	if (!pTileCharacter || pTileCharacter->GetType() == QBertCharacterType::QBert)
		canMove = true;

	if (rand() % 2 == 0)
	{
		if (neighbours.pRightBottomNeighbour)
			pTileCharacter = neighbours.pRightBottomNeighbour->GetCurrentCharacter();

		if (!pTileCharacter || pTileCharacter->GetType() == QBertCharacterType::QBert)
		{
			moveDir = MoveDirection::BottomRight;
			canMove = true;
		}
	}

	if (canMove)
		pMovement->TryMoveTo(moveDir);
}

void QBertRedBall::HasMoved()
{
	Vector2 texOffset = m_pSprite->GetTextureOffset();
	texOffset.x += m_TextureSize;
	m_pSprite->SetInitTexOffset(texOffset);
}

void QBertRedBall::HasLanded()
{
	Vector2 texOffset = m_pSprite->GetTextureOffset();
	texOffset.x -= m_TextureSize;
	m_pSprite->SetInitTexOffset(texOffset);
}

void QBertRedBall::Kill(bool hasFallen)
{
	hasFallen;
	GetLevel()->QueueEvent(this, GameEvent::kill_enemy);
}