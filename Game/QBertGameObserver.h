#pragma once
#include "Observer.h"

//observer doesn't own/control UI elements
class QBertGameObserver final : public Observer
{
public:
    QBertGameObserver() = default;
    ~QBertGameObserver() = default;
    void OnNotify(GameObject* pGameObject, int event) override;

private:
    //Achievements/Stats
};