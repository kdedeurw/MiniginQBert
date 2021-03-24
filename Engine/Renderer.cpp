#include "pch.h"
#include "Renderer.h"
#include <SDL.h>
#include "SceneManager.h"
#include "Texture2D.h"
#include <SDL_ttf.h>
#include "ResourceManager.h"
#include "Font.h"
#include "GameState.h"
#include "WindowInfo.h"
#include "Camera.h"
#include "Math2D.h"

void Renderer::Init(SDL_Window * window)
{
	m_pRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (!m_pRenderer) 
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
}

void Renderer::Render() const
{
	SDL_RenderClear(m_pRenderer);
	SceneManager::GetInstance().Render();
	SDL_RenderPresent(m_pRenderer);
}

void Renderer::Destroy()
{
	if (m_pRenderer)
	{
		SDL_DestroyRenderer(m_pRenderer);
		m_pRenderer = nullptr;
	}
}

void Renderer::DrawPoint(float x, float y, RGBAColour colour, bool isCameraTransformations) const
{
	SDL_SetRenderDrawColor(m_pRenderer, colour.r, colour.g, colour.b, colour.a);
	if (isCameraTransformations)
	{
		Camera& cam = GameState::GetInstance().Camera;
		const Vector2& camPos = cam.GetPosition();
		x += camPos.x;
		y += camPos.y;
	}
	SDL_RenderDrawPoint(m_pRenderer, std::move((int)x), std::move(GameState::GetInstance().pWindowInfo->Height - (int)y));
	SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 1);
}

void Renderer::DrawLine(float x1, float y1, float x2, float y2, RGBAColour colour, bool isCameraTransformations) const
{
	SDL_SetRenderDrawColor(m_pRenderer, colour.r, colour.g, colour.b, colour.a);
	if (isCameraTransformations)
	{
		Camera& cam = GameState::GetInstance().Camera;
		const Vector2& camPos = cam.GetPosition();
		x1 += camPos.x;
		x2 += camPos.x;
		y1 += camPos.y;
		y2 += camPos.y;
	}
	SDL_RenderDrawLine(m_pRenderer, std::move((int)x1), std::move(GameState::GetInstance().pWindowInfo->Height - (int)y1), std::move((int)x2), std::move(GameState::GetInstance().pWindowInfo->Height - (int)y2));
	SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 1);
}

void Renderer::RenderText(SDL_Texture* pTexture, float x, float y, float destX, float destY, float scaleX, float scaleY, float angle, const Vector2& pivot, RenderFlip flip) const
{
	float width, height;
	{
		int w, h;
		SDL_QueryTexture(pTexture, nullptr, nullptr, &w, &h);
		width = float(w);
		height = float(h);
	}
	AddCameraTransformations(GameState::GetInstance().Camera, x, y, angle);
	CalculateTransformations(x, y, width, height, destX, destY, scaleX, scaleY);

	SDL_Rect dst;
	dst.x = std::move((int)x);
	dst.y = std::move(GameState::GetInstance().pWindowInfo->Height - (int)y);
	dst.w = std::move((int)width);
	dst.h = std::move((int)height);
	SDL_Point point;
	point.x = int(pivot.x * width);
	point.y = int(pivot.y * height);
	SDL_RenderCopyEx(m_pRenderer, pTexture, nullptr, &dst, angle, &point, (SDL_RendererFlip)flip);
}

void Renderer::RenderUITexture(SDL_Texture* pTexture, float x, float y, float destX, float destY, float width, float height, float srcX, float srcY, float srcW, float srcH, float scaleX, float scaleY, float angle, const Vector2& pivot, RenderFlip flip) const
{
	CalculateTransformations(x, y, width, height, destX, destY, scaleX, scaleY);

	SDL_Rect dst;
	dst.x = std::move((int)x);
	dst.y = std::move(GameState::GetInstance().pWindowInfo->Height - (int)y);
	dst.w = std::move((int)width);
	dst.h = std::move((int)height);
	SDL_Rect src;
	src.x = std::move((int)srcX);
	src.y = std::move((int)srcY);
	src.w = std::move((int)srcW);
	src.h = std::move((int)srcH);
	SDL_Point point;
	point.x = int(pivot.x * width);
	point.y = int(pivot.y * height);
	SDL_RenderCopyEx(m_pRenderer, pTexture, &src, &dst, angle, &point, (SDL_RendererFlip)flip);
}

void Renderer::RenderTexture(SDL_Texture* pTexture, float x, float y, float angle, const Vector2& pivot, RenderFlip flip) const
{
	int w, h;
	SDL_QueryTexture(pTexture, nullptr, nullptr, &w, &h);
	RenderTexture(pTexture, std::move(x), std::move(y), std::move((float)w), std::move((float)h), std::move(angle), pivot, flip);
}

