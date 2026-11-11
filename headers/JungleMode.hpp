#include "AbstractBoard.hpp"
#include "GameMode.hpp"
#include "CellPoint.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/Texture.hpp"
#include <map>

class JungleMode: public GameMode
{
    private:
        float timer1 = 0; // số giây đã trôi qua từ lần sự kiện trc (mọc ngẫu nhiên)
        float timer2 = 0; // số giây đã trôi qua từ lần sự kiện trc (mọc lan)
        float nextInterval1 = 15; // chu kỳ cho lần tiếp theo random(15,20), mọc ngẫu nhiên rễ cây 
        float nextInterval2 = 10; // chu kỳ cho lần tiếp theo random(10,15), các ô có rễ sẽ mọc lan ngẫu nhiên sang các hướng
        std::map<CellPoint,float> roots;
        const sf::Texture* rootTexture;
    public:
        JungleMode(AbstractBoard* board, Player* player,const sf::Texture& sandTexture);
        ~JungleMode();
        OpenResult CheckCellOpen(CellPoint cellPoint) const override; // trả về tình trạng mở ô 
        bool CanInteract(CellPoint cellPoint) const override;
        bool NeedHold(CellPoint cellPoint) const override;
        void OnTick(float deltaTime) override;
        void OnLeftHold(CellPoint cellPoint,float deltaTime) override;
        void RenderEffects(sf::RenderTarget& target) const override;
        void Reset() override;
        bool Load(std::istream& inputr) override;
        void Save(std::ostream& output) const override;
};