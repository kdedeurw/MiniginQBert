#include "pch.h"
#include "QBertPlayer.h"
#include <iostream>
#include "GlobalInput.h"
#include "GameObject.h"
#include "Subject.h"
#include "QBertEvents.h"
#include "TextComponent.h"

QBertPlayer::~QBertPlayer()
{
	if (m_pSubject)
		delete m_pSubject;
	m_pSubject = nullptr;
}

void QBertPlayer::Initialize()
{
	if (m_pLifesDisplay)
		m_pLifesDisplay->SetText("Player " + m_Name + "'s lifes: " + std::to_string(m_Stats.Lives));

	if (m_pScoreDisplay)
		m_pScoreDisplay->SetText("Player " + m_Name + "'s score: " + std::to_string(m_Stats.Score));
}

void QBertPlayer::Render() const
{
}

void QBertPlayer::Update()
{
	if (m_Stats.Lives <= 0)
		return;

	//dirty implementation
	KeyboardMouseListener& km = GlobalInput::GetInstance().GetKeyboardMouseListener();

	if (!m_IsKilled && km.IsPressed(m_KillButton))
	{
		m_IsKilled = true;
		m_pSubject->Notify(GetGameObject(), (int)QbertEvent::event_player_die);
	}
	else if (m_IsKilled && km.IsReleased(m_KillButton))
		OnRespawn();

	if (!m_HasScored && km.IsPressed(m_RandomPointsButton))
	{
		m_HasScored = true;
		m_pSubject->Notify(GetGameObject(), rand() % 4 + 1); //events are enum class ints
	}
	else if (m_HasScored && km.IsReleased(m_RandomPointsButton))
		OnScored();
}

void QBertPlayer::AddScore(int amount)
{
	m_Stats.Score += amount;

	if (!m_pScoreDisplay)
	{
		std::cout << "No Score display!\n";
		return;
	}

	m_pScoreDisplay->SetText("Player " + m_Name + "'s score: " + std::to_string(m_Stats.Score));
}

void QBertPlayer::OnDeath()
{
	--m_Stats.Lives;
	if (!m_pLifesDisplay)
	{
		std::cout << "No Lifes display!\n";
		return;
	}

	if (m_Stats.Lives > 0)
		m_pLifesDisplay->SetText("Player " + m_Name + "'s lifes: " + std::to_string(m_Stats.Lives));
	else
		m_pLifesDisplay->SetText("Player " + m_Name + " was defeated!");
}

void QBertPlayer::OnRespawn()
{
	std::cout << "Player " << m_Name << "'s " << "lives: " << m_Stats.Lives << '\n';
	m_IsKilled = false;
}

void QBertPlayer::OnScored()
{
	std::cout << "Player " << m_Name << "'s " << "score: " << m_Stats.Score << '\n';
	m_HasScored = false;
}

void QBertPlayer::SetKillButton(Key button)
{
	m_KillButton = button;
}

void QBertPlayer::SetName(const std::string& name)
{
	m_Name = name;
}

void QBertPlayer::SetRandomPointsButton(Key button)
{
	m_RandomPointsButton = button;
}

void QBertPlayer::SetSubject(Subject* pSubject, bool isOverwrite)
{
	if (!m_pSubject)
		m_pSubject = pSubject;
	else if (isOverwrite)
		m_pSubject = pSubject;
}