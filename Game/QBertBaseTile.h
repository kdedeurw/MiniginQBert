#pragma once
#include <Component.h>

enum class QBertTileType
{
	Tile,
	SpinningDisk,
	Spawn,
};

class QBertCharacter;
class QBertLevel;
class QBertBaseTile : public Component
{
public:
	QBertBaseTile();
	virtual ~QBertBaseTile();

	virtual const QBertTileType GetType() const = 0;

	QBertCharacter* GetCurrentCharacter() const { return m_pCurrentCharacter; }
	//Did the character also enter the tile or get killed?
	virtual bool TryEnter(QBertCharacter* pNewCharacter) = 0;
	void LeaveCharacter();

	Subject* GetSubject() const { return m_pSubject; }
	short GetId() const { return m_Id; }

	QBertLevel* GetLevel() const { return m_pLevel; }

	struct Neighbours
	{
		QBertBaseTile* pLeftTopNeighbour = nullptr;
		QBertBaseTile* pRightTopNeighbour = nullptr;
		QBertBaseTile* pRightBottomNeighbour = nullptr;
		QBertBaseTile* pLeftBottomNeighbour = nullptr;
		QBertBaseTile* pLeftNeighbour = nullptr;
		QBertBaseTile* pRightNeighbour = nullptr;
	};

	const Neighbours& GetNeighbours() const { return m_Neighbours; }

protected:
	friend class QBertLevel;
	short m_Id;
	Subject* m_pSubject;
	QBertCharacter* m_pCurrentCharacter;
	Neighbours m_Neighbours;

private:
	friend class QBertLevel;
	QBertLevel* m_pLevel;
};