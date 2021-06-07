#include "pch.h"
#include "QBertTile.h"
#include "Subject.h"
#include "QBertEvents.h"
#include "QBertCharacter.h"
#include "QBertSpinningDisk.h"

int QBertTile::m_TextureId = 0;
const float QBertTile::m_TextureSize = 32.f;
Vector2 QBertTile::m_TextureOffset{ 80.f, 160.f };

QBertTile::QBertTile()
	: m_HasBeenIntermediateState{}
	, m_HasBeenTargetState{}
	, m_State{ TileState::DefaultState }
	, m_pTexture{}
{
}

QBertTile::~QBertTile()
{
	m_pTexture = nullptr;
}

void QBertTile::Initialize(bool forceInitialize)
{
	if (!forceInitialize && m_IsInitialized)
		return;

	if (!m_pTexture)
		m_pTexture = GetGameObject()->GetComponent<Texture2DComponent>();

	m_pTexture->SetTexture("QBert/Sprites.png");
	m_pTexture->SetTextureOffset(m_TextureOffset);
	m_pTexture->SetTextureSize({ m_TextureSize, m_TextureSize });

	m_IsInitialized = true;
}

void QBertTile::Render() const
{
}

void QBertTile::Update()
{
}

void QBertTile::Reset()
{
	m_State = TileState::DefaultState;
	m_HasBeenIntermediateState = false;
	m_HasBeenTargetState = false;
	LeaveCharacter();
	//update texture offset
	Vector4& srcRct = m_pTexture->GetTexture2D()->GetSourceRect();
	srcRct.x = m_TextureOffset.x * m_TextureId;
	srcRct.y = m_TextureOffset.y;
}

void QBertTile::AddSpinningDisk(QBertSpinningDisk* pDisk, bool isLeft)
{
	if (isLeft)
	{
		if (!m_Neighbours.pLeftTopNeighbour)
		{
			m_Neighbours.pLeftTopNeighbour = pDisk;
			pDisk->Connect(this, isLeft);
			return;
		}
	}
	else
	{
		if (!m_Neighbours.pRightTopNeighbour)
		{
			m_Neighbours.pRightTopNeighbour = pDisk;
			pDisk->Connect(this, isLeft);
			return;
		}
	}
	std::cout << "Tile has no free top neighbours!\n";
}

void QBertTile::RemoveSpinningDisk(bool isLeft)
{
	if (isLeft)
		m_Neighbours.pLeftTopNeighbour = nullptr;
	else
		m_Neighbours.pRightTopNeighbour = nullptr;
}

void QBertTile::SetState(TileState state)
{
	m_State = state;

	//update texture offset
	Vector4& srcRct = m_pTexture->GetTexture2D()->GetSourceRect();
	srcRct.y = m_TextureOffset.y + m_TextureSize * (int)m_State;
	 
	switch (m_State)
	{
	case TileState::IntermediateState:
		if (!m_HasBeenIntermediateState)
		{
			m_pSubject->Notify(GetGameObject(), (int)QBertEvent::event_tile_colour_change_intermediate);
			m_HasBeenIntermediateState = true;
		}
		break;
	case TileState::TargetState:
		if (!m_HasBeenTargetState)
		{
			m_pSubject->Notify(GetGameObject(), (int)QBertEvent::event_tile_colour_change_final);
			m_HasBeenTargetState = true;
		}
		break;
	}
}

bool QBertTile::TryEnter(QBertCharacter* pNewCharacter)
{
	return EvaluateCharacterConflict(pNewCharacter);
}

bool QBertTile::EvaluateCharacterConflict(QBertCharacter* pNewCharacter)
{
	if (!m_pCurrentCharacter)
	{
		m_pCurrentCharacter = pNewCharacter;
		return true;
	}

	const QBertCharacterType currentType = m_pCurrentCharacter->GetType();
	const QBertCharacterType newType = pNewCharacter->GetType();
	switch (currentType)
	{
	case QBertCharacterType::QBert:
	{
		switch (newType)
		{
			case QBertCharacterType::RedBall:
			case QBertCharacterType::Coily:
			case QBertCharacterType::UggWrongway:
			{
				//kill QBertPlayer
				m_pCurrentCharacter->Kill(false);
				//replace tile's current character (qbert) with enemy
				m_pCurrentCharacter = pNewCharacter;
				return true;
			}
			case QBertCharacterType::GreenBall:
			case QBertCharacterType::SlickSam:
			{
				//kill slicksam jumping on QBertPlayer
				pNewCharacter->Kill(false);
				return false;
			}
		}
	break;
	}
	case QBertCharacterType::RedBall:
	case QBertCharacterType::Coily:
	case QBertCharacterType::UggWrongway:
	{
		if (newType == QBertCharacterType::QBert)
		{
			//kill QBertPlayer jumping on enemy
			pNewCharacter->Kill(false);
			return true;
		}
		break;
	}
	case QBertCharacterType::GreenBall:
	case QBertCharacterType::SlickSam:
	{
		if (newType == QBertCharacterType::QBert)
		{
			//kill greenball/slicksam
			m_pCurrentCharacter->Kill(false);
			//replace with QBertPlayer
			m_pCurrentCharacter = pNewCharacter;
			return true;
		}
		break;
	}
	}
	return false;
}