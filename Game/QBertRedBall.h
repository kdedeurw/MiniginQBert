#pragma once
#include "QBertCharacter.h"

class SpriteComponent;
class QBertRedBall : public QBertCharacter
{
public:
	QBertRedBall();
	virtual ~QBertRedBall();

	virtual void Initialize(bool forceInitialize = false) override;
	virtual void Render() const override;
	virtual void Update() override;

	virtual void Kill(bool hasFallen) override;
	virtual const QBertCharacterType GetType() const override { return QBertCharacterType::RedBall; }
	virtual const QBertTileAlteration GetTileAlteration() const override { return QBertTileAlteration::None; }

	//how long it will rest on a tile before moving again
	void SetRestDelay(float delay) { m_RestDelay = delay; };

protected:
	float m_CurrentRestDelay;
	float m_RestDelay = 0.5f;
	SpriteComponent* m_pSprite;

	virtual void HandleAI();
	virtual void HasMoved() override;
	virtual void HasLanded() override;

	static float GetTextureSize() { return m_TextureSize; }
	static const float m_TextureSize;
};