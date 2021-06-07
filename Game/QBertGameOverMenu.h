#pragma once
#include <Component.h>

class QBertGameObserver;
class QBertLevel;
class QBertGameOverMenu : public Component
{
public:
	QBertGameOverMenu();
	~QBertGameOverMenu();

	virtual void Initialize(bool forceInitialize = false) override;
	virtual void Render() const override;
	virtual void Update() override;

	void SetObserver(QBertGameObserver* pObserver) { m_pObserver = pObserver; }
	void SetLevel(QBertLevel* pLevel) { m_pLevel = pLevel; }

private:
	QBertGameObserver* m_pObserver;
	QBertLevel* m_pLevel;
};