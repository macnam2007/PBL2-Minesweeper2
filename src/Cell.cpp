#include "../headers/Cell.hpp"

Cell::Cell() {}

bool Cell::OpenCell()
{
    if (this->isMine || !CanOpenCell())
    {return false;}
    this->state = CellState::Openned;
    return true;
}

bool Cell::CanOpenCell() const
{ return (this->state==CellState::Hidden); }

bool Cell::Openned() const
{ return (this->state==CellState::Openned); }

int Cell::GetNumber() const
{ return this->number; }

void Cell::MineToggle()
{ this->isMine = !this->isMine; }

void Cell::SetNumber(int number)
{ this->number = number; }

CellState Cell::GetState() const
{ return this->state; }

void Cell::ChangeState(CellState state)
{ this->state = state; }

void Cell::Reset()
{
    this->isMine = false;
    this->state = CellState::Hidden;
    this->number = 0;
}

bool Cell::CheckIsMine() const
{ return this->isMine; }