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

	//init chain is not in right order
	//GetMovement()->AssignCharacter(this);

	if (!m_pTexture)
		m_pTexture = GetGameObject()->GetComponent<Texture2DComponent>();

	m_pTexture->SetTexture("QBert/Sprites.png");
	m_pTexture->SetTextureSize({ m_TextureSize, m_TextureSize });
	m_pTexture->SetTextureOffset({m_TextureSize * 6, 0.f});

	//translate sprite above cube
	TransformComponent& trans = GetGameObject()->GetTransform();
	Vector4& dstRct = m_pTexture->GetTexture2D()->GetDestRect();
	dstRct.y += m_TextureSize * trans.GetWorld().Scale.y;
}

void QBertPlayer::Render() const
{
}

void QBertPlayer::Update()
{
	HandleInput();
}

void QBertPlayer::Respawn()
{
	GetLevel()->ClearAllEnemies();
	GetMovement()->SetToTile(GetLevel()->GetUpperTile());
	m_IsKilled = false;
}

void QBertPlayer::HasMoved()
{
	Vector2 texOffset = m_pTexture->GetTextureOffset();
	texOffset.x += m_TextureSize;
	m_pTexture->SetTextureOffset(texOffset);
}

void QBertPlayer::HasLanded()
{
	Vector2 texOffset = m_pTexture->GetTextureOffset();
	texOffset.x -= m_TextureSize;
	m_pTexture->SetTextureOffset(texOffset);
}

void QBertPlayer::HandleInput()
{
	const KeyboardMouseListener& kbml = KeyboardMouseListener::GetInstance();

	QBertCharacterMovement* pMovement = GetMovement();
	if (pMovement->IsMoving())
		return;

	//TODO: make commands
	if (kbml.IsPressed(Key::Q))
	{
		m_pTexture->SetTextureOffset({ m_TextureSize * 2, 0.f });
		pMovement->TryMoveTo(MoveDirection::TopLeft);
	}
	else if (kbml.IsPressed(Key::A))
	{
		m_pTexture->SetTextureOffset({ m_TextureSize * 6, 0.f });
		pMovement->TryMoveTo(MoveDirection::BottomLeft);
	}
	else if (kbml.IsPressed(Key::E))
	{
		m_pTexture->SetTextureOffset({ 0.f, 0.f });
		pMovement->TryMoveTo(MoveDirection::TopRight);
	}
	else if (kbml.IsPressed(Key::D))
	{
		m_pTexture->SetTextureOffset({ m_TextureSize * 4, 0.f });
		pMovement->TryMoveTo(MoveDirection::BottomRight);
	}
}

void QBertPlayer::Kill(bool hasFallen)
{
	GetSubject()->Notify(GetGameObject(), QBertEvent::event_player_die);
	m_IsKilled = true;

	//TODO: add respawn delay

	if (hasFallen)
		Respawn();
}