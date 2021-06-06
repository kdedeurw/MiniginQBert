#pragma once
#include "Component.h"

class BaseSoundSystem;
class SoundTester : public Component
{
public:
	SoundTester();
	~SoundTester();

	virtual void Initialize(bool forceInitialize = false) override;
	virtual void Update() override;
	virtual void Render() const override;

	void SetDebugSoundInit(bool isDebugSound = true);

private:
	bool m_IsPressed;
	bool m_IsDebugSound;
	unsigned int m_SoundId;
	BaseSoundSystem* m_pSoundSystem;
};