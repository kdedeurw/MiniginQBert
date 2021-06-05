#include "pch.h"
#include "QBertCharacterMovement.h"
#include "QBertLevel.h"
#include "QBertTile.h"
#include "QBertCharacter.h"

QBertCharacterMovement::QBertCharacterMovement()
	: m_IsOnTile{}
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
		m_pCharacter = m_pGameObject->GetComponent<QBertCharacter>();
}

void QBertCharacterMovement::Update()
{
	HandleMove();
}

void QBertCharacterMovement::SetToTile(QBertTile* pTile, bool isMoveOn)
{
	if (m_pCurrentTile)
		m_pCurrentTile->LeaveCharacter();

	m_pCurrentTile = pTile;

	if (isMoveOn)
		pTile->EnterCharacter(m_pCharacter);

	m_DesiredPos = pTile->GetGameObject()->GetTransform().GetWorld().Position;

	TransformComponent& trans = m_pGameObject->GetTransform();
	trans.SetPosition(m_DesiredPos);
}

void QBertCharacterMovement::SetToTile(int tileId, bool isMoveOn)
{
	SetToTile(m_pCharacter->GetLevel()->GetTile(tileId), isMoveOn);
}

void QBertCharacterMovement::HandleMove()
{
	GameState& gs = GameState::GetInstance();
	TransformComponent& trans = m_pGameObject->GetTransform();

	if (IsMoving())
	{
		const float remappedMoveDelay = m_CurrentMoveDelay / m_MoveDelay;
		trans.SetPosition(Math2D::LERP(m_FormerPos, m_DesiredPos, 1.f - remappedMoveDelay));

		m_CurrentMoveDelay -= gs.DeltaTime;
		return;
	}

	LandOnTile(m_pCurrentTile);

	const KeyboardMouseListener& kbml = KeyboardMouseListener::GetInstance();
	const QBertTile::Neighbours& neighbours = m_pCurrentTile->GetNeighbours();

	bool isInput{};
	QBertTile* pNextTile{};

	//TODO: make commands

	if (kbml.IsPressed(Key::A))
	{
		//m_pTexture->SetTextureOffset({ m_TextureSize * 6, 0.f });

		pNextTile = neighbours.pLeftBottomNeighbour;
		isInput = true;
	}
	else if (kbml.IsPressed(Key::D))
	{
		//m_pTexture->SetTextureOffset({ 0.f, 0.f });

		pNextTile = neighbours.pRightTopNeighbour;
		isInput = true;
	}
	else if (kbml.IsPressed(Key::W))
	{
		//m_pTexture->SetTextureOffset({ m_TextureSize * 2, 0.f });

		pNextTile = neighbours.pLeftTopNeighbour;
		isInput = true;

	}
	else if (kbml.IsPressed(Key::S))
	{
		//m_pTexture->SetTextureOffset({ m_TextureSize * 4, 0.f });

		pNextTile = neighbours.pRightBottomNeighbour;
		isInput = true;
	}

	if (!isInput)
		return;

	if (pNextTile)
	{
		m_CurrentMoveDelay = m_MoveDelay;

		m_FormerPos = trans.GetWorld().Position;
		m_DesiredPos = pNextTile->GetGameObject()->GetTransform().GetWorld().Position;

		m_pCurrentTile->LeaveCharacter();
		m_pCurrentTile = pNextTile;

		m_IsOnTile = false;

		//TODO: broadcast "Has Moved/Jumped" event to character

		//Vector2 texOffset = m_pTexture->GetTextureOffset();
		//texOffset.x += m_TextureSize;
		//m_pTexture->SetTextureOffset(texOffset);
	}
	else
	{
		m_pCurrentTile->LeaveCharacter();
		m_pCharacter->Kill(true);
	}
}

void QBertCharacterMovement::LandOnTile(QBertTile* pTile)
{
	if (m_IsOnTile)
		return;

	m_pCharacter->GetLevel()->MoveOnTile(m_pCharacter, pTile->GetId());
	m_IsOnTile = true;

	//TODO: broadcast "HasLanded" event to character
}

void QBertCharacterMovement::LandOnTile(int tileId)
{
	LandOnTile(m_pCharacter->GetLevel()->GetTile(tileId));
}