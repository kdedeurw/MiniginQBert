#pragma once
#include "Component.h"

class QBertTile;
class QBertCharacter;
class QBertCharacterMovement : public Component
{
public:
	QBertCharacterMovement();
	virtual ~QBertCharacterMovement();

	virtual void Initialize() override;
	virtual void Render() const override {};
	virtual void Update() override;

	virtual void SetToTile(QBertTile* pTile, bool isMoveOn = false);
	void SetToTile(int tileId, bool isMoveOn = false);

	float GetMoveDelay() const { return m_MoveDelay; }
	void SetMoveDelay(float moveDelay) { m_MoveDelay = moveDelay; }

	bool IsMoving() const { return m_CurrentMoveDelay > 0.f; }
	QBertTile* GetCurrentTile() const { return m_pCurrentTile; }

protected:
	friend class QBertLevel;
	bool m_IsOnTile;
	float m_CurrentMoveDelay;
	float m_MoveDelay = 0.5f;
	QBertCharacter* m_pCharacter;
	QBertTile* m_pCurrentTile;
	Vector2 m_FormerPos;
	Vector2 m_DesiredPos;

	void HandleMove();
	void LandOnTile(QBertTile* pTile);
	void LandOnTile(int tileId);
};