#pragma once

#include "AbstractBoard.hpp"
#include "CellPoint.hpp"

class Board : public AbstractBoard
{
    protected:
        int cols, rows;
        int mineCount;
        bool generated = false;
        std::vector<Cell> cells;

    public: 
        Board(int, int, int);
 
        Cell* GetCell(const CellPoint& cellPoint) override;

        void Generate(const CellPoint& cellPoint) override; // tạo màn với cú click đầu tiên
        void FloodFill(const CellPoint& cellPoint) override; // hủy các ô trống lân cận

        int GetCols() const override;
        int GetRows() const override;

};