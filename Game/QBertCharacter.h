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

enum class QBertTileAlteration
{
	None,
	Next,
	Previous,
};

class QBertLevel;
class Subject;
class QBertCharacterMovement;
class QBertCharacter : public Component
{
public:
	QBertCharacter();
	virtual ~QBertCharacter();
	
	virtual void Initialize(bool forceInitialize = false) override;

	virtual void Kill(bool hasFallen) = 0;

	QBertLevel* GetLevel() const { return m_pLevel; }
	QBertCharacterMovement* GetMovement() const { return m_pMovement; }
	Subject* GetSubject() const { return m_pSubject; }
	virtual const QBertCharacterType GetType() const = 0;
	virtual const QBertTileAlteration GetTileAlteration() const { return QBertTileAlteration::None; }

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