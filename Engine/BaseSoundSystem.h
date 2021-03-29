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

class BaseSoundSystem
{
public:
	virtual ~BaseSoundSystem() = default;

	virtual SoundId AddSound(const char* path, int volume = 1) { path; volume; return 0; };

	virtual void PlaySound(SoundId id, int vol = 1) { id; vol; };
	virtual void StopSound(SoundId id) { id; };
	virtual void StopAllSounds() {};
	virtual void PauseAllSounds() {};
	virtual void ResumeAllSounds() {};

};