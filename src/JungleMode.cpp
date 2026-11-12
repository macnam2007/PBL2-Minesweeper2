#include "../headers/JungleMode.hpp"
#include <random>

JungleMode::JungleMode(AbstractBoard* board, Player* player, const sf::Texture& rootTexture)
    : GameMode(board,player), rootTexture(&rootTexture)
{}

JungleMode::~JungleMode() {}

OpenResult JungleMode::CheckCellOpen(CellPoint cellPoint) const
{
    Cell* cell = this->board->GetCell(cellPoint);

    // Ô có đang bị mọc rễ hay không
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

bool JungleMode::CanInteract(CellPoint cellPoint) const
{ return !NeedHold(cellPoint); }

bool JungleMode::NeedHold(CellPoint cellPoint) const
{ return this->roots.count(cellPoint) > 0; }

void JungleMode::OnTick(float deltaTime)
{
    // tạo random
    static std::random_device rand;
    static std::mt19937 gen(rand());
    static std::uniform_int_distribution<int> dist1(15, 20); // thời gian tiếp theo mọc ngẫu nhiên
    static std::uniform_int_distribution<int> dist2(10, 15); // thời gian tiếp theo mọc lan
    static std::uniform_int_distribution<int> dist3(0, 2);  // số rễ mọc ngẫu nhiên
    static std::uniform_int_distribution<int> dist4(0, 5);  // số rễ mọc lan

    this->timer1 += deltaTime;
    this->timer2 += deltaTime;

    /*Mọc lan*/
    if (this->timer2 >= nextInterval2)
    {
        this->timer2 = 0;
        this->nextInterval2 = dist2(gen);
        
        // lấy các ô hàng xóm của các rễ cây
        int rows = this->board->GetRows();
        int cols = this->board->GetCols();
        std::vector<bool> visited(cols*rows, false);
        std::vector<CellPoint> neighborsCells;
        for (const auto& [point1,_] : this->roots)
        {
            std::vector<CellPoint> neighbors = this->board->GetNeighbors(point1);
            for (const auto& point2 : neighbors)
            {
                if (!visited[point2.y*cols + point2.x] && CanInteract(point1))
                {
                    visited[point2.y*cols + point2.x] = true;
                    neighborsCells.push_back(point2);
                }
            }
        }

        // random số lượng anh hàng xóm may mắn
        int count = dist4(gen);
        count = (count<=neighborsCells.size())?count:neighborsCells.size();

        // chọn ra anh hàng xóm may mắn
        for (int i=0; i<count; i++)
        {
            std::uniform_int_distribution<int> dist5(0,neighborsCells.size()-1);
            int k = dist5(gen);
            CellPoint select = neighborsCells[k];
            neighborsCells.erase(neighborsCells.begin() + k); // xoa anh hàng xóm may mắn trong danh sách

            // mọc rễ
            this->roots[select] = 0.5f; // thời gian cần giữ để phá rễ cây
        }
    }

    /*Mọc ngẫu nhiên*/
    if (timer1 > nextInterval1)
    {
        this->timer1 = 0;
        this->nextInterval1 = dist1(gen);

        // lấy các ô hiện đang trạng thái Hidded
        std::vector<CellPoint> hiddedCells;
        for (int y=0; y<this->board->GetRows(); y++)
        {
            for (int x=0; x<this->board->GetCols(); x++)
            {
                Cell* cell = this->board->GetCell(CellPoint(x,y));
                if (cell->GetState() == CellState::Hidden && CanInteract(CellPoint(x,y)))
                { hiddedCells.push_back(CellPoint(x,y)); }
            }
        }

        // random số lượng anh chàng bí ẩn may mắn
        int count = dist3(gen);
        count = (count<=hiddedCells.size())?count:hiddedCells.size();

        // chọn ra anh hàng xóm may mắn
        for (int i=0; i<count; i++)
        {
            std::uniform_int_distribution<int> dist5(0,hiddedCells.size()-1);
            int k = dist5(gen);
            CellPoint select = hiddedCells[k];
            hiddedCells.erase(hiddedCells.begin() + k); // xoa anh hàng xóm may mắn trong danh sách

            // mọc rễ
            this->roots[select] = 0.5f; // thời gian cần giữ để phá rễ cây
        }
    }
}

void JungleMode::OnLeftHold(CellPoint cellPoint, float deltaTime)
{
    if (NeedHold(cellPoint))
    {
        this->roots[cellPoint] -= deltaTime;
        if (this->roots[cellPoint] <= 0)
        { this->roots.erase(cellPoint); }
    }
}

void JungleMode::RenderEffects(sf::RenderTarget& target) const
{
    sf::Sprite sprite(*this->rootTexture);
    // dời về tâm
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f});

    // Lấy kích thước pixel gốc của tấm ảnh
    float originalWidth = static_cast<float>(sprite.getTexture().getSize().x);
    float originalHeight = static_cast<float>(sprite.getTexture().getSize().y);

    //tỷ lệ của ảnh mới so với ảnh gốc cho trục X và Y
    float scaleX = this->board->GetSizeCell() / originalWidth;
    float scaleY = this->board->GetSizeCell() / originalHeight; 

    // scale ảnh
    sprite.setScale({scaleX, scaleY});

    for (const auto& [point,_] : this->roots)
    {
        sf::Vector2f pointDraw = this->board->CellToPixel(point);
        sprite.setPosition(pointDraw);
        target.draw(sprite);
    }
}

void JungleMode::Reset()
{
    this->roots.clear();
    this->timer1 = 0;
    this->timer2 = 0;
    this->nextInterval1 = 0;
    this->nextInterval2 = 0;
}

bool JungleMode::Load(std::istream& input)
{ return true; }

void JungleMode::Save(std::ostream& output) const
{}
