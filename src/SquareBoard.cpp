#include "../headers/SquareBoard.hpp"
#include "SFML/System/Vector2.hpp"
#include <optional>
#include <fstream>
#include <iostream>


SquareBoard::SquareBoard(float sizeCell, sf::Vector2f pointDraw,int cols,int rows,int mineCount)
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
            if (x!=cellPoint.x || y!=cellPoint.y)
            {res.push_back(CellPoint(x,y));}
        }
    }   
    return res;
}

sf::Vector2f SquareBoard::CellToPixel(const CellPoint& point) const
{
    float totalWidth = this->cols * this->sizeCell;
    float totalHeight = this->rows * this->sizeCell;

    float topLeftX = this->pointDraw.x - (totalWidth / 2.0f);
    float topLeftY = this->pointDraw.y - (totalHeight / 2.0f);

    float pixelX = point.x * this->sizeCell;
    float pixelY = point.y * this->sizeCell;
    
    return sf::Vector2f(
        topLeftX + pixelX + (this->sizeCell / 2.0f),
        topLeftY + pixelY + (this->sizeCell / 2.0f)
    );
} 

// optional có thể trả về 1 giá trị có thể null giống như con trỏ 
std::optional<CellPoint> SquareBoard::MouseToCell(const sf::Vector2f& mousePoint) const
{
    // khoảng cách từ Tâm đến góc Trên Trái bảng
    float totalWidth = this->cols * this->sizeCell;
    float totalHeight = this->rows * this->sizeCell;

    // tọa độ góc Trên Trái
    float topLeftX = this->pointDraw.x - (totalWidth / 2.0f);
    float topLeftY = this->pointDraw.y - (totalHeight / 2.0f);

    // quy về gốc 0 0 để tính toán
    float localX = mousePoint.x - topLeftX;
    float localY = mousePoint.y - topLeftY;

    //  Suy ra tọa độ ô
    int x = static_cast<int>(localX / this->sizeCell);
    int y = static_cast<int>(localY / this->sizeCell);

    // lọc
    if (localX >= 0 && localY >= 0 && x >= 0 && x < this->cols && y >= 0 && y < this->rows)
    {
        return CellPoint(x, y);
    }
    
    return std::nullopt;
}

float SquareBoard::GetSizeCell() const
{ return this->sizeCell; }
