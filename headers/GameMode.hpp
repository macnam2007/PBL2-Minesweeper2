#pragma once

#include "CellPoint.hpp"
#include "EnumOpenResult.hpp"
#include "SFML/Graphics.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/Texture.hpp"

class AbstractBoard;
class Player;
class GameMode
{
    protected:
        AbstractBoard* board;
        Player* player;

    public:
        GameMode(AbstractBoard*, Player*);
        virtual  ~GameMode();
        virtual OpenResult CheckCellOpen(CellPoint cellPoint) const = 0; // trả về tình trạng mở ô 
        virtual bool CanInteract(CellPoint cellPoint) const; // chặn tương tác khi ô đó có rễ cây (Mode:jungle)
        virtual bool NeedHold(CellPoint cellPoint) const; // khi ô cần có tương tác giữ phím thì trả về true vd (cát phủ, rễ mọc)
        virtual void OnTick(float deltaTime) {}; // cập nhật thời gian 
        virtual void OnLeftHold(CellPoint cellPoint, float deltaTime) {}; // khi đã giữ n giây thì dùng hàm này thực hiện lệnh cần thiết
        virtual void RenderEffects(sf::RenderTarget& target) const {}; // vẽ cái những thứ đặc biệt của Mode đang chơi , đè lên Render của GameScreen 
        virtual bool IsBlockingInput() const; // chặn tương tác khi có hiệu ứng đặc biệt (Ong chích khi bị mở trúng ô có ong ở Bee's Hive )
        virtual void Reset() {};
        virtual bool Load(std::istream& input) { return true;};
        virtual void Save(std::ostream& output) const {};
        virtual bool OnHitMine(CellPoint cellPoint) { return false; }
};