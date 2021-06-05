#pragma once
#include "SingletonRef.h"
#include "Vectors.h"
#include "RGBAColour.h"
#include "RenderFlip.h"

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;
class Camera;
class Renderer final : public SingletonRef<Renderer>
{
public:
	void Init(SDL_Window* window);
	void Render() const;
	void Destroy();

	void DrawPoint(float x, float y, RGBAColour colour = RGBAColour{ 255, 255, 255, 255 }, bool isCameraTransformations = true) const;
	void DrawPoint(Vector2 pos, RGBAColour colour = RGBAColour{ 255, 255, 255, 255 }, bool isCameraTransformations = true) const { DrawPoint(std::move(pos.x), std::move(pos.y), std::move(colour), isCameraTransformations); };
	void DrawPoint(float x, float y, float size = 1.f, RGBAColour colour = RGBAColour{ 255, 255, 255, 255 }, bool isCameraTransformations = true) const;
	void DrawPoint(Vector2 pos, float size = 1.f, RGBAColour colour = RGBAColour{ 255, 255, 255, 255 }, bool isCameraTransformations = true) const { DrawPoint(std::move(pos.x), std::move(pos.y), std::move(size), std::move(colour), isCameraTransformations); }

	void DrawLine(float x1, float y1, float x2, float y2, RGBAColour colour = RGBAColour{ 255, 255, 255, 255 }, bool isCameraTransformations = true) const;
	void DrawLine(Vector2 p1, Vector2 p2, RGBAColour colour = RGBAColour{ 255, 255, 255, 255 }, bool isCameraTransformations = true) const
	{ DrawLine(std::move(p1.x), std::move(p1.y), std::move(p2.x), std::move(p2.y), colour, isCameraTransformations); }

	//DEBUG FUNCTION ONLY!
	void DrawTextImmediate(const std::string& text, Font* pFont, float x, float y, float scaleX = 1.f, float scaleY = 1.f, RGBAColour colour = {255, 255, 255, 255});

	//function specifically designed for rendering SDL text
	void RenderText(SDL_Texture* pTexture, float x, float y, float destX, float destY, float scaleX = 1.f, float scaleY = 1.f, float angle = 0.f, const Vector2& pivot = Vector2{ 0.5f, 0.5f }, RenderFlip flip = RenderFlip::None) const;
	//will render without camera transformations
	void RenderUITexture(SDL_Texture* pTexture, float x, float y, float destX, float destY, float width, float height, float srcX, float srcY, float srcW, float srcH, float scaleX, float scaleY, float angle = 0.f, const Vector2& pivot = Vector2{ 0.5f, 0.5f }, RenderFlip flip = RenderFlip::None) const;

	void RenderTexture(SDL_Texture* pTexture, float x, float y, float angle = 0.f, const Vector2& pivot = Vector2{ 0.5f, 0.5f }, RenderFlip flip = RenderFlip::None) const;
	void RenderTexture(SDL_Texture* pTexture, float x, float y, float width, float height, float angle = 0.f, const Vector2& pivot = Vector2{ 0.5f, 0.5f }, RenderFlip flip = RenderFlip::None) const;
	void RenderTexture(SDL_Texture* pTexture, float x, float y, float width, float height, float srcX, float srcY, float angle = 0.f, const Vector2& pivot = Vector2{ 0.5f, 0.5f }, RenderFlip flip = RenderFlip::None) const;
	void RenderTexture(SDL_Texture* pTexture, float x, float y, float width, float height, float srcX, float srcY, float srcW, float srcH, float angle = 0.f, const Vector2& pivot = Vector2{ 0.5f, 0.5f }, RenderFlip flip = RenderFlip::None) const;
	void RenderTexture(SDL_Texture* pTexture, float x, float y, float destX, float destY, float width, float height, float srcX, float srcY, float srcW, float srcH, float scaleX, float scaleY, float angle = 0.f, const Vector2& pivot = Vector2{ 0.5f, 0.5f }, RenderFlip flip = RenderFlip::None) const;

