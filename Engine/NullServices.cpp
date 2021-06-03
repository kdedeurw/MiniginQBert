#include "NullServices.h"
#include <iostream>

void NullSoundSystem::PlaySound(SoundId id, int volume)
{
	std::cout << "Sound id: " << id << " played at volume: " << volume << '\n';
}

void NullSoundSystem::StopSound(SoundId id)
{
	std::cout << "Sound id: " << id << " stopped\n";
}

void NullSoundSystem::StopAllSounds()
{
	std::cout << "All sounds stopped\n";
}

void NullSoundSystem::PauseAllSounds()
{
	std::cout << "All sounds paused\n";
}

void NullSoundSystem::ResumeAllSounds()
{
	std::cout << "All sounds resumed\n";
}