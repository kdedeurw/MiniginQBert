#include "pch.h"
#include "QBertRedBall.h"
#include "QBertEvents.h"
#include "QBertLevel.h"
#include "QBertCharacterMovement.h"

const float QBertRedBall::m_TextureSize = 16.f;

QBertRedBall::QBertRedBall()
	: m_pTexture{}
	, m_CurrentRestDelay{}
{
}

QBertRedBall::~QBertRedBall()
{
	m_pTexture = nullptr;
}

void QBertRedBall::Initialize()
{
	QBertCharacter::Initialize();

	//init chain is not in right order
	//GetMovement()->AssignCharacter(this);

	if (!m_pTexture)
		m_pTexture = GetGameObject()->GetComponent<Texture2DComponent>();

	m_pTexture->SetTexture("QBert/Sprites.png");
	m_pTexture->SetTextureSize({ m_TextureSize, m_TextureSize });
	m_pTexture->SetTextureOffset({ 0.f, m_TextureSize });

	//translate sprite above cube
	TransformComponent& trans = GetGameObject()->GetTransform();
	Vector4& dstRct = m_pTexture->GetTexture2D()->GetDestRect();
	dstRct.y += m_TextureSize * trans.GetWorld().Scale.y;

	m_CurrentRestDelay = m_RestDelay;
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

	if (rand() % 2 == 0)
	{
		pMovement->TryMoveTo(MoveDirection::BottomLeft);
	}
	else
	{
		pMovement->TryMoveTo(MoveDirection::BottomRight);
	}
}

void QBertRedBall::HasMoved()
{
	Vector2 texOffset = m_pTexture->GetTextureOffset();
	texOffset.x += m_TextureSize;
	m_pTexture->SetTextureOffset(texOffset);
}

void QBertRedBall::HasLanded()
{
	Vector2 texOffset = m_pTexture->GetTextureOffset();
	texOffset.x -= m_TextureSize;
	m_pTexture->SetTextureOffset(texOffset);
}

void QBertRedBall::Kill(bool hasFallen)
{
	hasFallen;
	GetLevel()->RemoveEnemy(this);
}