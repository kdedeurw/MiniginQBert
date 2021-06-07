#pragma once
#include <Component.h>

class TextComponent;
class QBertGameObserver;
class QBertUI : public Component
{
public:
	QBertUI();
	virtual ~QBertUI();

	virtual void Initialize(bool forceInitialize = false) override;
	virtual void Render() const override;
	virtual void Update() override;

	void SetScoreText(TextComponent* pText);
	void SetLivesText(TextComponent* pText);

	void SetObserver(QBertGameObserver* pObserver) { m_pObserver = pObserver; }

private:
	TextComponent* m_pScoreTextComponent;
	TextComponent* m_pLivesTextComponent;
	QBertGameObserver* m_pObserver;
};