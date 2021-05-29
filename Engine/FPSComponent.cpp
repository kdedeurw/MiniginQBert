#include "pch.h"
#include "FPSComponent.h"
#include "GameObject.h"
#include "TransformComponent.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include "ResourceManager.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "Math.h"
#include "GameState.h"
#include "GlobalMemoryPools.h"

FPSComponent::FPSComponent()
	: m_FpsInterval{}
	, m_CurrentFps{}
	, m_pFont{ ResourceManager::GetInstance().LoadFont("Lingua.otf", 24) }
	, m_Text{ "" }
	, m_ElapsedSec{}
	, m_NeedsUpdate{ true } //do not set this to false to start off, bc the Render function will crash bc invalid pTextureData
	, m_pTextureData{ GlobalMemoryPools::GetInstance().CreateTexture2D(nullptr) } //get memory address, this will be overwritten at will
{
	ResetFPS();
}

FPSComponent::~FPSComponent()
{
	m_pFont = nullptr;
	delete m_pTextureData;
	m_pTextureData = nullptr;
}

void FPSComponent::Update()
{
	++m_FpsInterval;

	GameState& gs = GameState::GetInstance();
	m_ElapsedSec += gs.DeltaTime;
	if (m_ElapsedSec >= 1.f)
	{
		ResetFPS();
		m_ElapsedSec = 0.f;
	}

	if (m_NeedsUpdate)
	{
		SDL_DestroyTexture(m_pTextureData->GetSDLTexture()); //destroy old texture before losing pointer
		const SDL_Color color = { 255,255,255 }; // only white text is supported now
		SDL_Surface* pSurface = TTF_RenderText_Blended(m_pFont->GetFont(), m_Text.c_str(), color);
		if (pSurface == nullptr)
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		SDL_Texture* pTexture = SDL_CreateTextureFromSurface(Renderer::GetInstance().GetSDLRenderer(), pSurface);
		if (pTexture == nullptr)
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(pSurface);
		m_pTextureData = GetTextureData(pTexture);
		m_pTextureData->InitializeTexture(); //TODO: replace for performance
		m_NeedsUpdate = false;
	}
}

void FPSComponent::Render() const
{
	Transform& worldTrans = m_pGameObject->GetTransform().GetWorld();
	Vector2 pos = worldTrans.Position;
	const float rot = worldTrans.Rotation;
	const Vector4& srcRect = m_pTextureData->GetSourceRect();
	const Vector4& dstRect = m_pTextureData->GetDestRect();
	const Vector2& scale = worldTrans.Scale;
	//TODO: define FPS 'corner' position and math (probs in Core::AddFPS) enum class?
	pos.x += dstRect.z / 2;
	pos.y -= dstRect.w / 2;
	if (m_pTextureData)
		Renderer::GetInstance().RenderUITexture(m_pTextureData->GetSDLTexture(), pos, dstRect, srcRect, scale, rot);
}

void FPSComponent::SetFont(Font* pFont)
{
	m_pFont = pFont;
	m_NeedsUpdate = true;
}

unsigned int FPSComponent::GetFPS() const
{
	return m_CurrentFps;
}

void FPSComponent::ResetFPS()
{
	std::cout << m_FpsInterval << '\n';
	m_CurrentFps = m_FpsInterval;
	m_Text = std::move("FPS " + std::to_string(m_FpsInterval));
	m_FpsInterval = 0;
	m_NeedsUpdate = true;
}

Texture2D* FPSComponent::GetTextureData(SDL_Texture* pTexture)
{
	return reinterpret_cast<Texture2D*>(new (m_pTextureData) Texture2D{ pTexture });
}