#include "pch.h"
#include <SDL.h>
#include <SDL_ttf.h>

#include "TextComponent.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include "GlobalMemoryPools.h"
#include "Vectors.h"

TextComponent::TextComponent()
	: Component{}
	, m_NeedsUpdate{ true }
	, m_Text{ "Sample Text" }
	, m_pFont{ nullptr } //TODO: assign basic font
	, m_pTextureData{ GlobalMemoryPools::GetInstance().CreateTexture2D(nullptr) } //get memory address, this will be overwritten at will
	, m_Colour{}
{}

TextComponent::~TextComponent()
{
	m_pFont = nullptr;
	delete m_pTextureData;
	m_pTextureData = nullptr;
}

void TextComponent::Initialize()
{
	Update();
}

void TextComponent::Update()
{
	if (m_NeedsUpdate)
	{
		SDL_DestroyTexture(m_pTextureData->GetSDLTexture()); //destroy old texture before losing pointer
		SDL_Surface* pSurface = TTF_RenderText_Blended(m_pFont->GetFont(), m_Text.c_str(), 
			SDL_Color{ m_Colour.r, m_Colour.g, m_Colour.b, m_Colour.a });
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
		m_pTextureData = ReplenishTextureData(pTexture);
		m_NeedsUpdate = false;
	}
}

void TextComponent::Render() const
{
	if (m_pTextureData != nullptr)
	{
		const Vector3& pos = m_pGameObject->GetTransform().GetWorld().Position;
		Renderer::GetInstance().RenderText(m_pTextureData->GetSDLTexture(), pos.x, pos.y, m_Offset.x, m_Offset.y);
	}
}

void TextComponent::SetFont(Font* pFont)
{
	m_pFont = pFont;
}

// This implementation uses the "dirty flag" pattern
void TextComponent::SetText(std::string text)
{
	m_Text = std::move(text);
	m_NeedsUpdate = true;
}

void TextComponent::SetColour(RGBAColour colour)
{
	m_Colour = std::move(colour);
}

Texture* TextComponent::ReplenishTextureData(SDL_Texture* pTexture)
{
	return reinterpret_cast<Texture*>(new (m_pTextureData) Texture{ pTexture });
	//will only call Texture's constructor, but use Texture2D's allocator
	//TODO: fix small wastage of 8 bytes per object
}