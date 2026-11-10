#pragma once

class Timer
{
    private:
        float timeElapsed;
    
    public:
        Timer() {};

        void SetTime(float time);
        void Reset();
        void Update(float deltaTime);
        float GetTime() const;
        int GetMinutes();
        int GetSeconds();
    
};