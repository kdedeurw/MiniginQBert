#pragma once
#include "BaseSoundSystem.h"
#include <thread>
#include "ThreadSafeQueue.h"
#include <map>

struct Mix_Chunk;
class SoundSystem final : public BaseSoundSystem
{
public:
	SoundSystem();
	~SoundSystem();
	SoundSystem(const SoundSystem & other) = delete;
	SoundSystem(SoundSystem && other) noexcept = delete;
	SoundSystem& operator=(const SoundSystem & other) = delete;
	SoundSystem& operator=(SoundSystem && other) noexcept = delete;

	SoundId AddSound(const char* path, int volume = 1) override;

	void PlaySound(SoundId id, int volume = 1) override;
	void StopSound(SoundId id) override;
	void StopAllSounds() override;
	void PauseAllSounds() override;
	void ResumeAllSounds() override;

private:
	//TODO: make singleton? or protect/friend Ctor from public
	bool m_IsContinue;
	std::thread m_Thread;
	ThreadSafeQueue<SoundRequest> m_SoundQueue;
	std::vector<Mix_Chunk*> m_Sounds;

	void ProcessAudioQueue();
};