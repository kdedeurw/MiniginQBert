#include "pch.h"
#include "SoundTester.h"
#include "GameState.h"
#include "ServiceLocator.h"
#include <iostream>

SoundTester::SoundTester()
	: m_pSoundSystem{}
	, m_IsPressed{}
	, m_SoundId{ UINT32_MAX }
	, m_IsDebugSound{}
{}

SoundTester::~SoundTester()
{
	m_pSoundSystem = nullptr;
}

void SoundTester::Initialize(bool forceInitialize)
{
	if (!forceInitialize && m_IsInitialized)
		return;

	if (!m_pSoundSystem)
		m_pSoundSystem = ServiceLocator::GetSoundSystem(m_IsDebugSound);

	if (m_pSoundSystem)
		m_SoundId = m_pSoundSystem->AddSound("../Resources/Audio/Qbert_sound_effect.wav");

	if (m_SoundId == UINT32_MAX)
		std::cout << "Sound creation failed!\n";

	m_IsInitialized = true;
}

void SoundTester::Update()
{
	KeyboardMouseListener& kbml = GameState::GetInstance().KeyboardMouseListener;

	//yet another dirty implementation
	if (!m_IsPressed && kbml.IsPressed(Key::One))
	{
		m_IsPressed = true;
		m_pSoundSystem->PlaySound(0, 10);
	}
	else if (m_IsPressed && kbml.IsReleased(Key::One))
		m_IsPressed = false;
}

void SoundTester::Render() const
{}

void SoundTester::SetDebugSoundInit(bool isDebugSound)
{
	m_IsDebugSound = isDebugSound;
}