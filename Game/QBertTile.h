#pragma once
#include "Component.h"

enum class TileState
{
	DefaultState,
	IntermediateState,
	TargetState,
};

class QBertCharacter;
class Texture2DComponent;
class Subject;
class QBertTile final : public Component
{
public:
	QBertTile();
	~QBertTile();

	virtual void Initialize(bool forceInitialize = false) override;
	virtual void Render() const override;
	virtual void Update() override;

	void Reset();

	TileState GetState() const { return m_State; }
	void SetState(TileState state);

	QBertCharacter* GetCurrentCharacter() const { return m_pCurrentCharacter; }
	//Did the character also enter the tile or get killed?
	bool EnterCharacter(QBertCharacter* pNewCharacter);
	void LeaveCharacter();

	Subject* GetSubject() const { return m_pSubject; }
	short GetId() const { return m_Id; }

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
	friend class QBertLevel;
	bool m_HasBeenIntermediateState, m_HasBeenTargetState;
	short m_Id;
	TileState m_State;
	Texture2DComponent* m_pTexture;
	QBertCharacter* m_pCurrentCharacter;
	Subject* m_pSubject;
	Neighbours m_Neighbours;

	bool EvaluateCharacterConflict(QBertCharacter* pNewCharacter);

	static int m_TextureId;
	static const float m_TextureSize;
	static Vector2 m_TextureOffset;

};