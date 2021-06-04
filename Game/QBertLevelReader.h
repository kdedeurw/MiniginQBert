#pragma once
#include <string>

struct QBertRound
{
	BYTE RoundId;
	std::string Enemies;
};

struct QBertLevelData
{
	static const int MaxRounds = 4;

	bool CanGoBack;
	BYTE Id;
	BYTE Steps;
	QBertRound Rounds[MaxRounds];
};

struct QBertLevelLayout
{
	struct QBertTileData
	{
		BYTE TileId;
		Vector2 Pos;
	};
	BYTE TileAmount;
	QBertTileData** pTiles;
};

class QBertLevelReader final
{
public:
	QBertLevelReader();
	~QBertLevelReader();

	const std::vector<QBertLevelData>& ReadLevelDatas(const std::string& filePath);
	
private:
	std::vector<QBertLevelData> m_LevelDatas;

};