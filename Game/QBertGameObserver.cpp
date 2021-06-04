#include "pch.h"
#include "QBertGameObserver.h"
#include "QBertEvents.h"
#include "QBertPlayer.h"

QBertGameObserver::QBertGameObserver()
{
}

QBertGameObserver::~QBertGameObserver()
{
}

void QBertGameObserver::OnNotify(GameObject* pGameObject, int event)
{
	switch ((QBertEvent)event)
	{
	case QBertEvent::event_player1_die:
	{
		QBertPlayer* pPlayer = pGameObject->GetComponent<QBertPlayer>();
		if (pPlayer)
		{
			std::cout << "Player 1 died\n";

			//TODO: respawn player
			--m_Player1Stats.Lives;
			pPlayer->Respawn();
		}
		break;
	}
	case QBertEvent::event_player2_die:
		--m_Player2Stats.Lives;
		break;
	case QBertEvent::event_tile_colour_change_intermediate:
		m_Player1Stats.Score += 20;
		break;
	case QBertEvent::event_tile_colour_change_final:
		m_Player1Stats.Score += 30;
		break;
	case QBertEvent::event_green_ball_catched:
		m_Player1Stats.Score += 100;
		break;
	case QBertEvent::event_coily_killed:
		m_Player1Stats.Score += 500;
		break;
	case QBertEvent::event_slick_catched:
		m_Player1Stats.Score += 300;
		break;
	case QBertEvent::event_sam_catched:
		m_Player1Stats.Score += 300;
		break;
	default:
		break;
	}
}

const PlayerStats& QBertGameObserver::GetStats(PlayerId id)
{
	if (id == PlayerId::Player1)
		return m_Player1Stats;
	return m_Player2Stats;
}
