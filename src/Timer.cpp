#include "../headers/Timer.hpp"
#include "cmath"

void Timer::SetTime(float time)
{ this->timeElapsed = time; }

void Timer::Reset()
{
    this->timeElapsed = 0;
}

void Timer::Update(float deltaTime)
{ this->timeElapsed += deltaTime; }

float Timer::GetTime() const
{ return this->timeElapsed; }
