#pragma once
#include "Texture2DComponent.h"

enum class SpriteLayout : bool
{
	Horizontal,
	Vertical,
};

class SpriteComponent : public Texture2DComponent
{
public:
	SpriteComponent();
	virtual ~SpriteComponent();

	virtual void Initialize();
	virtual void Update() override;

	void SetPlayOnce(bool enable);
	void SetCurrentFrame(int frame);
	void SetMaxFrames(int maxX, int maxY = 1);
	//set INITIAL offset (in pixels) in x and y directions
	void SetInitTexOffset(const Vector2& offset) { m_InitOffset = offset; }
	//get INITIAL offset (in pixels) in x and y directions
	Vector2 GetInitTexOffset() const { return m_InitOffset; }
	//set time per frame
	void SetTickRate(float tickRate);
	void SetSpriteLayout(SpriteLayout layout);
	//set current frame and current tick to 0
	void Reset();

protected:
	bool m_PlayOnce;
	SpriteLayout m_Layout;
	int m_CurrentFrame, m_MaxXFrames, m_MaxYFrames;
	float m_Tick, m_TickRate;
	Vector2 m_InitOffset;
};