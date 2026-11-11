#pragma once

#include "ResourceManager.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/System/Vector2.hpp"
#include "SFML/Window/Event.hpp"
#include <SFML/Graphics.hpp>
#include <vector>
#include "Button.hpp"

class Game;

class Screen
{
    protected:
        Game* game;
        std::vector<Button*> buttons;

        Button btnSound;
        Button btnMusic;

        void AddButton(Button& button);
        void RenderButtons(sf::RenderTarget& target) const;

        // Phòng hờ các lỗi khi chuyển trang
        void ResetButton();

    public:
        explicit Screen(Game*); // chỉ có thể dùng để Dựng , k dùng để ép kiểu được 
        virtual ~Screen() = 0;

        // Cấm thực hiện các hành động sao chép Screen 
        Screen(const Screen&) = delete;               // vector giữ địa chỉ nút của chính nó
        Screen& operator=(const Screen&) = delete;    // nên không được sao chép

        /* kiểm tra sự kiện game */
        virtual void HandleEvent(const sf::Event& event, const sf::Vector2f mousePoint, float deltaTime);

        virtual void Update(float deltaTime, sf::Vector2f mousePoint);
        virtual void Render(sf::RenderWindow& window) = 0;
};