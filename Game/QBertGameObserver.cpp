#include "pch.h"
#include "QBertGameObserver.h"
#include "QBertEvents.h"
#include <iostream>
#include "GameObject.h"
#include "QBertPlayer.h"

void QBertGameObserver::OnNotify(GameObject* pGameObject, int event)
{
	QbertEvent e = (QbertEvent)event;
	QBertPlayer* pPlayer = pGameObject->GetComponent<QBertPlayer>();
	switch (e)
	{
	case QbertEvent::event_player_die:
		std::cout << "PlayerDied\n";
		if (pPlayer)
			pPlayer->OnDeath(); //notifies the gameobject responsible for UI
		break;
	case QbertEvent::event_colour_change:
		std::cout << "Colour changed!\n";
		if (pPlayer)
			pPlayer->AddScore(25);
		break;
	case QbertEvent::event_enemy_coily_killed:
		std::cout << "Coily killed!\n";
		if (pPlayer)
			pPlayer->AddScore(500);
		break;
	case QbertEvent::event_slick_catched:
		std::cout << "Slick catched!\n";
		if (pPlayer)
			pPlayer->AddScore(300);
		break;
	case QbertEvent::event_sam_catched:
		std::cout << "Sam catched!\n";
		if (pPlayer)
			pPlayer->AddScore(300);
		break;
	default:
		std::cout << "Wrong event\n";
		break;
	}
}