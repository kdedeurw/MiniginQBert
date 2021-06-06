#pragma once
#include "QBertCharacter.h"

class QBertPlayer final : public QBertCharacter
{
public:
	QBertPlayer();
	~QBertPlayer();

	void Initialize() override;
	void Render() const override;
	void Update() override;

	void SetId(PlayerId id) { m_PlayerId = id; }
	PlayerId GetId() const { return m_PlayerId; }
	void Kill(bool hasFallen) override;
	void Respawn();

	bool IsKilled() const { return m_IsKilled; }
	const QBertCharacterType GetType() const override { return QBertCharacterType::QBert; };

	static float GetTextureSize() { return m_TextureSize; }

private:
	bool m_IsKilled;
	PlayerId m_PlayerId;
	Texture2DComponent* m_pTexture;

	void HasMoved() override;
	void HasLanded() override;
	void HandleInput();

	static const float m_TextureSize;
};