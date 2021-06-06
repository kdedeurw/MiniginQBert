#include "pch.h"
#include "QBertCharacter.h"
#include "Subject.h"
#include "QBertCharacterMovement.h"

QBertLevel* QBertCharacter::m_pLevel{};

QBertCharacter::QBertCharacter()
	: m_pSubject{ new Subject{} }
	, m_pMovement{}
{}

QBertCharacter::~QBertCharacter()
{
	m_pMovement = nullptr;
	if (m_pSubject)
		delete m_pSubject;
	m_pSubject = nullptr;
}

void QBertCharacter::Initialize()
{
	if (!m_pMovement)
		m_pMovement = GetGameObject()->GetComponent<QBertCharacterMovement>();
}
