#pragma once
#include "Texture2DComponent.h"

class SpriteComponent : public Texture2DComponent
{
public:
	SpriteComponent();
	virtual ~SpriteComponent();

	virtual void Initialize() override;
	//virtual void Render() const override;
	virtual void Update() override;

	void SetPlayOnce(bool enable);
	void SetCurrentFrame(int frame);
	void SetMaxFrames(int max);
	//set width and height of 1 frame (in pixels) -> srcRect.xy
	void SetSizes(const Vector2& sizes);
	//set time per frame
	void SetTickRate(float tickRate);
	//set offset in pixels in x and y directions
	void SetOffset(const Vector2& offset);
	//set current frame and current tick to 0
	void Reset();

protected:
	bool m_PlayOnce;
	int m_Frame;
	int m_MaxFrames;
	float m_Tick;
	float m_TickRate;
	Vector2 m_Offset;
};