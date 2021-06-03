#include "pch.h"
#include "QBertLevelReader.h"

QBertLevelReader::QBertLevelReader(const std::string& filePath)
    : m_Stream{ filePath }
{
}

QBertLevelReader::~QBertLevelReader()
{
    m_Stream.close();
}

const QBertLevelData& QBertLevelReader::ReadNextLevel()
{
    if (!m_Stream.is_open())
        return m_CurrentLevel;

    if (m_CurrentLevel.pTiles)
        delete m_CurrentLevel.pTiles;

    std::string line;
    while (std::getline(m_Stream, line))
    {
        //skip comments
        if (line.empty() || line.front() == '#')
            continue;

        if (line.find("Level") == 0)
        {
            std::string levelId = line.substr(line.find(':'));
            m_CurrentLevel.LevelId = (short)std::stoi(levelId);
            continue;
        }
        else if (line.find("Tiles") == 0)
        {
            std::string tiles = line.substr(line.find(':'));
            m_CurrentLevel.TileCount = (short)std::stoi(tiles);
            m_CurrentLevel.pTiles = new bool[m_CurrentLevel.TileCount]{};

            int currTile = 0;
            do
            {
                std::getline(m_Stream, line);
                for (char c : line)
                {
                    m_CurrentLevel.pTiles[currTile] = static_cast<bool>(c - '0');
                    ++currTile;
                }
            } while (!line.empty());
            continue;
        }
        else if (line.find("Rule") == 0)
        {
            std::string rule = line.substr(line.find(':'));
            m_CurrentLevel.Rule = (short)std::stoi(rule);
            continue;
        }
        else if (line.find("Enemies") == 0)
        {
            std::getline(m_Stream, line);
            m_CurrentLevel.Enemies;
            continue;
        }
        else if (line.find("End") == 0)
            break;
    }
    return m_CurrentLevel;
}