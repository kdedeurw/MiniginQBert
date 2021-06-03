#pragma once
#include "Component.h"

class QBertLevel;
class Subject;
class QBertEntity : public Component
{
public:
	QBertEntity();
	virtual ~QBertEntity();

	void SetSubject(Subject* pSubject, bool isOverwrite = false);
	Subject* GetSubject() const { return m_pSubject; }

protected:
	Subject* m_pSubject;
	static QBertLevel* m_pLevel;
};