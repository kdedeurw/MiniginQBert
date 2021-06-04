#pragma once
#include "Component.h"
#include "RGBAColour.h"
#include <string>

class Font;
class Texture;
struct SDL_Texture;
struct Vector2;
class TextComponent : public Component
{
public:
	TextComponent();
	virtual ~TextComponent();
	TextComponent(const TextComponent& other) = delete;
	TextComponent(TextComponent&& other) = delete;
	TextComponent& operator=(const TextComponent& other) = delete;
	TextComponent& operator=(TextComponent&& other) = delete;

	void Initialize() override;
	void Update() override;
	void Render() const override;

	void SetFont(Font* pFont);
	void SetText(std::string text);
	void SetColour(RGBAColour colour);
	Vector2& GetOffset() { return m_Offset; };
	//texturedata might NOT always be valid, be sure what you're doing here!
	Texture* GetTextureData() const { return m_pTextureData; };

private:
	bool m_NeedsUpdate;
	Font* m_pFont;
	Texture* m_pTextureData;
	Vector2 m_Offset;
	RGBAColour m_Colour;
	std::string m_Text;

	Texture* ReplenishTextureData(SDL_Texture* pTexture);
};