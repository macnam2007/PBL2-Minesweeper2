#include "../headers/Timer.hpp"

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

int Timer::GetMinutes()
{
    int time = static_cast<int>(this->timeElapsed);
    return time / 60;
}

int Timer::GetSeconds()
{
    int time = static_cast<int>(this->timeElapsed);
    return time % 60;
}
