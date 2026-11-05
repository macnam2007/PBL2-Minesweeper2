#pragma once

class Timer
{
    private:
        float timeElapsed;
        bool running = false; // true kể từ khi click chuột đầu tiên
    
    public:
        Timer() {};

        void Start();
        void Pause();
        void Resume();
        void Update(float deltaTime);
        float GetTime() const;
        int GetMinutes();
        int GetSeconds();
    
};