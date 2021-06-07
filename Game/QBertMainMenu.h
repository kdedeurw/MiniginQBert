#pragma once
#include <Component.h>

class QBertMainMenu : public Component
{
public:
	QBertMainMenu();
	~QBertMainMenu();

	virtual void Initialize(bool forceInitialize = false) override;
	virtual void Render() const override;
	virtual void Update() override;

private:
};