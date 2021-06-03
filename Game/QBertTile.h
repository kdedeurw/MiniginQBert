#pragma once
#include "QBertEntity.h"
#include "Vectors.h"

enum class State
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

	State GetState() const { return m_State; }
	void UpdateState(State state);

	static float GetTextureSize() { return m_TextureSize; }

private:
	State m_State;
	Texture2DComponent* m_pTexture;
	Vector2 m_InitOffset;

	int level;
	int x;
	int y;
	friend class QBertLevel;
	QBertTile* m_pNeighbours[4];

	const static float m_TextureSize;

};