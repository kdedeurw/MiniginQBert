#include "pch.h"
#include "QBertSpinningDisk.h"
#include "Subject.h"
#include "QBertCharacter.h"
#include "QBertLevel.h"
#include "QBertEvents.h"
#include "QBertTile.h"
#include "QBertPlayer.h"
#include "QBertCharacterMovement.h"

const float QBertSpinningDisk::m_TextureSize = 16.f;

QBertSpinningDisk::QBertSpinningDisk()
	: m_IsOnLeftSide{}
	, m_pSprite{}
{
}

QBertSpinningDisk::~QBertSpinningDisk()
{
	m_pSprite = nullptr;
}

void QBertSpinningDisk::Initialize(bool forceInitialize)
{
	if (!forceInitialize && m_IsInitialized)
		return;

	if (!m_pSprite)
	{
		m_pSprite = GetGameObject()->GetComponent<SpriteComponent>();

		m_pSprite->SetTexture("QBert/Sprites.png");
		m_pSprite->SetInitTexOffset({ 0.f, 352.f });
		m_pSprite->SetTextureSize({ m_TextureSize, m_TextureSize });
		m_pSprite->SetPlayOnce(false);
		m_pSprite->SetTickRate(0.25f);

		m_pSprite->SetSpriteLayout(SpriteLayout::Horizontal);
		const int frames = 4;
		m_pSprite->SetMaxFrames(frames, 0);
	}

	m_IsInitialized = true;
}

void QBertSpinningDisk::Update()
{
}

void QBertSpinningDisk::Kill()
{
	Disconnect();
}

bool QBertSpinningDisk::TryEnter(QBertCharacter* pNewCharacter)
{
	if (m_pCurrentCharacter)
		return false;

	QBertPlayer* pPlayer = dynamic_cast<QBertPlayer*>(pNewCharacter);
	if (pPlayer)
	{
		m_pCurrentCharacter = pNewCharacter;
		pPlayer->UseSpinningDisk(this);
		m_pSubject->Notify(GetGameObject(), QBertEvent::event_spinning_pad_used);
		return true;
	}
	return false;
}

void QBertSpinningDisk::Connect(QBertBaseTile* pTile, bool isLeft)
{
	if (m_Neighbours.pLeftBottomNeighbour || m_Neighbours.pRightBottomNeighbour)
		return;

	if (isLeft)
		m_Neighbours.pRightBottomNeighbour = pTile;
	else
		m_Neighbours.pLeftBottomNeighbour = pTile;

	m_IsOnLeftSide = isLeft;
}

void QBertSpinningDisk::Disconnect()
{
	if (m_IsOnLeftSide)
	{
		dynamic_cast<QBertTile*>(m_Neighbours.pRightBottomNeighbour)->RemoveSpinningDisk(m_IsOnLeftSide);
		m_Neighbours.pRightBottomNeighbour = nullptr;
	}
	else
	{
		dynamic_cast<QBertTile*>(m_Neighbours.pLeftBottomNeighbour)->RemoveSpinningDisk(m_IsOnLeftSide);
		m_Neighbours.pLeftBottomNeighbour = nullptr;
	}
}