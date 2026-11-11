#pragma once

struct CellPoint
{
    CellPoint(int x, int y) : x(x), y(y) {};
    int x,y;

    bool operator==(const CellPoint& cellPoint) const
    { return (this->x == cellPoint.x && this->y == cellPoint.y);}

    // dành riêng cho map : *this < cellPoint
    bool operator<(const CellPoint& cellPoint) const
    {
        if (this->y != cellPoint.y)
        { return this->y<cellPoint.y; }

        return this->x < cellPoint.x; // chắc chắn rằng trong map không thể có 2 cellPoint giống nhau
    }
};