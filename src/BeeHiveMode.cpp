#include "../headers/BeeHiveMode.hpp"
#include "SFML/Graphics/RenderTarget.hpp"

BeeHiveMode::BeeHiveMode(AbstractBoard* board, Player* player)
    : GameMode(board,player)
{}

BeeHiveMode::~BeeHiveMode() {}

OpenResult BeeHiveMode::CheckCellOpen(CellPoint cellPoint) const
{
    Cell* cell = this->board->GetCell(cellPoint);
    // Ô cần mở đang được đánh dấu
    if(
        cell->GetState() == CellState::Flagged ||
        cell->GetState() == CellState::Questioned ||
        (cell->GetState() == CellState::Openned && cell->GetNumber() == 0)
    ) { return OpenResult::Blocked; }

    { return OpenResult::Safe; } // board bên ngoài nhận và tự kiểm tra nên mở như thế nào
}

void BeeHiveMode::Reset()
{
    this->life = LIFE;
    this->stung = false;
}

bool BeeHiveMode::Load(std::istream& input)
{
    int life;
    bool stung;

    if (!(input >> life >> stung))
    { return false;}

    this->life = life;
    this->stung = stung;
    return true;
}

void BeeHiveMode::Save(std::ostream& output) const
{
    output << this->life <<" "<<this->stung<<"\n";
}

bool BeeHiveMode::OnHitMine(CellPoint cellPoint)
{
    if (this->life > 0)
    {
        this->life--;
        this->stung = true;
        this->RemoveMine(cellPoint);
        this->board->GetCell(cellPoint)->OpenCell();
        return true;
    }
    return false;
}

void BeeHiveMode::RemoveMine(CellPoint cellPoint)
{
    std::vector<CellPoint> neighbors = this->board->GetNeighbors(cellPoint);
    for (const CellPoint& point : neighbors)
    {
        Cell* cell = this->board->GetCell(point);
        cell->SetNumber(cell->GetNumber()-1);
    }
    this->board->GetCell(cellPoint)->MineToggle();
    this->life--;
}

void BeeHiveMode::RenderEffects(sf::RenderTarget& target) const
{}


bool BeeHiveMode::IsBlockingInput() const // chặng tương tác khi xuất hiện hiệu ứng ong chính
{ return false;}