#include "pch.h"
#include "DebugSoundSystem.h"
#include <iostream>

SoundId DebugSoundSystem::AddSound(const char* path, int volume)
{
	std::cout << "Sound added with id: " << 0 << " and volume: " << volume << "\npath: " << path << '\n';
	return 0;
}

void DebugSoundSystem::PlaySound(SoundId id, int volume)
{
	std::cout << "Sound id: " << id << " played at volume: " << volume << '\n';
}

void DebugSoundSystem::StopSound(SoundId id)
{
	std::cout << "Sound id: " << id << " stopped\n";
}

void DebugSoundSystem::StopAllSounds()
{
	std::cout << "All sounds stopped\n";
}

void DebugSoundSystem::PauseAllSounds()
{
	std::cout << "All sounds paused\n";
}

void DebugSoundSystem::ResumeAllSounds()
{
	std::cout << "All sounds resumed\n";
}