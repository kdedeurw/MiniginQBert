#pragma once
#include "QBertRedBall.h"

class SpriteComponent;
class QBertSlickSam final : public QBertRedBall
{
public:
	QBertSlickSam();
	~QBertSlickSam();

	void Initialize(bool forceInitialize = false) override;
	void Render() const override;
	void Update() override;

	void Kill(bool hasFallen) override;
	const QBertCharacterType GetType() const override { return QBertCharacterType::SlickSam; }
	const QBertTileAlteration GetTileAlteration() const override { return QBertTileAlteration::Previous; }

private:
	bool m_IsSlick;

};