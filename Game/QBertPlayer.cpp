#include "pch.h"
#include "QBertPlayer.h"
#include "KeyboardMouseListener.h"
#include "Subject.h"
#include "QBertEvents.h"
#include "QBertTile.h"
#include "QBertLevel.h"

const float QBertPlayer::m_TextureSize = 16.f;

QBertPlayer::QBertPlayer()
	: m_IsKilled{}
	, m_IsOnTile{ true }
	, m_PlayerId{ PlayerId::Player1 }
	, m_pTexture{}
	, m_CurrentMoveDelay{}
	, m_pCurrentTile{}
	, m_DesiredPos{}
	, m_FormerPos{}
{
	if (!m_pSubject)
		m_pSubject = new Subject{};
}

QBertPlayer::~QBertPlayer()
{
}

void QBertPlayer::Initialize()
{
	if (!m_pTexture)
		m_pTexture = m_pGameObject->GetComponent<Texture2DComponent>();

	m_pTexture->SetTexture("QBert/Sprites.png");
	m_pTexture->SetTextureSize({ m_TextureSize, m_TextureSize });
	m_pTexture->SetTextureOffset({m_TextureSize * 6, 0.f});

	TransformComponent& trans = m_pGameObject->GetTransform();
	Vector4& dstRct = m_pTexture->GetTexture2D()->GetDestRect();
	dstRct.y += m_TextureSize * trans.GetWorld().Scale.y;

	m_pCurrentTile = m_pLevel->GetUpperTile();
}

void QBertPlayer::Render() const
{
}

void QBertPlayer::Update()
{
	HandleMove();

	//TODO: grid movement
	//1: start on tile
	//2: find new tile to move on
	//3: move for 0.5f, THEN jump on tile
	//4: lock movement to tile pos
	//5: LERP between desired and current pos OR add gravity factor
}

void QBertPlayer::Respawn()
{
	m_pCurrentTile = m_pLevel->GetUpperTile();
	m_DesiredPos = m_pCurrentTile->GetGameObject()->GetTransform().GetWorld().Position;

	TransformComponent& trans = m_pGameObject->GetTransform();
	trans.SetPosition(m_pCurrentTile->GetGameObject()->GetTransform().GetWorld().Position);
}

void QBertPlayer::HandleMove()
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
		m_pTexture->SetTextureOffset({ m_TextureSize * 6, 0.f });

		pNextTile = neighbours.pLeftBottomNeighbour;
		isInput = true;
	}
	else if (kbml.IsPressed(Key::D))
	{
		m_pTexture->SetTextureOffset({ 0.f, 0.f });

		pNextTile = neighbours.pRightTopNeighbour;
		isInput = true;
	}
	else if (kbml.IsPressed(Key::W))
	{
		m_pTexture->SetTextureOffset({ m_TextureSize * 2, 0.f });

		pNextTile = neighbours.pLeftTopNeighbour;
		isInput = true;

	}
	else if (kbml.IsPressed(Key::S))
	{
		m_pTexture->SetTextureOffset({ m_TextureSize * 4, 0.f });

		pNextTile = neighbours.pRightBottomNeighbour;
		isInput = true;
	}

	if (!isInput)
		return;

	if (pNextTile)
	{
		m_CurrentMoveDelay = m_MoveDelay;
		m_pCurrentTile = pNextTile;
		m_FormerPos = trans.GetWorld().Position;
		m_DesiredPos = pNextTile->GetGameObject()->GetTransform().GetWorld().Position;
		m_IsOnTile = false;


		Vector2 texOffset = m_pTexture->GetTextureOffset();
		texOffset.x += m_TextureSize;
		m_pTexture->SetTextureOffset(texOffset);
	}
	else
		OnDeath();
}

void QBertPlayer::OnDeath()
{
	m_pSubject->Notify(m_pGameObject, m_PlayerId);
	//TODO: respawn player (observer handles for now)
}

void QBertPlayer::LandOnTile(QBertTile* pTile)
{
	if (m_IsOnTile)
		return;

	m_pLevel->JumpOnTile(pTile->GetTileId());
	m_IsOnTile = true;
}