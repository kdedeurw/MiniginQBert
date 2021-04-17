#pragma once
#include "BaseSoundSystem.h"

class DebugSoundSystem : public BaseSoundSystem
{
public:
	virtual ~DebugSoundSystem() = default;

	virtual SoundId AddSound(const char* path, int volume = 1);

	virtual void PlaySound(SoundId id, int volume = 1) override;
	virtual void StopSound(SoundId id)override;
	virtual void StopAllSounds() override;
	virtual void PauseAllSounds() override;
	virtual void ResumeAllSounds() override;

private:
};