#pragma once
#include "QBertCharacter.h"

class QBertRedBall : public QBertCharacter
{
public:
	QBertRedBall();
	virtual ~QBertRedBall();

	virtual void Initialize() override;
	virtual void Render() const override;
	virtual void Update() override;

	virtual void Kill(bool hasFallen) override;
	virtual const QBertCharacterType GetType() const override { return QBertCharacterType::RedBall; }

private:


};