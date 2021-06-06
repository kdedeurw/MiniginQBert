#pragma once
#include "Component.h"

enum class QBertCharacterType
{
	QBert,
	RedBall,
	Coily,
	UggWrongway,
	GreenBall,
	SlickSam,
};

class QBertLevel;
class Subject;
class QBertCharacterMovement;
class QBertCharacter : public Component
{
public:
	QBertCharacter();
	virtual ~QBertCharacter();
	
	virtual void Initialize() override;

	virtual void Kill(bool hasFallen) = 0;

	QBertLevel* GetLevel() const { return m_pLevel; }
	QBertCharacterMovement* GetMovement() const { return m_pMovement; }
	Subject* GetSubject() const { return m_pSubject; }
	virtual const QBertCharacterType GetType() const = 0;

protected:
	friend class QBertCharacterMovement;
	virtual void HasMoved() {};
	virtual void HasLanded() {};

private:
	Subject* m_pSubject;
	QBertCharacterMovement* m_pMovement;
	friend class QBertLevel;
	static QBertLevel* m_pLevel;
};