#pragma once

#include "EnumCellState.hpp"

class Cell
{
    private:
        bool isMine = false;
        CellState state = CellState::Hidden;
        int number = 0;
    
    public:
        Cell();

        void OpenCell();
        bool CanOpenCell() const; // kiểm tra đc mở hay không
        bool Openned() const; // kiểm tra xem ô đã mở chưa
        int GetNumber() const;
        void MineToggle();
        void SetNumber(int);
        CellState GetState() const;
        void ChangeState(CellState);
        void Reset();
        bool CheckIsMine() const;
};