#pragma once
#include "QBertBaseTile.h"

class SpriteComponent;
class QBertSpinningDisk final : public QBertBaseTile
{
public:
	QBertSpinningDisk();
	~QBertSpinningDisk();

	void Initialize(bool forceInitialize = false) override;
	void Render() const override {};
	void Update() override;

	void Kill();

	const QBertTileType GetType() const override { return QBertTileType::SpinningDisk; }

	bool TryEnter(QBertCharacter* pNewCharacter) override;
	void Connect(QBertBaseTile* pTile, bool isLeft);
	void Disconnect();

private:
	bool m_IsOnLeftSide;
	friend class QBertLevel;
	SpriteComponent* m_pSprite;

	static const float m_TextureSize;
};