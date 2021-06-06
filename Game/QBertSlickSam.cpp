#include "pch.h"
#include "QBertSlickSam.h"
#include "QBertEvents.h"
#include "QBertLevel.h"
#include "QBertCharacterMovement.h"
#include "QBertTile.h"

QBertSlickSam::QBertSlickSam()
	: m_IsSlick{}
{
}

QBertSlickSam::~QBertSlickSam()
{
}

void QBertSlickSam::Initialize(bool forceInitialize)
{
	if (!forceInitialize && m_IsInitialized)
		return;

	QBertCharacter::Initialize();

	SetRestDelay(0.75f);

	if (!m_pSprite)
	{
		m_pSprite = GetGameObject()->GetComponent<SpriteComponent>();

		m_pSprite->SetTexture("QBert/Sprites.png");
		m_pSprite->SetTextureSize({ m_TextureSize, m_TextureSize });
		m_pSprite->SetInitTexOffset({ 0.f, 128.f });
		m_pSprite->SetPlayOnce(false);

		m_pSprite->SetSpriteLayout(SpriteLayout::Vertical);
		const int frames = 4;
		m_pSprite->SetMaxFrames(frames, 0);
		m_pSprite->SetTickRate(m_RestDelay);
	}

	//translate sprite above cube
	TransformComponent& trans = GetGameObject()->GetTransform();
	Vector4& dstRct = m_pSprite->GetTexture2D()->GetDestRect();
	dstRct.y = m_TextureSize * trans.GetWorld().Scale.y;

	m_CurrentRestDelay = m_RestDelay;

	m_IsInitialized = true;
}

void QBertSlickSam::Render() const
{
}

void QBertSlickSam::Update()
{
	if (GetMovement()->IsMoving())
		return;

	GameState& gs = GameState::GetInstance();
	m_CurrentRestDelay -= gs.DeltaTime;
	if (m_CurrentRestDelay > 0.f)
		return;

	m_CurrentRestDelay = m_RestDelay;
	QBertRedBall::HandleAI();
}

void QBertSlickSam::Kill(bool hasFallen)
{
	hasFallen;
	GetLevel()->QueueEvent(this, GameEvent::kill_enemy);
	GetSubject()->Notify(GetGameObject(), QBertEvent::event_sam_catched);
}