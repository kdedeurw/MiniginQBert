#include "pch.h"
#include "QBertTile.h"
#include "GameObject.h"
#include "Components.h"
#include "Scene.h"
#include "Subject.h"
#include "QBertEvents.h"
#include "Texture2D.h"
#include "GameState.h"

const float QBertTile::m_TextureSize = 32.f;

QBertTile::QBertTile()
	: m_State{ State::DefaultState }
	, m_pTexture{}
	, m_InitOffset{}
{
}

QBertTile::~QBertTile()
{
	m_pTexture = nullptr;
}

void QBertTile::Initialize()
{
	if (!m_pTexture)
		m_pTexture = m_pGameObject->GetComponent<Texture2DComponent>();

	m_pTexture->SetTexture("QBert/Sprites.png");
	m_InitOffset = { 0.f, 160.f };
	m_pTexture->SetTextureOffset(m_InitOffset);
	m_pTexture->SetTextureSize({ m_TextureSize, m_TextureSize });
}

void QBertTile::Render() const
{
}

void QBertTile::Update()
{
}

void QBertTile::UpdateState(State state)
{
	m_State = state;

	//update texture offset
	Vector4& srcRct = m_pTexture->GetTexture2D()->GetSourceRect();
	srcRct.y = m_InitOffset.y + m_TextureSize * (int)m_State;
}