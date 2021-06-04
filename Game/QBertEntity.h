#pragma once
#include "Component.h"

class QBertLevel;
class Subject;
class QBertEntity : public Component
{
public:
	QBertEntity();
	virtual ~QBertEntity();

	Subject* GetSubject() const { return m_pSubject; }

protected:
	Subject* m_pSubject;
	friend class QBertLevel;
	static QBertLevel* m_pLevel;
};