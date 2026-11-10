#include "AbstractBoard.hpp"
#include "GameMode.hpp"
#include <optional>

struct Record {
    int score;
    float time;

    Record(int score, float time) : score(score), time(time) {}
};

namespace SaveManager
{
    bool SaveGame(const AbstractBoard* board , const GameMode* mode, int mapID, int modeID, float time);
    bool LoadGame(AbstractBoard&, GameMode&, int mapID, int modeID, float& time);

    bool UpdateRecord(int mapID, int modeID, int score, float time);
    std::optional<Record> GetRecord(int mapID, int modeID);
}; 