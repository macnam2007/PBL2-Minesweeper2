#include "../headers/GameScreen.hpp"
#include "../headers/Game.hpp"
#include "../headers/SelectModeScreen.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Event.hpp"

GameScreen::GameScreen(Game* game, MapID map, ModeID mode)
    : Screen(game), map(map), mode(mode),
    btnPause(
        game->GetResources().GetImage(ImageID::G_BtnPause),
        game->GetResources().GetImage(ImageID::G_BtnPause),
        game->GetResources().GetImage(ImageID::G_BtnPause),
        {79.f,76.f}
    ),
    spriteBg(game->GetResources().GetImage(ImageID::G_Bg)),
    overlay(game,this->overlayOn)
{
    // Thêm nút vào danh sách quản lý
    AddButton(this->btnPause);

    // Thêm chức năng khi click nút
    this->btnPause.SetOnClick(
        [game,this] {
            game->GetResources().PlaySound(SoundID::BtnReleased);
            // lệnh thực hiện khi click chuột
            this->overlayOn = true;
        }
    );
    // Đặt vị trí của ảnh (Gốc của ảnh: góc trên bên trái)
    this->spriteBg.setPosition({0.f,0.f});

}

void GameScreen::Render(sf::RenderWindow& window)
{
    window.draw(this->spriteBg);
    RenderButtons(window);
    if (overlayOn)
    {this->overlay.Render(window);}
}

void GameScreen::HandleEvent(const sf::Event& event, sf::Vector2f mousePoint)
{
    if (this->overlayOn)
    { this->overlay.HandleEvent(event, mousePoint); }
    else
    { Screen::HandleEvent(event, mousePoint); }
}

void GameScreen::Update(float deltaTime,sf::Vector2f mousePoint)
{
    if (this->overlayOn)
    { this->overlay.Update(deltaTime, mousePoint); }
    else
    { Screen::Update(deltaTime, mousePoint); }
}