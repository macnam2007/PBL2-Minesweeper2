#pragma once

struct CellPoint
{
    CellPoint(int x, int y) : x(x), y(y) {};
    int x,y;

    bool operator==(const CellPoint& cellPoint)
    { return (this->x == cellPoint.x && this->y == cellPoint.y);}
};