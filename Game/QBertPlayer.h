#pragma once
#include "QBertEntity.h"
#include <string>

class Texture2DComponent;
enum PlayerId : int;
class QBertTile;
class QBertPlayer final : public QBertEntity
{
public:
	QBertPlayer();
	~QBertPlayer();

	void Initialize() override;
	void Render() const override;
	void Update() override;

	void Respawn();

	static float GetTextureSize() { return m_TextureSize; }

private:
	bool m_IsKilled;
	float m_MoveDelay;
	PlayerId m_PlayerId;
	Texture2DComponent* m_pTexture;
	QBertTile* m_pCurrentTile;

	void HandleMove();
	void OnDeath();

	static const float m_TextureSize;
};