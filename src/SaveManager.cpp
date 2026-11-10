#include "../headers/SaveManager.hpp"
#include <fstream>
#include <optional>
#include <string>

bool SaveManager::LoadGame(AbstractBoard& board, GameMode& mode, int mapID, int modeID, float& time)
{
    std::string nameFile = std::to_string(mapID) + "_" + std::to_string(modeID) + ".txt";
    std::ifstream input(nameFile);

    if (!input)
    { return false; }

    if (!board.Load(input,time))
    { return false; }

    if (!mode.Load(input))
    { return false; }

    return true;
}

bool SaveManager::SaveGame(const AbstractBoard* board , const GameMode* mode, int mapID, int modeID, float time)
{
    std::string nameFile = std::to_string(mapID) + "_" + std::to_string(modeID) +".txt";
    std::ofstream output(nameFile);
    if (!output) { return false; }
    board->Save(output);
    mode->Save(output);
    return true;
}

bool SaveManager::UpdateRecord(int mapID, int modeID, int score, float time)
{
    std::string nameFile = std::to_string(mapID) + "_" + std::to_string(modeID) +".txt";
    std::ofstream output(nameFile);
    if (!output) { return false; }
    output << score << "\n" <<time;
    return true;
}

std::optional<Record> SaveManager::GetRecord(int mapID, int modeID)
{
    std::string nameFile = std::to_string(mapID) + "_" + std::to_string(modeID) +".txt";
    std::ifstream input(nameFile);
    int score;
    float time;
    if (!(input >> score >> time) || score<0 || time<0)
    { return std::nullopt; }
    return Record(score,time);
}