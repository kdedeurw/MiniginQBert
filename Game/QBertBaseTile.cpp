#include "pch.h"
#include "QBertBaseTile.h"

QBertBaseTile::QBertBaseTile()
	: m_Id{ -1 }
	, m_pCurrentCharacter{}
	, m_pSubject{ new Subject{} }
	, m_pLevel{}
	, m_Neighbours{}
{}

QBertBaseTile::~QBertBaseTile()
{
	m_pCurrentCharacter = nullptr;
	std::memset(&m_Neighbours, 0, sizeof(Neighbours));
	if (m_pSubject)
		delete m_pSubject;
	m_pSubject = nullptr;
	m_pLevel = nullptr;
}

void QBertBaseTile::LeaveCharacter()
{
	m_pCurrentCharacter = nullptr;
}