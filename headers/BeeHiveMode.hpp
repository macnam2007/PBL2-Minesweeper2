#include "AbstractBoard.hpp"
#include "CellPoint.hpp"
#include "GameMode.hpp"

#define LIFE 2

class BeeHiveMode : public GameMode
{
    private:
        int life = LIFE;
        bool stung = false;
    
    public:
        BeeHiveMode(AbstractBoard* board, Player* player);
        ~BeeHiveMode();
        OpenResult CheckCellOpen(CellPoint cellPoint) const override; // trả về tình trạng mở ô 
        void RenderEffects(sf::RenderTarget& target) const override;
        void Reset() override;
        bool Load(std::istream& input) override;
        void Save(std::ostream& output) const override;
        bool OnHitMine(CellPoint cellPoint) override;
        void RemoveMine(CellPoint cellPoint);
        bool IsBlockingInput() const override; // chặng tương tác khi xuất hiện hiệu ứng ong chính
};