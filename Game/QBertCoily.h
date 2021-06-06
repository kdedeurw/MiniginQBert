#pragma once
#include "QBertRedBall.h"

class QBertCoily final : public QBertRedBall
{
public:
	QBertCoily();
	~QBertCoily();

	void Initialize(bool forceInitialize = false) override;
	void Render() const override;
	void Update() override;

	void Kill(bool hasFallen) override;
	const QBertCharacterType GetType() const override { return QBertCharacterType::Coily; }
	const QBertTileAlteration GetTileAlteration() const override { return QBertTileAlteration::None; }

	static float GetTextureHeight() { return m_TextureHeight; }

private:
	bool m_IsDescending;
	void HandleAI() override;
	void HasMoved() override;
	void HasLanded() override;

	void HandleCoilyAI(QBertCharacterMovement* pMovement);

	static const float m_TextureHeight;
};