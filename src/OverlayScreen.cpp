#include "../headers/OverlayScreen.hpp"
#include "../headers/Game.hpp"
#include "../headers/HomeScreen.hpp"
#include "SFML/Graphics/RenderWindow.hpp"

OverlayScreen::OverlayScreen(Game* game, bool& overlayOn)
    : game(game),
    btnContinue(
        game->GetResources().GetImage(ImageID::OL_BtnContinue),
        game->GetResources().GetImage(ImageID::OL_BtnContinue),
        game->GetResources().GetImage(ImageID::OL_BtnContinue),
        {640.f,316.f}
    ),
    btnBackToHome(
        game->GetResources().GetImage(ImageID::OL_BtnBackToHome),
        game->GetResources().GetImage(ImageID::OL_BtnBackToHome),
        game->GetResources().GetImage(ImageID::OL_BtnBackToHome),
        {640.f,461.f}
    ),
    spriteBg(game->GetResources().GetImage(ImageID::OL_Bg))
{
    AddButton(this->btnContinue);
    AddButton(this->btnBackToHome);

    this->btnContinue.SetOnClick(
        [game,&overlayOn] {
            game->GetResources().PlaySound(SoundID::BtnReleased);
            // tương tác sau khi click chuột vào nút
            overlayOn = false;
        }
    );
    this->btnBackToHome.SetOnClick(
        [game] {
            game->GetResources().PlaySound(SoundID::BtnReleased);
            // tương tác sau khi click chuột vào nút
            game->ChangeScreen(std::make_unique<HomeScreen>(game));
        }
    );

    // Đặt vị trí ảnh (Gốc ảnh: bên trên trái)
    spriteBg.setPosition({287.f,122.f});
}

void OverlayScreen::AddButton(Button& button)
{ this->buttons.push_back(&button); }

void OverlayScreen::RenderButtons(sf::RenderTarget& target) const
{
    for (int i=0; i<this->buttons.size(); i++)
    { this->buttons[i]->Render(target); }
}

void OverlayScreen::ResetButton()
{
    for (int i=0; i<this->buttons.size(); i++)
    { this->buttons[i]->Reset(); }
}

void OverlayScreen::HandleEvent(const sf::Event& event, const sf::Vector2f mousePoint)
{
    for (int i=0; i<this->buttons.size(); i++)
    { this->buttons[i]->HandleEvent(event, mousePoint); }
}

void OverlayScreen::Update(sf::Vector2f mousePoint)
{
    for (int i=0; i<this->buttons.size(); i++)
    { this->buttons[i]->Update(mousePoint); }
}

void OverlayScreen::Render(sf::RenderWindow& window)
{
    window.draw(this->spriteBg);
    RenderButtons(window);
}