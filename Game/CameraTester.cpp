#include "pch.h"
#include "CameraTester.h"
#include "GameState.h"
#include "Renderer.h"
#include "WindowInfo.h"
#include "RGBAColour.h"

void CameraTester::Update()
{
	GameState& gs = GameState::GetInstance();
	KeyboardMouseListener& kbml = KeyboardMouseListener::GetInstance();
	const float deltaTime = gs.DeltaTime;
	Camera& cam = gs.Camera;

	//position
	if (kbml.IsPressed(Key::A))
	{
		cam.Translate(cam.GetRight() * -m_MoveSpeed * deltaTime);
	}
	else if (kbml.IsPressed(Key::D))
	{
		cam.Translate(cam.GetRight() * m_MoveSpeed * deltaTime);
	}
	if (kbml.IsPressed(Key::W))
	{
		cam.Translate(cam.GetForward() * m_MoveSpeed * deltaTime);
	}
	else if (kbml.IsPressed(Key::S))
	{
		cam.Translate(cam.GetForward() * -m_MoveSpeed * deltaTime);
	}

	//scale
	if (kbml.IsPressed(Key::ArrowUp))
	{
		cam.GetScale().x += m_ScaleSpeed * deltaTime;
	}
	else if (kbml.IsPressed(Key::ArrowDown))
	{
		cam.GetScale().x -= m_ScaleSpeed * deltaTime;
	}
	if (kbml.IsPressed(Key::ArrowLeft))
	{
		cam.GetScale().y -= m_ScaleSpeed * deltaTime;
	}
	else if (kbml.IsPressed(Key::ArrowRight))
	{
		cam.GetScale().y += m_ScaleSpeed * deltaTime;
	}

	//rotation
	if (kbml.IsPressed(Key::Q))
	{
		cam.Rotate(m_RotationSpeed * deltaTime);
	}
	else if (kbml.IsPressed(Key::E))
	{
		cam.Rotate(-m_RotationSpeed * deltaTime);
	}
}

void CameraTester::Render() const
{
	GameState& gs = GameState::GetInstance();
	Camera& cam = gs.Camera;
	WindowInfo& wi = *gs.pWindowInfo;
	Vector2 center{}, fwd{}, right{};
	center.x += wi.Width / 2.f;
	center.y += wi.Height / 2.f;
	fwd.x += wi.Width / 2.f;
	fwd.y += wi.Height / 2.f;
	right.x += wi.Width / 2.f;
	right.y += wi.Height / 2.f;

	fwd += cam.GetForward() * 5.f;

	right += cam.GetRight() * 5.f;

	Renderer::GetInstance().DrawLine(center, fwd, RGBAColour{ 0, 0, 255, 255 }, false);
	Renderer::GetInstance().DrawLine(center, right, RGBAColour{ 0, 255, 0, 255 }, false);
}