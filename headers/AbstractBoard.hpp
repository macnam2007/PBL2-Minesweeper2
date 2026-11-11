#pragma once
#include "Cell.hpp"
#include "CellPoint.hpp"
#include "EnumCellState.hpp"
#include "EnumOpenResult.hpp"
#include "SFML/Graphics.hpp"
#include <optional>
#include <vector>
#include <string>
#include <functional>

class AbstractBoard
{
    protected:
        bool ended = false;
        bool losed = false;
        std::vector<CellPoint> cellPointDead;
        int countClick = 0;
        int countClickTB = 0; // số click cần thiết để giải (Mode:easy,normal,hard : không cần kỉ thuật nâng cao | Mode:explore : có thể cần kỉ thuật nâng cao) 
        std::function<bool(CellPoint)> validator; // nhận lambda kiểm tra có bị chặn bởi rễ cây không
    public:
        AbstractBoard(){};
        virtual ~AbstractBoard(){};

        virtual Cell* GetCell(const CellPoint& cellPoint) = 0;
        virtual std::vector<CellPoint> GetNeighbors(const CellPoint& cellPoint) = 0; // Danh sách ô lân cận

        virtual void Generate(const CellPoint& cellPoint) = 0; // tạo màn với cú click đầu tiên
        virtual void FloodFill(const CellPoint& cellPoint) = 0; // hủy các ô trống lân cận

        virtual sf::Vector2f CellToPixel(const CellPoint& cellPoint) const = 0; // chuyển ô này thành tọa độ cần vẽ trên màn hình
        virtual std::optional<CellPoint> MouseToCell(const sf::Vector2f& mousePoint) const= 0; // kiểm tra vị trí này nằm trong ô nào
        
        void SetValidator(std::function<bool(CellPoint)> validator) { this->validator = validator; }
        virtual int GetCountClick() const { return this->countClick; }
        virtual void UpCountCLick() { this->countClick++;}
        virtual int GetCountClickTB() const { return this->countClickTB; }
        virtual int GetMineCount() const = 0;
        virtual int GetFlaggedCount()  const = 0;
        virtual int GetCols() const = 0;
        virtual int GetRows() const = 0;
        virtual float GetSizeCell() const = 0;
        virtual std::vector<CellPoint> GetCellPointDead() const { return this->cellPointDead; } 
        virtual void AddCellPointDead(CellPoint cellPoint) { this->cellPointDead.push_back(cellPoint); }

        virtual bool IsGenerated() const = 0;
        virtual bool IsEnded() const {return this->ended;}
        virtual bool IsLosed() const {return this->losed;}
        virtual void SetEnded(bool value) { this->ended = value; }
        virtual void SetLosed(bool value) { this->losed = value; }

        virtual bool IsWon() const = 0;
        virtual void ResetBoard() = 0;


        virtual OpenResult OpenAt(const CellPoint& cellPoint) = 0;   // click trái
        virtual void ToggleMark(const CellPoint& cellPoint) = 0;     // click phải

        virtual bool Load(std::istream& input, float& time) = 0;       // Lưu các ô hiện tại vào vùng trên của file
        virtual void Save(std::ostream& output, float time) const = 0; // Tương tác file vùng trên (chứa các ô)
};