	//overloads
	void RenderUITexture(SDL_Texture* pTexture, const Vector2& pos, const Vector4& dstRect, const Vector4& srcRect, const Vector2& scale, float angle = 0.f, const Vector2& pivot = Vector2{ 0.5f, 0.5f }, RenderFlip flip = RenderFlip::None) const
	{ RenderUITexture(pTexture, pos.x, pos.y, dstRect.x, dstRect.y, dstRect.z, dstRect.w, srcRect.x, srcRect.y, srcRect.z, srcRect.w, scale.x, scale.y, angle, pivot, flip); };

	void RenderTexture(SDL_Texture* pTexture, const Vector2& pos, float angle = 0.f, const Vector2& pivot = Vector2{ 0.5f, 0.5f }, RenderFlip flip = RenderFlip::None) const
	{ RenderTexture(pTexture, pos.x, pos.y, angle, pivot, flip); };
	void RenderTexture(SDL_Texture* pTexture, const Vector2& pos, const Vector2& dimensions, float angle = 0.f, const Vector2& pivot = Vector2{ 0.5f, 0.5f }, RenderFlip flip = RenderFlip::None) const
	{ RenderTexture(pTexture, pos.x, pos.y, dimensions.x, dimensions.y, angle, pivot, flip); };
	void RenderTexture(SDL_Texture* pTexture, const Vector4& dstRect, float angle = 0.f, const Vector2& pivot = Vector2{ 0.5f, 0.5f }, RenderFlip flip = RenderFlip::None) const
	{ RenderTexture(pTexture, dstRect.x, dstRect.y, dstRect.z, dstRect.w, angle, pivot, flip); };
	void RenderTexture(SDL_Texture* pTexture, const Vector2& pos, const Vector2& dimensions, const Vector2& srcXY, float angle = 0.f, const Vector2& pivot = Vector2{ 0.5f, 0.5f }, RenderFlip flip = RenderFlip::None) const
	{ RenderTexture(pTexture, pos.x, pos.x, dimensions.x, dimensions.y, srcXY.x, srcXY.y, angle, pivot, flip); };
	void RenderTexture(SDL_Texture* pTexture, const Vector2& pos, const Vector2& dimensions, const Vector2& srcXY, const Vector2& srcWH, float angle = 0.f, const Vector2& pivot = Vector2{ 0.5f, 0.5f }, RenderFlip flip = RenderFlip::None) const
	{ RenderTexture(pTexture, pos.x, pos.x, dimensions.x, dimensions.y, srcXY.x, srcXY.y, srcWH.x, srcWH.y, angle, pivot, flip); };
	void RenderTexture(SDL_Texture* pTexture, const Vector4& dstRect, const Vector4& srcRect, float angle = 0.f, const Vector2& pivot = Vector2{ 0.5f, 0.5f }, RenderFlip flip = RenderFlip::None) const
	{ RenderTexture(pTexture, dstRect.x, dstRect.y, dstRect.z, dstRect.w, srcRect.x, srcRect.y, srcRect.z, srcRect.w, angle, pivot, flip); };
	void RenderTexture(SDL_Texture* pTexture, const Vector2& pos, const Vector4& dstRect, const Vector4& srcRect, const Vector2& scale, float angle = 0.f, const Vector2& pivot = Vector2{ 0.5f, 0.5f }, RenderFlip flip = RenderFlip::None) const
	{ RenderTexture(pTexture, pos.x, pos.y, dstRect.x, dstRect.y, dstRect.z, dstRect.w, srcRect.x, srcRect.y, srcRect.z, srcRect.w, scale.x, scale.y, angle, pivot, flip); };

	SDL_Renderer* GetSDLRenderer() const { return m_pRenderer; }

private:
	SDL_Renderer* m_pRenderer{};

	void AddCameraTransformations(Camera& camera, float& x, float& y, float& angle) const;
	void CalculateTransformations(float& x, float& y, float& width, float& height, float destX, float destY, float scaleX, float scaleY) const;
};