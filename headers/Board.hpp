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
        bool IsGenerated() const override;
        void ResetBoard() override;

        OpenResult OpenAt(const CellPoint& cellPoint) override;   // click trái vào ô Hidden
        OpenResult OpenChord(const CellPoint& cellPoint);    // click trái vào ô đã mở
        void ToggleMark(const CellPoint& cellPoint) override;     // click phải

        bool CheckWin();

        bool Load(std::istream& input, float& time) override;
        void Save(std::ostream& output) const override;
};