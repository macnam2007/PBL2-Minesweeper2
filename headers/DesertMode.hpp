#include "AbstractBoard.hpp"
#include "CellPoint.hpp"
#include "GameMode.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/Texture.hpp"
#include <map>

class DesertMode : public GameMode
{       
    private:
        float timer = 0; // số giây đã trôi qua từ lần sự kiện trc
        float nextInterval = 8; // chu kỳ cho lần tiếp theo random(8,15), số ô bị che cũng random
        std::map<CellPoint,float> sands;
        const sf::Texture* sandTexture;
    public:
        DesertMode(AbstractBoard* board, Player* player,const sf::Texture& sandTexture);
        ~DesertMode();
        OpenResult CheckCellOpen(CellPoint cellPoint) const override; // trả về tình trạng mở ô 
        bool NeedHold(CellPoint cellPoint) const override;
        void OnTick(float deltaTime) override;
        void OnLeftHold(CellPoint cellPoint,float deltaTime) override;
        void RenderEffects(sf::RenderTarget& target) const override;
        void Reset() override;
        bool Load(std::istream& inputr) override;
        void Save(std::ostream& output) const override;
};