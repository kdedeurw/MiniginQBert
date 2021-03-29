#include "ServiceLocator.h"
#include "DebugSoundSystem.h"
#include "SoundSystem.h"

BaseSoundSystem ServiceLocator::m_NullSoundSystem{};
DebugSoundSystem ServiceLocator::m_DebugSoundSystem{};
SoundSystem* ServiceLocator::m_pSoundSystem{};

void ServiceLocator::RegisterSoundSystem(SoundSystem* pSoundSystem)
{
	m_pSoundSystem = pSoundSystem;
}

BaseSoundSystem* ServiceLocator::GetSoundSystem(bool isDebug)
{
	if (isDebug)
		return &m_DebugSoundSystem;

	if (m_pSoundSystem)
		return m_pSoundSystem;

	return &m_NullSoundSystem;
}