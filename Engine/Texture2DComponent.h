#pragma once
#include "Component.h"
#include "Vectors.h"
#include <string>

struct SDL_Texture;
class Texture2D;
class Texture2DComponent : public Component
{
public:
	Texture2DComponent();
	virtual ~Texture2DComponent();

	virtual void Initialize() override;
	void Render() const override;
	virtual void Update() {};

	Texture2D* GetTexture2D() const { return m_pTexture; };

	void SetTexture(SDL_Texture* pTexture);
	void SetTexture(const std::string& fileAsset);
	void SetPivot(Vector2 pivot);

	//set width and height of 1 frame (in pixels) -> srcRect.zw
	void SetTextureSize(const Vector2& sizes);
	//get width and height of SourceRect
	Vector2 GetTextureSize() const;
	//set offset in pixels in x and y directions -> srcRct.xy
	void SetTextureOffset(const Vector2& offset);
	//get x and y of SourceRect
	Vector2 GetTextureOffset() const;

protected:
	Texture2D* m_pTexture;
	Vector2 m_Pivot;

};