#pragma once

#include "Board.hpp"

class HexBoard : public Board
{
    private:
        float sizeCell;
        sf::Vector2f pointDraw; // vị trí trung tâm của bảng game
    
    public:
        HexBoard(float,sf::Vector2f,int,int,int);

        std::vector<CellPoint> GetNeighbors(const CellPoint& cellPoint) override; // Danh sách ô lân cận
        sf::Vector2f CellToPixel(const CellPoint& cellPoint) const override; // chuyển ô này thành tọa độ cần vẽ trên màn hình
        std::optional<CellPoint> MouseToCell(const sf::Vector2f& mousePoint) const override; // kiểm tra vị trí này nằm trong ô nào

        float GetSizeCell() const override;
};