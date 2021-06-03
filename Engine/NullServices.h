#pragma once
#include <stdint.h>

using SoundId = unsigned int;
struct SoundRequest
{
	SoundRequest(SoundId id = UINT32_MAX, int vol = -1)
		: soundId{ id }
		, volume{ vol }
	{};

	SoundId soundId;
	int volume;
};

class NullSoundSystem
{
public:
	virtual ~NullSoundSystem() = default;
	virtual void PlaySound(SoundId id, int volume = 1);
	virtual void StopSound(SoundId id);
	virtual void StopAllSounds();
	virtual void PauseAllSounds();
	virtual void ResumeAllSounds();

private:
};