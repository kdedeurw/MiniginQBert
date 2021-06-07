#include "pch.h"
#include "QBertGameObserver.h"
#include "QBertEvents.h"
#include "QBertPlayer.h"
#include "QBertTile.h"

QBertGameObserver::QBertGameObserver()
	: m_Player1Stats{}
	, m_Player2Stats{}
{
}

QBertGameObserver::~QBertGameObserver()
{
}

void QBertGameObserver::OnNotify(GameObject* pGameObject, int event)
{
	switch (static_cast<QBertEvent>(event))
	{
	case QBertEvent::event_player_die:
	{
		QBertPlayer* pPlayer = pGameObject->GetComponent<QBertPlayer>();
		if (pPlayer)
		{
			if (pPlayer->GetId() == PlayerId::Player1)
			{
				--m_Player1Stats.Lives;
				m_Player1Stats.Score -= 100;
				std::cout << "Player 1 died, lives left: " << m_Player1Stats.Lives << '\n';
			}
			else
			{
				--m_Player2Stats.Lives;
				m_Player2Stats.Score -= 100;
				std::cout << "Player 2 died, lives left: " << m_Player2Stats.Lives << '\n';
			}
		}
		break;
	}
	case QBertEvent::event_tile_colour_change_intermediate:
	{
		QBertTile* pTile = pGameObject->GetComponent<QBertTile>();
		if (pTile)
		{
			QBertPlayer* pPlayer = dynamic_cast<QBertPlayer*>(pTile->GetCurrentCharacter());
			if (pPlayer)
			{
				if (pPlayer->GetId() == PlayerId::Player1)
					m_Player1Stats.Score += 20;
				else
					m_Player2Stats.Score += 20;
			}
		}
		else
			m_Player1Stats.Score += 20;
		break;
	}
	case QBertEvent::event_tile_colour_change_final:
	{
		QBertTile* pTile = pGameObject->GetComponent<QBertTile>();
		if (pTile)
		{
			QBertPlayer* pPlayer = dynamic_cast<QBertPlayer*>(pTile->GetCurrentCharacter());
			if (pPlayer)
			{
				if (pPlayer->GetId() == PlayerId::Player1)
					m_Player1Stats.Score += 30;
				else
					m_Player2Stats.Score += 30;
			}
		}
		else
			m_Player1Stats.Score += 30;
		break;
	}
	case QBertEvent::event_green_ball_catched:
	{
		m_Player1Stats.Score += 100;
		std::cout << "Player 1 caught a green ball, score: " << m_Player1Stats.Score << '\n';
		break;
	}
	case QBertEvent::event_coily_killed:
	{
		m_Player1Stats.Score += 500;
		std::cout << "Killed a coily, score: " << m_Player1Stats.Score << '\n';
		break;
	}
	case QBertEvent::event_slick_catched:
	{
		m_Player1Stats.Score += 300;
		std::cout << "Caught a slick, score: " << m_Player1Stats.Score << '\n';
		break;
	}
	case QBertEvent::event_sam_catched:
	{
		m_Player1Stats.Score += 300;
		std::cout << "Caught a sam, score: " << m_Player1Stats.Score << '\n';
		break;
	}
	case QBertEvent::event_round_complete:
	{
		m_Player1Stats.Score += 100;
		m_Player2Stats.Score += 100;
		std::cout << "Round complete event\n";
		break;
	}
	case QBertEvent::event_level_complete:
	{
		m_Player1Stats.Score += 1000;
		m_Player2Stats.Score += 1000;
		std::cout << "Level complete event\n";
		break;
	}
	case QBertEvent::event_spinning_pad_used:
	{
		std::cout << "Spinning pad used\n";
		break;
	}
	default:
		std::cout << "Wrong/unimplemented event\n";
		break;
	}
}

void QBertGameObserver::Reset()
{
	m_Player1Stats.Lives = 3;
	m_Player1Stats.Score = 0;
	m_Player2Stats.Lives = 3;
	m_Player2Stats.Score = 0;
}

PlayerStats& QBertGameObserver::GetStats(PlayerId id)
{
	if (id == PlayerId::Player1)
		return m_Player1Stats;
	return m_Player2Stats;
}
