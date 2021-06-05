#include "pch.h"
#include "QBertGameObserver.h"
#include "QBertEvents.h"
#include "QBertPlayer.h"
#include "QBertTile.h"

QBertGameObserver::QBertGameObserver()
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
		QBertPlayer* pPlayer = pGameObject->GetComponent<QBertPlayer>();
		if (pPlayer)
		{
			if (pPlayer->GetId() == PlayerId::Player1)
			{
				m_Player1Stats.Score += 100;
				std::cout << "Player 1 caught a green ball, score: " << m_Player1Stats.Score << '\n';
			}
			else
			{
				m_Player2Stats.Score += 100;
				std::cout << "Player 2 caught a green ball, score: " << m_Player2Stats.Score << '\n';
			}
		}
		break;
	}
	case QBertEvent::event_coily_killed:
	{
		QBertPlayer* pPlayer = pGameObject->GetComponent<QBertPlayer>();
		if (pPlayer)
		{
			if (pPlayer->GetId() == PlayerId::Player1)
			{
				m_Player1Stats.Score += 500;
				std::cout << "Player 1 killed a coily, score: " << m_Player1Stats.Score << '\n';
			}
			else
			{
				m_Player2Stats.Score += 500;
				std::cout << "Player 2 killed a coily, score: " << m_Player2Stats.Score << '\n';
			}
		}
		break;
	}
	case QBertEvent::event_slick_catched:
	{
		QBertPlayer* pPlayer = pGameObject->GetComponent<QBertPlayer>();
		if (pPlayer)
		{
			if (pPlayer->GetId() == PlayerId::Player1)
			{
				m_Player1Stats.Score += 300;
				std::cout << "Player 1 caught a slick, score: " << m_Player1Stats.Score << '\n';
			}
			else
			{
				m_Player2Stats.Score += 500;
				std::cout << "Player 2 caught a slick, score: " << m_Player2Stats.Score << '\n';
			}
		}
		break;
	}
	case QBertEvent::event_sam_catched:
	{
		QBertPlayer* pPlayer = pGameObject->GetComponent<QBertPlayer>();
		if (pPlayer)
		{
			if (pPlayer->GetId() == PlayerId::Player1)
			{
				m_Player1Stats.Score += 300;
				std::cout << "Player 1 caught a sam, score: " << m_Player1Stats.Score << '\n';
			}
			else
			{
				m_Player2Stats.Score += 500;
				std::cout << "Player 2 caught a sam, score: " << m_Player2Stats.Score << '\n';
			}
		}
		break;
	}
	case QBertEvent::event_round_complete:
	{
		m_Player1Stats.Score += 1000;
		m_Player2Stats.Score += 1000;
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
	default:
		std::cout << "Wrong/unimplemented event\n";
		break;
	}
}

const PlayerStats& QBertGameObserver::GetStats(PlayerId id)
{
	if (id == PlayerId::Player1)
		return m_Player1Stats;
	return m_Player2Stats;
}
