#pragma once
#include "Component.h"

class CameraTester : public Component
{
public:

	virtual void Initialize(bool) override {};
	virtual void Update() override;
	virtual void Render() const override;

private:
	float m_MoveSpeed = 150.f;
	float m_ScaleSpeed = 1.f;
	float m_RotationSpeed = 50.f;
};