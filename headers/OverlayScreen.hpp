#pragma once

#include "SFML/Graphics/RenderWindow.hpp"
#include "Screen.hpp"

class Game;

class OverlayScreen
{
    protected:
        Game* game;
        std::vector<Button*> buttons;

        Button btnContinue;
        Button btnBackToHome;

        sf::Sprite spriteBg;

        void AddButton(Button& button);
        void RenderButtons(sf::RenderTarget& target) const;

        // Phòng hờ các lỗi <chưa xác định lỗi>
        void ResetButton();

    public:
        explicit OverlayScreen(Game* game, bool& overlayOn); // chỉ có thể dùng để Dựng , k dùng để ép kiểu được 

        // Cấm thực hiện các hành động sao chép OverlayScreen 
        OverlayScreen(const OverlayScreen&) = delete;               // vector giữ địa chỉ nút của chính nó
        OverlayScreen& operator=(const OverlayScreen&) = delete;    // nên không được sao chép

        /* kiểm tra sự kiện game */
        void HandleEvent(const sf::Event& event, const sf::Vector2f mousePoint);

        void Update(float deltaTime, sf::Vector2f mousePoint);
        void Render(sf::RenderWindow& window);
};