#pragma once
#include "QBertEntity.h"
#include "Vectors.h"

enum class TileState
{
	DefaultState,
	IntermediateState,
	TargetState,
};

class Texture2DComponent;
class QBertTile final : public QBertEntity
{
public:
	QBertTile();
	~QBertTile();

	virtual void Initialize() override;
	virtual void Render() const override;
	virtual void Update() override;

	TileState GetState() const { return m_State; }
	void UpdateState(TileState state);
	QBertEntity* GetEntity() const { return m_pCurrentEntity; }
	void UpdateEntity(QBertEntity* pEntity) { m_pCurrentEntity = pEntity; }

	short GetTileId() const { return m_Id; }

	struct Neighbours
	{
		QBertTile* pLeftTopNeighbour = nullptr;
		QBertTile* pRightTopNeighbour = nullptr;
		QBertTile* pRightBottomNeighbour = nullptr;
		QBertTile* pLeftBottomNeighbour = nullptr;
	};

	const Neighbours& GetNeighbours() const { return m_Neighbours; }

	static float GetTextureSize() { return m_TextureSize; }

private:
	short m_Id;
	TileState m_State;
	Texture2DComponent* m_pTexture;
	QBertEntity* m_pCurrentEntity;
	Vector2 m_InitOffset;
	friend class QBertLevel;
	Neighbours m_Neighbours;

	const static float m_TextureSize;

};