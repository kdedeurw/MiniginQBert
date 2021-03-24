#pragma once
#include "SingletonRef.h"

using SoundId = unsigned int;

class SoundSystem : public SingletonRef<SoundSystem>
{
public:

	void PlaySound(SoundId id, float volume = 1.f);

private:
	friend static SoundSystem& SingletonRef<SoundSystem>::GetInstance();
	SoundSystem() = default;

	~SoundSystem() = default;
	SoundSystem(const SoundSystem& other) = delete;
	SoundSystem(SoundSystem&& other) noexcept = delete;
	SoundSystem& operator=(const SoundSystem& other) = delete;
	SoundSystem& operator=(SoundSystem&& other) noexcept = delete;

};