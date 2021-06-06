#include "pch.h"
#include "Texture2D.h"
#include "Texture2DComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include "Math2D.h"
#include "GameObject.h"
#include "GlobalMemoryPools.h"

Texture2DComponent::Texture2DComponent()
	: m_pTexture{ GlobalMemoryPools::GetInstance().CreateTexture2D(nullptr) }
	, m_Pivot{ 0.5f, 0.5f }
{}

Texture2DComponent::~Texture2DComponent()
{
	m_pTexture = nullptr;
}

void Texture2DComponent::Initialize()
{
	//if (!m_pTexture->InitializeTexture())
	//	throw std::exception("Texture2DComponent::Initialize > invalid texture or texture not set");
	//resets dest- and sourcerects;
}

void Texture2DComponent::Render() const
{
	const Transform& worldTrans = GetGameObject()->GetTransform().GetWorld();
	const Vector2& pos = worldTrans.Position;
	const Vector2& scale = worldTrans.Scale;
	const Vector4& dstRect = m_pTexture->GetDestRect();
	const Vector4& srcRect = m_pTexture->GetSourceRect();
	Renderer::GetInstance().RenderTexture(m_pTexture->GetSDLTexture(), pos, dstRect, srcRect, scale, worldTrans.Rotation, m_Pivot, m_pTexture->GetFlip() );
}

void Texture2DComponent::SetTexture(SDL_Texture* pTexture)
{
	m_pTexture->SetTexture(pTexture);
}

void Texture2DComponent::SetTexture(const std::string& fileAsset)
{
	m_pTexture->SetTexture(fileAsset);
}

void Texture2DComponent::SetPivot(Vector2 pivot)
{
	Math2D::Clamp(pivot.x);
	Math2D::Clamp(pivot.y);
	m_Pivot = std::move(pivot);
}

void Texture2DComponent::SetTextureSize(const Vector2& sizes)
{
	m_pTexture->GetSourceRect().z = sizes.x;
	m_pTexture->GetDestRect().z = sizes.x;

	m_pTexture->GetSourceRect().w = sizes.y;
	m_pTexture->GetDestRect().w = sizes.y;
}

Vector2 Texture2DComponent::GetTextureSize() const
{
	const Vector4& srcRct = m_pTexture->GetSourceRect();
	return Vector2{ srcRct.z, srcRct.w };
}

void Texture2DComponent::SetTextureOffset(const Vector2& offset)
{
	Vector4& srcRct = m_pTexture->GetSourceRect();
	srcRct.x = offset.x;
	srcRct.y = offset.y;
}

Vector2 Texture2DComponent::GetTextureOffset() const
{
	Vector4& srcRct = m_pTexture->GetSourceRect();
	return Vector2{ srcRct.x, srcRct.y };
}