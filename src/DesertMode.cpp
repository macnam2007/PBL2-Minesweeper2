#include "../headers/DesertMode.hpp"
#include "SFML/Graphics/RenderTarget.hpp"

DesertMode::DesertMode(AbstractBoard* board, Player* player)
    : GameMode(board, player) {}

DesertMode::~DesertMode() {}

OpenResult DesertMode::CheckCellOpen(CellPoint cellPoint) const
{
    Cell* cell = this->board->GetCell(cellPoint);
    // Ô cần mở đang được đánh dấu
    if(
        cell->GetState() == CellState::Flagged ||
        cell->GetState() == CellState::Questioned ||
        (cell->GetState() == CellState::Openned && cell->GetNumber() == 0)
    ) { return OpenResult::Blocked; }

    if (cell->CheckIsMine())
    { return OpenResult::Lose; }

    { return OpenResult::Safe; } // board bên ngoài nhận và tự kiểm tra nên mở như thế nào
}

bool DesertMode::NeedHold(CellPoint cellPoint) const
{
    for (const auto& [point,_] : this->sands)
    { if (point.x == cellPoint.x && point.x == cellPoint.y) {return true;} }
    return false;
}

void DesertMode::OnTick(CellPoint cellPoint, float deltaTime)
{
    for (auto& [point,time] : this->sands)
    { if (point.x == cellPoint.x && point.x == cellPoint.y) {time-=deltaTime;} }
}

void DesertMode::OnLeftHold(CellPoint cellPoint)
{ 
    for (auto& [point,time] : this->sands)
    {
        if (point.x == cellPoint.x && point.x == cellPoint.y && time <= 0)
        {
            this->sands.erase(cellPoint);
            return;
        }
    }
}

void DesertMode::RenderEffects(sf::RenderTarget& target) const
{
    
}
