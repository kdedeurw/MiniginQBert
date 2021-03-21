#pragma once
#include "Vectors.h"

class Camera
{
public:
	Camera();
	~Camera();

	Vector2& GetPosition();
	Vector2& GetScale();
	//forward and right will NOT be altered
	float& GetRotation();
	Vector2& GetForward();
	Vector2& GetRight();

	//forward and right vectors will be altered
	void SetRotation(float degrees);

	void Translate(float x, float y);
	void Translate(const Vector2& trans);
	void Scale(float x, float y);
	void Scale(const Vector2& scale);
	//forward and right vectors will be altered
	void Rotate(float degrees);

private:
	Vector2 m_Position;
	Vector2 m_Scale;
	float m_Rotation;
	Vector2 m_PerpendicularForward;
	Vector2 m_Right;

	void CalculateFwdRight();
};

//camera will not have a height parameter, this would not make sense
//the same scaling effect can be achieved through scale
//the camera's forward will not point into the screen, but behave as a normal forward vector