#pragma once
#include "QBertBaseTile.h"

enum class TileState
{
	DefaultState,
	IntermediateState,
	TargetState,
};

class Texture2DComponent;
class Subject;
class QBertSpinningDisk;
class QBertTile final : public QBertBaseTile
{
public:
	QBertTile();
	~QBertTile();

	virtual void Initialize(bool forceInitialize = false) override;
	virtual void Render() const override;
	virtual void Update() override;

	void Reset();
	void AddSpinningDisk(QBertSpinningDisk* pDisk, bool isLeft);
	void RemoveSpinningDisk(bool isLeft);

	const QBertTileType GetType() const override { return QBertTileType::Tile; }
	TileState GetState() const { return m_State; }
	void SetState(TileState state);

	bool TryEnter(QBertCharacter* pNewCharacter) override;

	static float GetTextureSize() { return m_TextureSize; }

private:
	friend class QBertLevel;
	bool m_HasBeenIntermediateState, m_HasBeenTargetState;
	TileState m_State;
	Texture2DComponent* m_pTexture;

	bool EvaluateCharacterConflict(QBertCharacter* pNewCharacter);

	static int m_TextureId;
	static const float m_TextureSize;
	static Vector2 m_TextureOffset;

};