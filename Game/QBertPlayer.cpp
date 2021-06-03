#include "pch.h"
#include "QBertPlayer.h"
#include "KeyboardMouseListener.h"
#include "GameObject.h"
#include "Subject.h"
#include "QBertEvents.h"
#include "Components.h"
#include "GameState.h"
#include "QBertTile.h"

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
{}

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
}

void QBertPlayer::Render() const
{
}

void QBertPlayer::Update()
{
	m_MoveDelay -= GameState::GetInstance().DeltaTime;

	if (m_MoveDelay > 0.f)
		return;

	m_MoveDelay = 0.5f;

	const KeyboardMouseListener& kbml = KeyboardMouseListener::GetInstance();
	TransformComponent& trans = m_pGameObject->GetTransform();
	const float tileSize = QBertTile::GetTextureSize();
	const Vector2& scale = trans.GetWorld().Scale;
	if (kbml.IsPressed(Key::A))
	{
		trans.Translate((-tileSize / 2) * scale.x, (-tileSize / 2 - m_TextureSize / 2) * scale.y);
		m_pTexture->SetTextureOffset({ m_TextureSize * 6, 0.f });
	}
	else if (kbml.IsPressed(Key::D))
	{
		trans.Translate((tileSize / 2) * scale.x, (tileSize / 2 + m_TextureSize / 2) * scale.y);
		m_pTexture->SetTextureOffset({ m_TextureSize * 0, 0.f });
	}
	if (kbml.IsPressed(Key::W))
	{
		trans.Translate((-tileSize / 2) * scale.x, (tileSize / 2 + m_TextureSize / 2) * scale.y);
		m_pTexture->SetTextureOffset({ m_TextureSize * 2, 0.f });
	}
	else if (kbml.IsPressed(Key::S))
	{
		trans.Translate((tileSize / 2) * scale.x, (-tileSize / 2 - m_TextureSize / 2) * scale.y);
		m_pTexture->SetTextureOffset({ m_TextureSize * 4, 0.f });
	}
}

void QBertPlayer::OnDeath()
{
	m_pSubject->Notify(m_pGameObject, m_PlayerId);
}

void QBertPlayer::OnRespawn()
{
}