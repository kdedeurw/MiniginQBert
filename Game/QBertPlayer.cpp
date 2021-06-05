#include "pch.h"
#include "QBertPlayer.h"
#include "KeyboardMouseListener.h"
#include "Subject.h"
#include "QBertEvents.h"
#include "QBertTile.h"
#include "QBertLevel.h"
#include "QBertCharacterMovement.h"

const float QBertPlayer::m_TextureSize = 16.f;

QBertPlayer::QBertPlayer()
	: m_IsKilled{}
	, m_PlayerId{ PlayerId::Player1 }
	, m_pTexture{}
{
}

QBertPlayer::~QBertPlayer()
{
}

void QBertPlayer::Initialize()
{
	QBertCharacter::Initialize();

	if (!m_pTexture)
		m_pTexture = m_pGameObject->GetComponent<Texture2DComponent>();

	m_pTexture->SetTexture("QBert/Sprites.png");
	m_pTexture->SetTextureSize({ m_TextureSize, m_TextureSize });
	m_pTexture->SetTextureOffset({m_TextureSize * 6, 0.f});

	//translate sprite above cube
	TransformComponent& trans = m_pGameObject->GetTransform();
	Vector4& dstRct = m_pTexture->GetTexture2D()->GetDestRect();
	dstRct.y += m_TextureSize * trans.GetWorld().Scale.y;
}

void QBertPlayer::Render() const
{
}

void QBertPlayer::Update()
{
	if (!GetMovement()->IsMoving())
		HandleInput();

	//Vector2 texOffset = m_pTexture->GetTextureOffset();
	//texOffset.x += m_TextureSize;
	//m_pTexture->SetTextureOffset(texOffset);
}

void QBertPlayer::Respawn()
{
	GetLevel()->ClearAllEnemies();
	GetMovement()->SetToTile(GetLevel()->GetUpperTile());
	m_IsKilled = false;
}

void QBertPlayer::HandleInput()
{
	//TODO: make commands

	const KeyboardMouseListener& kbml = KeyboardMouseListener::GetInstance();
	if (kbml.IsPressed(Key::A))
	{
		m_pTexture->SetTextureOffset({ m_TextureSize * 6, 0.f });
	}
	else if (kbml.IsPressed(Key::D))
	{
		m_pTexture->SetTextureOffset({ 0.f, 0.f });
	}
	else if (kbml.IsPressed(Key::W))
	{
		m_pTexture->SetTextureOffset({ m_TextureSize * 2, 0.f });
	}
	else if (kbml.IsPressed(Key::S))
	{
		m_pTexture->SetTextureOffset({ m_TextureSize * 4, 0.f });
	}
}

void QBertPlayer::Kill(bool hasFallen)
{
	GetSubject()->Notify(m_pGameObject, QBertEvent::event_player_die);
	m_IsKilled = true;

	//TODO: add respawn delay

	if (hasFallen)
		Respawn();
}