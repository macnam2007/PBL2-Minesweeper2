#include "../headers/DesertMode.hpp"
#include <iostream>
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
{ return this->sands.count(cellPoint) > 0; }

void DesertMode::OnTick(float deltaTime)
{
    // tạo random
    static std::random_device rand;
    static std::mt19937 gen(rand());
    static std::uniform_int_distribution<int> dist1(CK_START, CK_STOP); // thời gian tiếp theo
    static std::uniform_int_distribution<int> dist2(0, 3);  // số cát
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

        // chọn ra em may mắn
        for (int i=0; i<count; i++)
        {
            std::uniform_int_distribution<int> dist3(0,openCells.size()-1);
            int k = dist3(gen);
            CellPoint select = openCells[k];
            openCells.erase(openCells.begin() + k); // xóa vị trí của ô vừa lấy

            // đặt cát
            this->sands[select] = 1.f; // thời gian cần giữ chuột để xóa cát
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
{
    /* Mô tả file data */
    // .. Thông tin của Board
    //6) timer nextInterval sandCount
    //7) sandCount dòng tiếp theo, mỗi dòng chứa : x y time


    //6) -----------------
    float timer, nextInterval, sandCount;
    if (!(input >> timer >> nextInterval >> sandCount))
    { std::cout<<"vitri1"; return false; }
    // kiểm tra dữ liệu
    if (timer <0 || nextInterval < CK_START || nextInterval >CK_STOP)
    { std::cout<<"vitri2"; return false; }

    //7) -----------------
    std::map<CellPoint,float> loaded;
    for (int i=0; i<sandCount; i++)
    {
        int x,y;
        float time;
        if (!(input >> x >> y >> time))
        { std::cout<<"vitri3"; return false; }
        // kiểm tra dữ liệu
        if (x<0 || x>=this->board->GetCols() || y<0 || y>=this->board->GetRows())
        { std::cout<<"vitri4"; return false; }
        loaded[CellPoint(x,y)] = time;
    }

    /*Vùng xác nhận dữ liệu*/
    this->timer = timer;
    this->nextInterval = nextInterval;
    this->sands = loaded;
    
    return true;
}

void DesertMode::Save(std::ostream& output) const
{
    output << this->timer << " " << nextInterval << " " << this->sands.size() << "\n";
    for (const auto& [point,time] : this->sands)
    { output << point.x << " " << point.y << " " << time << "\n"; }
}
