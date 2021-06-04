#pragma once
#include "QBertEntity.h"

class QBertTile;
class QBertPlayer final : public QBertEntity
{
public:
	QBertPlayer();
	~QBertPlayer();

	void Initialize() override;
	void Render() const override;
	void Update() override;

	bool IsMoving() const { return m_CurrentMoveDelay > 0.f; }
	void Respawn();

	static float GetTextureSize() { return m_TextureSize; }

private:
	bool m_IsKilled, m_IsOnTile;
	float m_CurrentMoveDelay;
	const float m_MoveDelay = 0.5f;
	PlayerId m_PlayerId;
	Texture2DComponent* m_pTexture;
	QBertTile* m_pCurrentTile;
	Vector2 m_FormerPos;
	Vector2 m_DesiredPos;

	void HandleMove();
	void OnDeath();
	void LandOnTile(QBertTile* pTile);

	static const float m_TextureSize;
};