#include "../headers/HexBoard.hpp"
#include "SFML/System/Vector2.hpp"
#include <cmath>
#include <optional>
#include <fstream>
#include <iostream>

HexBoard::HexBoard(float sizeCell, sf::Vector2f pointDraw,int cols,int rows,int mineCount)
    : sizeCell(sizeCell), pointDraw(pointDraw), Board(cols,rows,mineCount) {}

std::vector<CellPoint> HexBoard::GetNeighbors(const CellPoint& cellPoint)
{
    std::vector<CellPoint> res;
    int x = cellPoint.x;
    int y = cellPoint.y;
    
    //
    /* {move_x,move_y} */
    int evenRow[6][2] =
    {
        {-1,-1}, {0,-1},
        {-1,0 }, {1,0 },
        {-1,1 }, {0, 1}
    };
    int oddRow[6][2] =
    {
        { 0,-1}, { 1, -1},
        {-1, 0}, { 1, 0},
        { 0, 1}, { 1, 1},
    };

    auto& select = (y%2==0)?evenRow:oddRow;

    for (int i=0; i<6; i++)
    {
        int X = x + select[i][0];
        int Y = y + select[i][1];

        if (X>=0 && X<this->cols && Y>=0 && Y<this->rows)
        { res.push_back(CellPoint(X,Y));}
    }
    return res;
}

sf::Vector2f HexBoard::CellToPixel(const CellPoint& point) const
{
    float R = this->sizeCell; 
    float width = std::sqrt(3.0f) * R;
    
    // Chiều rộng: Các hàng lẻ thụt ra thêm nửa ô nên cộng thêm (width / 2)
    float totalWidth = this->cols * width + (width / 2.0f);
    // Chiều cao: Mỗi hàng cách nhau 1.5R, cộng thêm 2R cho nửa trên hàng đầu và nửa dưới hàng cuối
    float totalHeight = (this->rows - 1) * (1.5f * R) + 2.0f * R;
    
    // Từ Tâm (pointDraw) lùi về để tìm tọa độ góc trên cùng bên trái của bản đồ
    float topLeftX = this->pointDraw.x - (totalWidth / 2.0f);
    float topLeftY = this->pointDraw.y - (totalHeight / 2.0f);

    // Tính vị trí pixel dựa trên Top Left mới
    float pixelY = point.y * (1.5f * R);
    float pixelX = point.x * width;
    if (point.y % 2 != 0) { pixelX += (width / 2.0f); }
    
    // Trả về đúng tâm của từng ô lục giác
    return sf::Vector2f(
        topLeftX + pixelX + (width / 2.0f), 
        topLeftY + pixelY + R
    );
}

std::optional<CellPoint> HexBoard::MouseToCell(const sf::Vector2f& mousePoint) const
{
    float R = this->sizeCell;
    float width = std::sqrt(3.0f) * R;

    // Tính Top-Left của toàn bộ bàn cờ (Giữ nguyên)
    float totalWidth = this->cols * width + (width / 2.0f);
    float totalHeight = (this->rows - 1) * (1.5f * R) + 2.0f * R;
    
    float topLeftX = this->pointDraw.x - (totalWidth / 2.0f);
    float topLeftY = this->pointDraw.y - (totalHeight / 2.0f);

    float localX = mousePoint.x - topLeftX;
    float localY = mousePoint.y - topLeftY;

    int approxY = static_cast<int>(std::round((localY - R) / (1.5f * R)));
    int approxX = static_cast<int>(std::round((localX - width / 2.0f) / width));

    for (int y = approxY - 1; y <= approxY + 1; ++y)
    {
        for (int x = approxX - 1; x <= approxX + 1; ++x)
        {
            if (x < 0 || x >= this->cols || y < 0 || y >= this->rows) continue;

            CellPoint currentPoint(x, y);
            sf::Vector2f center = CellToPixel(currentPoint);

            // Tính khoảng cách từ chuột đến Tâm
            float dx = std::abs(mousePoint.x - center.x);
            float dy = std::abs(mousePoint.y - center.y);

            // Trục X, Y không được vượt quá Hình chữ nhật bao quanh
            if (dx > width / 2.0f || dy > R) continue; 
            
            //  Kiểm tra 4 cạnh chéo (Bằng phương trình đường chéo)
            // Phương trình chuẩn cho Pointy-Topped Hexagon: dx + sqrt(3)*dy <= sqrt(3)*R
            if (dx + std::sqrt(3.0f) * dy <= std::sqrt(3.0f) * R)
            { return currentPoint; }
        }
    }

    return std::nullopt; 
}

float HexBoard::GetSizeCell() const
{ return this->sizeCell; }