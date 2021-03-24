#include "pch.h"
#include "SpriteComponent.h"
#include "GameState.h"

SpriteComponent::SpriteComponent()
	: m_PlayOnce{}
	, m_Frame{}
	, m_MaxFrames{ 1 }
	, m_Tick{}
	, m_TickRate{ 0.25f }
	, m_Offset{}
{}

SpriteComponent::~SpriteComponent()
{}

void SpriteComponent::Initialize()
{}

//void SpriteComponent::Render() const
//{
//}

void SpriteComponent::Update()
{
	m_Tick += GameState::GetInstance().DeltaTime;
	if (m_PlayOnce)
	{
		if (m_Frame >= m_MaxFrames)
			return;
	}
	if (m_Tick >= m_TickRate)
	{
		++m_Frame;
		if (m_Frame >= m_MaxFrames)
			m_Frame = 0;
		m_Tick = 0;
		
		Vector4& srcRect = m_pTexture->GetSourceRect();
		srcRect.x = srcRect.z * m_Frame + m_Offset.x;
	}
}

void SpriteComponent::SetPlayOnce(bool enable)
{
	m_PlayOnce = enable;
}

void SpriteComponent::SetCurrentFrame(int frame)
{
	m_Frame = frame;
}

void SpriteComponent::SetMaxFrames(int max)
{
	m_MaxFrames = max;
}

void SpriteComponent::SetSizes(const Vector2& sizes)
{
	m_pTexture->GetSourceRect().z = sizes.x;
	m_pTexture->GetDestRect().z = sizes.x;

	m_pTexture->GetSourceRect().w = sizes.y;
	m_pTexture->GetDestRect().w = sizes.y;
}

void SpriteComponent::SetTickRate(float tickRate)
{
	m_TickRate = tickRate;
}

void SpriteComponent::SetOffset(const Vector2& offset)
{
	m_Offset = offset;
}

void SpriteComponent::Reset()
{
	m_Frame = 0;
	m_Tick = 0;
}