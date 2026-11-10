#include "../headers/GameMode.hpp"
#include "../headers/SquareBoard.hpp"
#include "../headers/EnumOpenResult.hpp"
#include "../headers/EnumCellState.hpp"

GameMode::GameMode(AbstractBoard* board, Player* player)
    : board(board), player(player) {}

GameMode::~GameMode(){}

// tình trạng khi mở ô (mặc định)
OpenResult GameMode::CheckCellOpen(CellPoint cellPoint) const
{
    Cell* cell = this->board->GetCell(cellPoint);
    if (!cell || cell->GetState() != CellState::Hidden) return OpenResult::Blocked;
    return cell->CheckIsMine() ? OpenResult::Lose : OpenResult::Safe;
}

// game dò mìn mặc định không có tương tác đặc biệt nên trả về true
bool GameMode::CanInteract(CellPoint)
{ return true; }

// game dò mìn mặc định không có tương tác giữ phim
bool GameMode::CanHold(CellPoint) const
{ return false; }

// game dò mìn mặc định không có hiệu ứng nên k chặn tương tác
bool GameMode::IsBlockingInput() const
{ return false; } 
