#include "../headers/SaveManager.hpp"
#include <fstream>
#include <optional>
#include <string>

#include <iostream>

bool SaveManager::LoadGame(AbstractBoard& board, GameMode& mode, int mapID, int modeID, float& time)
{
    std::string nameFile = "../data/" + std::to_string(mapID) + "_" + std::to_string(modeID) + ".txt";
    std::ifstream input(nameFile);

    if (!input)
    { std::cout<<"error:1"; return false; }

    if (!board.Load(input,time))
    { std::cout<<"error:2"; return false; }

    if (!mode.Load(input))
    { std::cout<<"error:3"; return false; }

    return true;
}

bool SaveManager::SaveGame(const AbstractBoard* board , const GameMode* mode, int mapID, int modeID, float time)
{
    std::string nameFile = "../data/" + std::to_string(mapID) + "_" + std::to_string(modeID) +".txt";
    std::ofstream output(nameFile);
    if (!output) { return false;}
    board->Save(output,time);
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