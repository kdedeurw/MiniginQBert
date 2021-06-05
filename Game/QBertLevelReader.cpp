#include "pch.h"
#include "QBertLevelReader.h"

#include <rapidjson/rapidjson.h>
#include <rapidjson/document.h>
#include <rapidjson/stream.h>
#include <rapidjson/filereadstream.h>

std::vector<QBertLevelData> QBertLevelReader::m_LevelDatas{};

const std::vector<QBertLevelData>& QBertLevelReader::GetLevelData(const std::string& filePath, bool isOverWrite)
{
	if (!m_LevelDatas.empty())
	{
		if (!isOverWrite)
			return m_LevelDatas;
		m_LevelDatas.clear();
	}

	using rapidjson::Document;
	Document jsonDoc;
	FILE* fp = nullptr;
	fopen_s(&fp, filePath.c_str(), "rb");

	if (!fp)
	{
		std::cout << "Unable to open file: " << filePath << '\n';
		return m_LevelDatas;
	}

	fseek(fp, 0, SEEK_END);
	const size_t fileSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	char* readBuffer = new char[fileSize];
	rapidjson::FileReadStream is(fp, readBuffer, fileSize);
	jsonDoc.ParseStream(is);

	delete[] readBuffer;
	fclose(fp);

	using rapidjson::Value;

	QBertLevelData levelData{};
	const Value& levels = jsonDoc["levels"];
	for (Value::ConstValueIterator levelItr = levels.Begin(); levelItr != levels.End(); ++levelItr)
	{
		const Value& level = *levelItr;
		levelData.Id = static_cast<BYTE>(level["level"].GetUint());
		levelData.Steps = static_cast<BYTE>(level["steps"].GetUint());
		levelData.CanGoBack = level["can_go_back"].GetBool();

		int currRound{};
		const Value& rounds = level["rounds"];
		for (Value::ConstValueIterator roundItr = rounds.Begin(); roundItr != rounds.End(); ++roundItr)
		{
			const Value& round = *roundItr;
			QBertRound& Qround = levelData.Rounds[currRound];
			Qround.RoundId = static_cast<BYTE>(round["round"].GetUint());
			Qround.Enemies = round["enemies"].GetString();
			++currRound;
		}
		m_LevelDatas.push_back(std::move(levelData));
	}
	const Value& tiles = jsonDoc["tiles"];
	for (Value::ConstValueIterator itr = tiles.Begin(); itr != tiles.End(); ++itr)
	{
		const Value& tile = *itr;
		//TODO: parse tile
		tile;
	}
	return m_LevelDatas;
}