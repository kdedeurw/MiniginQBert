#include "pch.h"
#include "QBertGameObserver.h"
#include "QBertEvents.h"
#include <iostream>
#include "GameObject.h"
#include "QBertPlayer.h"

QBertGameObserver::QBertGameObserver()
{
}

QBertGameObserver::~QBertGameObserver()
{
}

void QBertGameObserver::OnNotify(GameObject* pGameObject, int event)
{
	QBertPlayer* pPlayer = pGameObject->GetComponent<QBertPlayer>();
	switch ((QBertEvent)event)
	{
	case QBertEvent::event_player1_die:
		if (pPlayer)
			pPlayer->OnDeath();
		break;
	case QBertEvent::event_player2_die:
		break;
	case QBertEvent::event_tile_colour_change_intermediate:
		break;
	case QBertEvent::event_tile_colour_change_final:
		break;
	case QBertEvent::event_green_ball_catched:
		break;
	case QBertEvent::event_coily_killed:
		break;
	case QBertEvent::event_slick_catched:
		break;
	case QBertEvent::event_sam_catched:
		break;
	default:
		break;
	}
}