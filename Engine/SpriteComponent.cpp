#include "pch.h"
#include "SpriteComponent.h"

SpriteComponent::SpriteComponent()
	: m_PlayOnce{}
	, m_Layout{ SpriteLayout::Horizontal }
	, m_CurrentFrame{}
	, m_MaxXFrames{ 1 }
	, m_MaxYFrames{ 1 }
	, m_Tick{}
	, m_TickRate{ 0.25f }
{}

SpriteComponent::~SpriteComponent()
{}

void SpriteComponent::Initialize(bool forceInitialize)
{
	if (!forceInitialize && m_IsInitialized)
		return;

	//Set init frame coords
	Vector4& srcRect = m_pTexture->GetSourceRect();
	if (m_Layout == SpriteLayout::Horizontal)
	{
		srcRect.x = srcRect.z * (m_CurrentFrame % m_MaxXFrames);
		srcRect.x += m_InitOffset.x;
		if (m_MaxYFrames > 1)
			srcRect.y = srcRect.w * (m_CurrentFrame / m_MaxYFrames) + m_InitOffset.y;
		else
			srcRect.y += m_InitOffset.y;
	}
	else
	{
		if (m_MaxXFrames > 1)
			srcRect.x = srcRect.z * (m_CurrentFrame / m_MaxXFrames) + m_InitOffset.x;
		else
			srcRect.x += m_InitOffset.x;
		srcRect.y = srcRect.w * (m_CurrentFrame % m_MaxYFrames);
		srcRect.y += m_InitOffset.y;
	}

	m_IsInitialized = true;
}

void SpriteComponent::Update()
{
	const int maxFrames = m_MaxXFrames * m_MaxYFrames;
	if (m_PlayOnce)
	{
		if (m_CurrentFrame >= maxFrames)
			return;
	}

	m_Tick += GameState::GetInstance().DeltaTime;
	if (m_Tick >= m_TickRate)
	{
		++m_CurrentFrame;
		if (m_CurrentFrame >= maxFrames)
			m_CurrentFrame = 0;
		m_Tick = 0;
		
		//TODO: 'clean'
		Vector4& srcRect = m_pTexture->GetSourceRect();
		if (m_Layout == SpriteLayout::Horizontal)
		{
			srcRect.x = srcRect.z * (m_CurrentFrame % m_MaxXFrames);
			srcRect.x += m_InitOffset.x;
			if (m_MaxYFrames > 1)
				srcRect.y = srcRect.w * (m_CurrentFrame / m_MaxYFrames);
			else
				srcRect.y = m_InitOffset.y;
		}
		else
		{
			if (m_MaxXFrames > 1)
				srcRect.x = srcRect.z * (m_CurrentFrame / m_MaxXFrames);
			else
				srcRect.x = m_InitOffset.x;
			srcRect.y = srcRect.w * (m_CurrentFrame % m_MaxYFrames);
			srcRect.y += m_InitOffset.y;
		}
	}
}

void SpriteComponent::SetPlayOnce(bool enable)
{
	m_PlayOnce = enable;
}

void SpriteComponent::SetCurrentFrame(int frame)
{
	m_CurrentFrame = frame;
}

void SpriteComponent::SetMaxFrames(int maxX, int maxY)
{
	if (maxX > 0)
		m_MaxXFrames = maxX;
	if (maxY > 0)
		m_MaxYFrames = maxY;
}

void SpriteComponent::SetTickRate(float tickRate)
{
	m_TickRate = tickRate;
}

void SpriteComponent::SetSpriteLayout(SpriteLayout layout)
{
	m_Layout = layout;
}

void SpriteComponent::Reset()
{
	m_CurrentFrame = 0;
	m_Tick = 0;
}