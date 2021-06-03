#pragma once
#include "Observer.h"
#include "QBertEvents.h"

struct PlayerStats
{
    int Lives = 3;
    int Score = 0;
};

class QBertGameObserver final : public Observer
{
public:
    QBertGameObserver();
    ~QBertGameObserver();
    void OnNotify(GameObject* pGameObject, int event) override;

private:
    PlayerStats m_Player1Stats;
    PlayerStats m_Player2Stats;

};