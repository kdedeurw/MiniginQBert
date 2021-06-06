#include "pch.h"
#include "QBertCharacterMovement.h"
#include "QBertLevel.h"
#include "QBertTile.h"
#include "QBertCharacter.h"

QBertCharacterMovement::QBertCharacterMovement()
	: m_IsOnTile{}
	, m_IsTryMove{}
	, m_CurrentMoveDelay{}
	, m_pCharacter{}
	, m_pCurrentTile{}
	, m_FormerPos{}
	, m_DesiredPos{}
{
}

QBertCharacterMovement::~QBertCharacterMovement()
{
}

void QBertCharacterMovement::Initialize()
{
	if (!m_pCharacter)
		m_pCharacter = GetGameObject()->GetComponent<QBertCharacter>();
}

void QBertCharacterMovement::Update()
{
	HandleMove();
}

void QBertCharacterMovement::TryMoveTo(MoveDirection moveState)
{
	if (m_IsTryMove)
		return;

	const QBertTile::Neighbours& neighbours = m_pCurrentTile->GetNeighbours();
	TransformComponent& trans = GetGameObject()->GetTransform();

	m_IsTryMove = true;
	QBertTile* pNextTile{};

	switch (moveState)
	{
	case MoveDirection::TopLeft:
		pNextTile = neighbours.pLeftTopNeighbour;
		break;
	case MoveDirection::TopRight:
		pNextTile = neighbours.pRightTopNeighbour;
		break;
	case MoveDirection::BottomLeft:
		pNextTile = neighbours.pLeftBottomNeighbour;
		break;
	case MoveDirection::BottomRight:
		pNextTile = neighbours.pRightBottomNeighbour;
		break;
	case MoveDirection::Left:
		break;
	case MoveDirection::Right:
		break;
	}

	if (pNextTile)
	{
		m_CurrentMoveDelay = m_MoveDelay;

		m_FormerPos = trans.GetWorld().Position;
		m_DesiredPos = pNextTile->GetGameObject()->GetTransform().GetWorld().Position;

		m_pCurrentTile->LeaveCharacter();
		m_pCurrentTile = pNextTile;

		m_IsOnTile = false;

		//TODO: observer/subject?
		m_pCharacter->HasMoved();
	}
	else
	{
		m_pCurrentTile->LeaveCharacter();
		m_pCharacter->Kill(true);
	}
}

void QBertCharacterMovement::SetToTile(QBertTile* pTile, bool isMoveOn)
{
	m_IsTryMove = false;
	if (isMoveOn)
		LandOnTile(pTile);
	//stop character from stepping on tile
	m_IsOnTile = true;

	if (m_pCurrentTile)
		m_pCurrentTile->LeaveCharacter();

	pTile->EnterCharacter(m_pCharacter);

	m_pCurrentTile = pTile;
	m_CurrentMoveDelay = m_MoveDelay;
	m_DesiredPos = pTile->GetGameObject()->GetTransform().GetWorld().Position;

	TransformComponent& trans = GetGameObject()->GetTransform();
	trans.SetPosition(m_DesiredPos);
	m_FormerPos = m_DesiredPos;
}

void QBertCharacterMovement::SetToTile(int tileId, bool isMoveOn)
{
	SetToTile(GetLevel()->GetTile(tileId), isMoveOn);
}

void QBertCharacterMovement::HandleMove()
{
	GameState& gs = GameState::GetInstance();
	TransformComponent& trans = GetGameObject()->GetTransform();

	if (m_CurrentMoveDelay >= 0.f)
	{
		const float remappedMoveDelay = m_CurrentMoveDelay / m_MoveDelay;
		trans.SetPosition(Math2D::LERP(m_FormerPos, m_DesiredPos, 1.f - remappedMoveDelay));

		m_CurrentMoveDelay -= gs.DeltaTime;
		return;
	}

	LandOnTile(m_pCurrentTile);
}

void QBertCharacterMovement::LandOnTile(QBertTile* pTile)
{
	if (m_IsOnTile)
		return;

	GetLevel()->MoveOnTile(m_pCharacter, pTile->GetId());
	m_IsOnTile = true;

	m_IsTryMove = false;

	//TODO: observer/subject?
	m_pCharacter->HasLanded();
}

void QBertCharacterMovement::LandOnTile(int tileId)
{
	LandOnTile(GetLevel()->GetTile(tileId));
}

QBertLevel* QBertCharacterMovement::GetLevel() const
{
	return m_pCharacter->GetLevel();
}