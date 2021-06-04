#include "pch.h"
#include "SoundSystem.h"
#include <SDL_mixer.h>

SoundSystem::SoundSystem()
	: m_IsContinue{ true }
	, m_SoundQueue{}
	, m_Thread{}
	, m_Sounds{}
{
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		std::cerr << "Core::Initialize( ), error when calling Mix_OpenAudio: " << Mix_GetError() << std::endl;

	m_Thread = std::thread{ &SoundSystem::ProcessAudioQueue, this };
}

SoundSystem::~SoundSystem()
{
	m_IsContinue = false; //kill loop inside thread, not thread safe, but will never crash
	m_Thread.join(); //wait for thread's loop to finish
	//only after deconstructor the queue will be deleted, so thread safety guarantueed (even no data races)

	for (Mix_Chunk* pMixChunk : m_Sounds)
		Mix_FreeChunk(pMixChunk);
	m_Sounds.clear();

	Mix_Quit();
}

SoundId SoundSystem::AddSound(const char* path, int volume)
{
	Mix_Chunk* pMixChunk = Mix_LoadWAV(path);
	pMixChunk->volume = (Uint8)volume;
	m_Sounds.push_back(pMixChunk);
	return (SoundId)m_Sounds.size() - 1;
}

void SoundSystem::PlaySound(SoundId id, int volume)
{
	SoundRequest request;
	request.soundId = id;
	request.volume = volume;
	m_SoundQueue.Push(request);
}

void SoundSystem::StopSound(SoundId id)
{
	//TODO: work around with sounds (ids) and channels
	id;
	Mix_Pause(-1);
}

void SoundSystem::PauseAllSounds()
{
	Mix_Pause(-1);
}

void SoundSystem::ResumeAllSounds()
{
	Mix_Resume(-1);
}

void SoundSystem::StopAllSounds()
{
	Mix_HaltChannel(-1);
}

void SoundSystem::ProcessAudioQueue()
{
	while (m_IsContinue)
	{
		//UPDATE existing audio
		//handled by SDL mixer

		if (m_SoundQueue.IsEmpty())
			continue;

		//HANDLE new audio
		const SoundRequest& request = m_SoundQueue.Pop();

		Mix_Chunk* pMixChunk = m_Sounds[request.soundId];

		//should be a valid mixchunk
		//if (!pMixChunk)
		//	continue;

		Mix_VolumeChunk(pMixChunk, request.volume);

		//PLAY new audio
		int channel = Mix_PlayChannel(-1, pMixChunk, 0);

		if (channel == -1)
			std::cout << "Sound unable to play: channel is probably full\n(more than 8 sounds at once per channel)\n";

		//std::this_thread::yield;
	}
}