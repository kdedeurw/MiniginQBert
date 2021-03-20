#include "pch.h"
#include "Texture2DComponent.h"
#include "ResourceManager.h"
#include "Renderer.h"
#include "GameObject.h"
#include "Transform.h"
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
	Transform& transform = m_pGameObject->GetWorldTransform();
	const Vector2& pos = transform.GetPosition();
	const Vector2& scale = transform.GetScale();
	const Vector4& dstRec = m_pTexture->GetDestRect();
	const Vector4& srcRect = m_pTexture->GetSourceRect();
	Renderer::GetInstance().RenderTexture(m_pTexture->GetSDLTexture(), 
		(pos.x + dstRec.x) - (dstRec.z / 2 * scale.x), //x
		(pos.y + dstRec.y) + (dstRec.w / 2 * scale.y), //y, swapped because origin of XY is down left
		dstRec.z * scale.x, dstRec.w * scale.y, //scale
		srcRect.x, srcRect.y, srcRect.z, srcRect.w, //srcRct
		transform.GetRotation(), //rot
		m_Pivot, //pivot
		m_pTexture->GetFlip()); //flip
}

void Texture2DComponent::Update()
{}

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