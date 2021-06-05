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
	QBertLevelReader() = delete;
	~QBertLevelReader() = delete;

	static const std::vector<QBertLevelData>& GetLevelData(const std::string& filePath, bool isOverWrite = false);
	
private:
	static std::vector<QBertLevelData> m_LevelDatas;

};