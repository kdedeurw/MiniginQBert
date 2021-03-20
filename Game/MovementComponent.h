#pragma once
#include <Component.h>
#include "Vectors.h"

class MovementComponent : public Component
{
public:
	MovementComponent();
	virtual ~MovementComponent();

	virtual void Initialize() override;
	virtual void Render() const override;
	virtual void Update() override;

	float GetAngularVelocity() const;
	const Vector2& GetVelocity() const;

	void AddVelocity(const Vector2& vel);
	void AddAngularVelocity(float vel);

	void SetDesiredVelocity(const Vector2& vel);
	void SetCurrentVelocity(const Vector2& vel);
	void SetAngularVelocity(float vel);

	void AutoOrient();

protected:
	float m_Weight;
	float m_AngularVelocity;
	Vector2 m_DesiredVelocity;

private:
	Vector2 m_CurrentVelocity;
};