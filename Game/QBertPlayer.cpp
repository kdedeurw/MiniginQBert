#include "pch.h"
#include "QBertPlayer.h"
#include "KeyboardMouseListener.h"
#include "GameObject.h"
#include "Subject.h"
#include "QBertEvents.h"
#include "Components.h"
#include "GameState.h"
#include "QBertTile.h"
#include "QBertLevel.h"

const float QBertPlayer::m_TextureSize = 16.f;

enum class MovementState
{
	Ground,
	Jump,
};

QBertPlayer::QBertPlayer()
	: m_IsKilled{}
	, m_PlayerId{ PlayerId::Player1 }
	, m_pTexture{}
	, m_MoveDelay{}
	, m_pCurrentTile{}
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

	m_pCurrentTile = m_pLevel->GetUpperTile();
}

void QBertPlayer::Render() const
{
}

void QBertPlayer::Update()
{
	HandleMove();
}

void QBertPlayer::Respawn()
{
	m_pCurrentTile = m_pLevel->GetUpperTile();

	TransformComponent& trans = m_pGameObject->GetTransform();
	trans.SetPosition(m_pCurrentTile->GetGameObject()->GetTransform().GetWorld().Position);
}

void QBertPlayer::HandleMove()
{
	m_MoveDelay -= GameState::GetInstance().DeltaTime;

	if (m_MoveDelay > 0.f)
		return;

	const KeyboardMouseListener& kbml = KeyboardMouseListener::GetInstance();
	TransformComponent& trans = m_pGameObject->GetTransform();
	const float tileSize = QBertTile::GetTextureSize();
	const Vector2& scale = trans.GetWorld().Scale;

	const QBertTile::Neighbours& neighbours = m_pCurrentTile->GetNeighbours();

	if (kbml.IsPressed(Key::A))
	{
		trans.Translate((-tileSize / 2) * scale.x, (-tileSize / 2 - m_TextureSize / 2) * scale.y);
		m_pTexture->SetTextureOffset({ m_TextureSize * 7, 0.f });

		if (neighbours.pLeftBottomNeighbour)
		{
			m_MoveDelay = 0.5f;
			m_pCurrentTile = neighbours.pLeftBottomNeighbour;
			m_pLevel->JumpOnTile(m_pCurrentTile->GetTileId());
		}
		else
			OnDeath();
	}
	else if (kbml.IsPressed(Key::D))
	{
		trans.Translate((tileSize / 2) * scale.x, (tileSize / 2 + m_TextureSize / 2) * scale.y);
		m_pTexture->SetTextureOffset({ m_TextureSize * 1, 0.f });

		if (neighbours.pRightTopNeighbour)
		{
			m_MoveDelay = 0.5f;
			m_pCurrentTile = neighbours.pRightTopNeighbour;
			m_pLevel->JumpOnTile(m_pCurrentTile->GetTileId());
		}
		else
			OnDeath();
	}
	if (kbml.IsPressed(Key::W))
	{
		trans.Translate((-tileSize / 2) * scale.x, (tileSize / 2 + m_TextureSize / 2) * scale.y);
		m_pTexture->SetTextureOffset({ m_TextureSize * 3, 0.f });

		if (neighbours.pLeftTopNeighbour)
		{
			m_MoveDelay = 0.5f;
			m_pCurrentTile = neighbours.pLeftTopNeighbour;
			m_pLevel->JumpOnTile(m_pCurrentTile->GetTileId());
		}
		else
			OnDeath();

	}
	else if (kbml.IsPressed(Key::S))
	{
		trans.Translate((tileSize / 2) * scale.x, (-tileSize / 2 - m_TextureSize / 2) * scale.y);
		m_pTexture->SetTextureOffset({ m_TextureSize * 5, 0.f });

		if (neighbours.pRightBottomNeighbour)
		{
			m_MoveDelay = 0.5f;
			m_pCurrentTile = neighbours.pRightBottomNeighbour;
			m_pLevel->JumpOnTile(m_pCurrentTile->GetTileId());
		}
		else
			OnDeath();
	}
}

void QBertPlayer::OnDeath()
{
	m_pSubject->Notify(m_pGameObject, m_PlayerId);
	//TODO: respawn player
}