#include "pch.h"
#include "QBertEntity.h"
#include "Subject.h"

QBertLevel* QBertEntity::m_pLevel{};

QBertEntity::QBertEntity()
	: m_pSubject{}
{}

QBertEntity::~QBertEntity()
{
	if (m_pSubject)
		delete m_pSubject;
	m_pSubject = nullptr;
}

void QBertEntity::SetSubject(Subject* pSubject, bool isOverwrite)
{
	if (!m_pSubject || isOverwrite)
		m_pSubject = pSubject;
}