#pragma once
#include "BaseSoundSystem.h"

class DebugSoundSystem;
class SoundSystem;
class ServiceLocator
{
public:

	static void RegisterSoundSystem(SoundSystem* pSoundSystem);
	static BaseSoundSystem* GetSoundSystem(bool isDebug = false);

private:
	static BaseSoundSystem m_NullSoundSystem;
	static DebugSoundSystem m_DebugSoundSystem;
	static SoundSystem* m_pSoundSystem;

	ServiceLocator() = delete; //prohibits creation of this object
};