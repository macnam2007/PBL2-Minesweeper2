#include "../headers/DesertMode.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/Texture.hpp"
#include <random>

DesertMode::DesertMode(AbstractBoard* board, Player* player, const sf::Texture& sandTexture)
    : GameMode(board, player), sandTexture(&sandTexture)
{}

DesertMode::~DesertMode() {}

OpenResult DesertMode::CheckCellOpen(CellPoint cellPoint) const
{
    Cell* cell = this->board->GetCell(cellPoint);

    // Ô có đang bị cát phủ hay không
    if (NeedHold(cellPoint))
    { return OpenResult::Blocked; }

    // Ô cần mở đang được đánh dấu
    if(
        cell->GetState() == CellState::Flagged ||
        cell->GetState() == CellState::Questioned ||
        (cell->GetState() == CellState::Openned && cell->GetNumber() == 0)
    ) { return OpenResult::Blocked; }

    { return OpenResult::Safe; } // board bên ngoài nhận và tự kiểm tra nên mở như thế nào
}

bool DesertMode::NeedHold(CellPoint cellPoint) const
{
    return this->sands.count(cellPoint) > 0;
}

void DesertMode::OnTick(float deltaTime)
{
    // tạo random
    static std::random_device rand;
    static std::mt19937 gen(rand());
    static std::uniform_int_distribution<int> dist1(8, 15); // thời gian tiếp theo
    static std::uniform_int_distribution<int> dist2(0, 3);  // số mìn
    this->timer += deltaTime;
    if (timer >= nextInterval)
    {
        this->nextInterval = dist1(gen);
        this->timer = 0;

        // lấy các ô hiện đang là số k bị che
        std::vector<CellPoint> openCells;
        for (int y=0; y<this->board->GetRows(); y++)
        {
            for (int x=0; x<this->board->GetCols(); x++)
            {
                Cell* cell = this->board->GetCell(CellPoint(x,y));
                if (cell->Openned() && cell->GetNumber()!=0 && !NeedHold(CellPoint(x,y)))
                { openCells.push_back(CellPoint(x,y)); }
            }
        }
        
        // random số ô cát cần tạo
        int count = dist2(gen);
        count = (count<=openCells.size())?count:openCells.size();

        // .. tạo cát 
        while (count)
        {
            count--;
            std::uniform_int_distribution<int> dist3(0,openCells.size()-1);
            // chọn ra 1 em may mắn
            int k = dist3(gen);
            CellPoint select = openCells[k];
            openCells.erase(openCells.begin() + k); // xóa vị trí của ô vừa lấy

            // đặt cát
            this->sands[select] = 2; // thời gian cần giữ chuột để xóa cát
        }
    }
}

void DesertMode::OnLeftHold(CellPoint cellPoint,float deltaTime)
{ 
    if (NeedHold(cellPoint))
    {
        this->sands[cellPoint] -= deltaTime;
        if (this->sands[cellPoint] <= 0)
        { this->sands.erase(cellPoint); }
    }
}

void DesertMode::RenderEffects(sf::RenderTarget& target) const
{
    sf::Sprite sprite(*this->sandTexture);

    // Lấy kích thước pixel gốc của tấm ảnh
    float originalWidth = static_cast<float>(sprite.getTexture().getSize().x);
    float originalHeight = static_cast<float>(sprite.getTexture().getSize().y);

    //tỷ lệ của ảnh mới so với ảnh gốc cho trục X và Y
    float scaleX = this->board->GetSizeCell() / originalWidth;
    float scaleY = this->board->GetSizeCell() / originalHeight; 

    // scale ảnh
    sprite.setScale({scaleX, scaleY});

    for (const auto& [point,_] : this->sands)
    {
        sf::Vector2f pointDraw = this->board->CellToPixel(point);
        sprite.setPosition(pointDraw);
        target.draw(sprite);
    }
}

void DesertMode::Reset()
{
    this->sands.clear();
    this->timer = 0;
    this->nextInterval = 8;
}

bool DesertMode::Load(std::istream& input)
{ return true;}

void DesertMode::Save(std::ostream& output) const
{}
