#include "../headers/BasicMode.hpp"
#include "../headers/SquareBoard.hpp"
#include "../headers/Player.hpp"
#include "../headers/EnumOpenResult.hpp"
#include "SFML/Graphics/RenderTarget.hpp"

BasicMode::BasicMode(AbstractBoard* board, Player* player)
    : GameMode(board,player) {}

BasicMode::~BasicMode() {}

OpenResult BasicMode::CheckCellOpen(CellPoint cellPoint) const
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