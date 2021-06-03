#pragma once
#include <fstream>
#include <string>

struct QBertLevelData
{
	short LevelId = -1;
	short TileCount = -1;
	bool* pTiles;
	short Rule;
	const char* Enemies;
};

class QBertLevelReader final
{
public:
	QBertLevelReader(const std::string& filePath);
	~QBertLevelReader();

	const QBertLevelData& ReadNextLevel();
	
private:
	std::ifstream m_Stream;
	QBertLevelData m_CurrentLevel;

};