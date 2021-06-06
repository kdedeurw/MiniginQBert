#pragma once
#include "QBertCharacter.h"

class Texture2DComponent;
class QBertRedBall : public QBertCharacter
{
public:
	QBertRedBall();
	virtual ~QBertRedBall();

	virtual void Initialize() override;
	virtual void Render() const override;
	virtual void Update() override;

	virtual void Kill(bool hasFallen) override;
	virtual const QBertCharacterType GetType() const override { return QBertCharacterType::RedBall; }

	//how long it will rest on a tile before moving again
	void SetRestDelay(float delay) { m_RestDelay = delay; };

private:
	float m_CurrentRestDelay;
	float m_RestDelay = 0.25f;
	Texture2DComponent* m_pTexture;

	virtual void HandleAI();
	virtual void HasMoved() override;
	virtual void HasLanded() override;

	static float GetTextureSize() { return m_TextureSize; }

	static const float m_TextureSize;
};