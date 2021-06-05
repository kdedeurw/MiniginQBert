#include "pch.h"
#include "QBertTile.h"
#include "Subject.h"
#include "QBertEvents.h"
#include "QBertCharacter.h"

int QBertTile::m_TextureId = 0;
const float QBertTile::m_TextureSize = 32.f;
Vector2 QBertTile::m_TextureOffset{ 80.f, 160.f };

QBertTile::QBertTile()
	: m_HasBeenIntermediateState{}
	, m_HasBeenTargetState{}
	, m_Id{ -1 }
	, m_State{ TileState::DefaultState }
	, m_pTexture{}
	, m_pCurrentCharacter{}
	, m_pSubject{ new Subject{} }
	, m_Neighbours{}
{
}

QBertTile::~QBertTile()
{
	m_pTexture = nullptr;
	m_pCurrentCharacter = nullptr;
	std::memset(&m_Neighbours, 0, sizeof(Neighbours));
	if (m_pSubject)
		delete m_pSubject;
	m_pSubject = nullptr;
}

void QBertTile::Initialize()
{
	if (!m_pTexture)
		m_pTexture = m_pGameObject->GetComponent<Texture2DComponent>();

	m_pTexture->SetTexture("QBert/Sprites.png");
	m_pTexture->SetTextureOffset(m_TextureOffset);
	m_pTexture->SetTextureSize({ m_TextureSize, m_TextureSize });
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
	m_pCurrentCharacter = nullptr;
	//update texture offset
	Vector4& srcRct = m_pTexture->GetTexture2D()->GetSourceRect();
	srcRct.x = m_TextureOffset.x * m_TextureId;
	srcRct.y = m_TextureOffset.y;
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
			m_pSubject->Notify(m_pGameObject, (int)QBertEvent::event_tile_colour_change_intermediate);
			m_HasBeenIntermediateState = true;
		}
		break;
	case TileState::TargetState:
		if (!m_HasBeenTargetState)
		{
			m_pSubject->Notify(m_pGameObject, (int)QBertEvent::event_tile_colour_change_final);
			m_HasBeenTargetState = true;
		}
		break;
	}
}

void QBertTile::EnterCharacter(QBertCharacter* pNewCharacter)
{
	if (!m_pCurrentCharacter)
		m_pCurrentCharacter = pNewCharacter;
	else
		EvaluateCharacterConflict(pNewCharacter);
}

void QBertTile::LeaveCharacter()
{
	m_pCurrentCharacter = nullptr;
}

void QBertTile::EvaluateCharacterConflict(QBertCharacter* pNewCharacter)
{
	if (!m_pCurrentCharacter)
	{
		m_pCurrentCharacter = pNewCharacter;
		return;
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
				m_pCurrentCharacter->Kill(false);
				m_pCurrentCharacter = pNewCharacter;
				break;
			}
			case QBertCharacterType::GreenBall:
			case QBertCharacterType::SlickSam:
			{
				pNewCharacter->Kill(false);
				break;
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
			pNewCharacter->Kill(false);
		}
		break;
	}
	case QBertCharacterType::GreenBall:
	case QBertCharacterType::SlickSam:
	{
		if (newType == QBertCharacterType::QBert)
		{
			m_pCurrentCharacter->Kill(false);
			m_pCurrentCharacter = pNewCharacter;
		}
		break;
	}
	}
}