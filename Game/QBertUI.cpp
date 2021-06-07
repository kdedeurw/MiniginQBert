#include "pch.h"
#include "QBertUI.h"
#include "QBertGameObserver.h"

QBertUI::QBertUI()
	: m_pScoreTextComponent{}
	, m_pLivesTextComponent{}
	, m_pObserver{}
{
}

QBertUI::~QBertUI()
{
	m_pObserver = nullptr;
	m_pScoreTextComponent = nullptr;
	m_pLivesTextComponent = nullptr;
}

void QBertUI::Initialize(bool forceInitialize)
{
	forceInitialize;
}

void QBertUI::Render() const
{
}

void QBertUI::Update()
{
	if (!m_pObserver)
		return;

	const auto& stats = m_pObserver->GetStats(PlayerId::Player1);
	if (m_pScoreTextComponent)
		m_pScoreTextComponent->SetText("Score: " + std::to_string(stats.Score));
	if (m_pLivesTextComponent)
		m_pLivesTextComponent->SetText("Lives: " + std::to_string(stats.Lives));
}

void QBertUI::SetScoreText(TextComponent* pText)
{
	m_pScoreTextComponent = pText;
	pText->SetOffset({ 75.f, 350.f });
}

void QBertUI::SetLivesText(TextComponent* pText)
{
	m_pLivesTextComponent = pText;
	pText->SetOffset({ 75.f, 400.f });
}