#pragma once
#include "Cell.hpp"
#include "CellPoint.hpp"
#include "EnumCellState.hpp"
#include "SFML/Graphics.hpp"
#include <optional>
#include <vector>

class AbstractBoard
{
    public:
        AbstractBoard(){};
        virtual ~AbstractBoard(){};

        virtual Cell* GetCell(const CellPoint& cellPoint) = 0;
        virtual std::vector<CellPoint> GetNeighbors(const CellPoint& cellPoint) = 0; // Danh sách ô lân cận

        virtual void Generate(const CellPoint& cellPoint) = 0; // tạo màn với cú click đầu tiên
        virtual void FloodFill(const CellPoint& cellPoint) = 0; // hủy các ô trống lân cận

        virtual sf::Vector2f CellToPixel(const CellPoint& cellPoint) const = 0; // chuyển ô này thành tọa độ cần vẽ trên màn hình
        virtual std::optional<CellPoint> MouseToCell(const sf::Vector2f& mousePoint) const= 0; // kiểm tra vị trí này nằm trong ô nào
        
        virtual int GetMineCount() const = 0;
        virtual int GetFlaggedCount()  const = 0;
};