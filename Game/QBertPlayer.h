#pragma once
#include "QBertCharacter.h"

class QBertSpinningDisk;
class QBertPlayer final : public QBertCharacter
{
public:
	QBertPlayer();
	~QBertPlayer();

	void Initialize(bool forceInitialize = false) override;
	void Render() const override;
	void Update() override;

	void SetId(PlayerId id) { m_PlayerId = id; }
	PlayerId GetId() const { return m_PlayerId; }
	void Kill(bool hasFallen) override;
	void Respawn(bool hasFallen = true);
	void UseSpinningDisk(QBertSpinningDisk* pDisk);

	bool IsKilled() const { return m_IsKilled; }
	const QBertCharacterType GetType() const override { return QBertCharacterType::QBert; }
	const QBertTileAlteration GetTileAlteration() const override { return QBertTileAlteration::Next; }

	static float GetTextureSize() { return m_TextureSize; }

private:
	bool m_IsKilled;
	float m_RespawnDelay;
	PlayerId m_PlayerId;
	Texture2DComponent* m_pTexture;
	QBertSpinningDisk* m_pDisk;

	void HasMoved() override;
	void HasLanded() override;
	void HandleInput();

	static const float m_TextureSize;
};