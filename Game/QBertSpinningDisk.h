#pragma once
#include "Component.h"

class SpriteComponent;
class Subject;
class QBertSpinningDisk : public Component
{
public:
	QBertSpinningDisk();
	~QBertSpinningDisk();

private:
	friend class QBertLevel;
	Subject* m_pSubject;
};