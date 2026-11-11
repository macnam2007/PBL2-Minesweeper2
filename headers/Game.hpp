#pragma once

#include <memory> //unique_ptr
#include <SFML/Graphics.hpp>
#include "ResourceManager.hpp"
#include "Screen.hpp"

class Game
{
    private:
        void ChangePendingScreen();

        sf::RenderWindow window;
        ResourceManager resource;        // Game sẽ hủy theo thứ tự : pending -> current -> resource -> window
        std::unique_ptr<Screen> current; // |tương đương với
        std::unique_ptr<Screen> pending; // |Screen* current

        sf::Clock clock;
        float deltaTime = 0.f;
        bool running = true;

    public:
        Game();
        ~Game();

        // Cấm các trường hợp Copy vì game là duy nhất 
        Game(const Game&) = delete;
        Game& operator=(const Game&) = delete;

        void Run();
        void ChangeScreen(std::unique_ptr<Screen> next);  
        void Quit();

        bool ToggleMusic();
        bool ToggleVoice();

        ResourceManager&  GetResources() { return resource; } // Screen lấy ảnh, font
        sf::RenderWindow& GetWindow()    { return window; }
};