#include "../headers/SquareBoard.hpp"
#include "SFML/System/Vector2.hpp"
#include <optional>


SquareBoard::SquareBoard(int sizeCell, sf::Vector2f pointDraw,int cols,int rows,int mineCount)
    : sizeCell(sizeCell), pointDraw(pointDraw), Board(cols,rows,mineCount) {}

std::vector<CellPoint> SquareBoard::GetNeighbors(const CellPoint& cellPoint)
{
    std::vector<CellPoint> res;
    for ( int y = (cellPoint.y-1<0?0:cellPoint.y-1);
          y<=(cellPoint.y+1>=this->rows?this->rows-1:cellPoint.y+1);
          y++)
    {
        for ( int x = (cellPoint.x-1<0?0:cellPoint.x-1);
            x<=(cellPoint.x+1>=this->cols?this->cols-1:cellPoint.x+1);
            x++)
        {
            if (x!=cellPoint.x && y!=cellPoint.y)
            {res.push_back(CellPoint(x,y));}
        }
    }   
    return res;
}

sf::Vector2f SquareBoard::CellToPixel(const CellPoint& cellPoint) const
{ return this->pointDraw + sf::Vector2f(cellPoint.x*this->sizeCell,cellPoint.y*this->sizeCell); }

// optional có thể trả về 1 giá trị có thể null giống như con trỏ 
std::optional<CellPoint> SquareBoard::MouseToCell(const sf::Vector2f& mousePoint) const
{
    float mouseX = mousePoint.x;
    float mouseY = mousePoint.y;
    // kiểm tra xem có nằm trong vùng bảng không
    if (
        ( this->pointDraw.x < mouseX && mouseX < this->pointDraw.x+this->sizeCell*this->cols ) &&
        ( this->pointDraw.y < mouseY && mouseY < this->pointDraw.y+this->sizeCell*this->rows )
    )
    {
        // điều chỉnh cho gốc về (0,0)
        mouseX -= this->pointDraw.x;
        mouseY -= this->pointDraw.y;
    
        return CellPoint(mouseX/this->sizeCell,mouseY/this->sizeCell);

    }
    else return std::nullopt;
}

int SquareBoard::GetMineCount() const
{
    int count = 0;
    for (const Cell cell : this->cells)
    { if (cell.CheckIsMine()) count++; }
    return count;
}

int SquareBoard::GetFlaggedCount() const
{
    int count = 0;
    for (const Cell cell : this->cells)
    { if (cell.GetState() == CellState::Flagged) count++; }
    return count;
}