void Renderer::RenderTexture(SDL_Texture* pTexture, float x, float y, float width, float height, float angle, const Vector2& pivot, RenderFlip flip) const
{
	AddCameraTransformations(GameState::GetInstance().Camera, x, y, angle);

	SDL_Rect dst;
	dst.x = std::move((int)x);
	dst.y = std::move(GameState::GetInstance().pWindowInfo->Height - (int)y);
	dst.w = std::move((int)width);
	dst.h = std::move((int)height);
	SDL_Point point;
	point.x = int(pivot.x * width);
	point.y = int(pivot.y * height);
	SDL_RenderCopyEx(m_pRenderer, pTexture, nullptr, &dst, angle, &point, (SDL_RendererFlip)flip);
}

void Renderer::RenderTexture(SDL_Texture* pTexture, float x, float y, float width, float height, float srcX, float srcY, float angle, const Vector2& pivot, RenderFlip flip) const
{
	int w, h;
	SDL_QueryTexture(pTexture, nullptr, nullptr, &w, &h);
	RenderTexture(pTexture, std::move(x), std::move(y), std::move(width), std::move(height), std::move(srcX), std::move(srcY), std::move((float)w), std::move((float)h), std::move(angle), pivot, flip);
}

void Renderer::RenderTexture(SDL_Texture* pTexture, float x, float y, float width, float height, float srcX, float srcY, float srcW, float srcH, float angle, const Vector2& pivot, RenderFlip flip) const
{
	AddCameraTransformations(GameState::GetInstance().Camera, x, y, angle);

	SDL_Rect dst;
	dst.x = std::move((int)x);
	dst.y = std::move(GameState::GetInstance().pWindowInfo->Height - (int)y);
	dst.w = std::move((int)width);
	dst.h = std::move((int)height);
	SDL_Rect src;
	src.x = std::move((int)srcX);
	src.y = std::move((int)srcY);
	src.w = std::move((int)srcW);
	src.h = std::move((int)srcH);
	SDL_Point point;
	point.x = int(pivot.x * width);
	point.y = int(pivot.y * height);
	SDL_RenderCopyEx(m_pRenderer, pTexture, &src, &dst, angle, &point, (SDL_RendererFlip)flip);
}

void Renderer::RenderTexture(SDL_Texture* pTexture, float x, float y, float destX, float destY, float width, float height, float srcX, float srcY, float srcW, float srcH, float scaleX, float scaleY, float angle, const Vector2& pivot, RenderFlip flip) const
{
	AddCameraTransformations(GameState::GetInstance().Camera, x, y, angle);
	CalculateTransformations(x, y, width, height, destX, destY, scaleX, scaleY);

	SDL_Rect dst;
	dst.x = std::move((int)x);
	dst.y = std::move(GameState::GetInstance().pWindowInfo->Height - (int)y);
	dst.w = std::move((int)width);
	dst.h = std::move((int)height);
	SDL_Rect src;
	src.x = std::move((int)srcX);
	src.y = std::move((int)srcY);
	src.w = std::move((int)srcW);
	src.h = std::move((int)srcH);
	SDL_Point point;
	point.x = int(pivot.x * width);
	point.y = int(pivot.y * height);
	SDL_RenderCopyEx(m_pRenderer, pTexture, &src, &dst, angle, &point, (SDL_RendererFlip)flip);
}

void Renderer::CalculateTransformations(float& x, float& y, float& width, float& height, float destX, float destY, float scaleX, float scaleY) const
{
	width *= scaleX;
	height *= scaleY;

	x = (x + destX) - (width / 2 * scaleX);
	y = (y + destY) + (height / 2 * scaleY); //sign flipped, because bottom left == 0,0
}

void Renderer::AddCameraTransformations(Camera& camera, float& x, float& y, float& angle) const
{
	const WindowInfo& wi = *GameState::GetInstance().pWindowInfo;
	//camera transformation
	const Vector2& camPos = camera.GetPosition();
	const Vector2& camScale = camera.GetScale();

	//position
	x -= camPos.x;
	y -= camPos.y;
	//rotation
	angle += camera.GetRotation();

	//TODO: scale according to camScale AND WIDTH AND HEIGHT
	//rotational center == center of screen
	x -= wi.Width / 2.f;
	y -= wi.Height / 2.f;

	const float cameraRot = -camera.GetRotation() * Math2D::ToRadians;
	const float sinCalc{ sinf(cameraRot) };
	const float cosCalc{ cosf(cameraRot) };
	const float newX{ x * cosCalc - y * sinCalc };
	y = y * cosCalc + x * sinCalc;
	x = newX;

	//TODO: scale according to camScale AND WIDTH AND HEIGHT
	//undo positional changes
	x += wi.Width / 2.f;
	y += wi.Height / 2.f;

	//scaling
	//TODO: scale according to camScale AND WIDTH AND HEIGHT
	SDL_RenderSetScale(m_pRenderer, camScale.x, camScale.y);
}