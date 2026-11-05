#include "../headers/Timer.hpp"


void Timer::Start()
{
    this->timeElapsed = 0.f;
    this->running = true;
}
void Timer::Pause()
{ this->running = false; }

void Timer::Resume()
{ this->running = true; }

void Timer::Update(float deltaTime)
{ if (this->running) this->timeElapsed += deltaTime; }

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