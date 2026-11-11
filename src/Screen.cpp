#include "../headers/Screen.hpp"
#include "../headers/Button.hpp"
#include "../headers/Game.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/System/Vector2.hpp"

Screen::Screen(Game* game)
    : game(game),
    btnSound(game->GetResources().GetImage(ImageID::All_BtnSoundOn),
            game->GetResources().GetImage(ImageID::All_BtnSoundOn),
            game->GetResources().GetImage(ImageID::All_BtnSoundOn),
            {1103.f,76.f}),
    btnMusic(game->GetResources().GetImage(ImageID::All_BtnMusicOn),
            game->GetResources().GetImage(ImageID::All_BtnMusicOn),
            game->GetResources().GetImage(ImageID::All_BtnMusicOn),
            {1201.f,76.f})
{
    // Thêm nút vào danh sách quản lý
    AddButton(btnSound);
    AddButton(btnMusic);

    // Nếu âm thanh/ nhạc đang tắt thì cập nhật lại ảnh (Trường hợp chuyển trang)
    if (!game->GetResources().SoundCheckOn())
    {
        btnSound.UpdateImage(
            game->GetResources().GetImage(ImageID::All_BtnSoundOff),
            game->GetResources().GetImage(ImageID::All_BtnSoundOff),
            game->GetResources().GetImage(ImageID::All_BtnSoundOff)
        );
    }
    if (!game->GetResources().MusicCheckOn())
    {
        game->GetResources().PlaySound(SoundID::BtnReleased);
        btnMusic.UpdateImage(
            game->GetResources().GetImage(ImageID::All_BtnMusicOff),
            game->GetResources().GetImage(ImageID::All_BtnMusicOff),
            game->GetResources().GetImage(ImageID::All_BtnMusicOff)
        );
    }

    // Thiết lập chức năng khi click vào nút
    btnSound.SetOnClick(
        [game,this] {
            if (game->GetResources().SoundCheckOn())
            {
                btnSound.UpdateImage(
                    game->GetResources().GetImage(ImageID::All_BtnSoundOff),
                    game->GetResources().GetImage(ImageID::All_BtnSoundOff),
                    game->GetResources().GetImage(ImageID::All_BtnSoundOff)
                );
            }
            else {
                btnSound.UpdateImage(
                    game->GetResources().GetImage(ImageID::All_BtnSoundOn),
                    game->GetResources().GetImage(ImageID::All_BtnSoundOn),
                    game->GetResources().GetImage(ImageID::All_BtnSoundOn)
                );
            }
            game->GetResources().ToggleSound();
        }
    );
    btnMusic.SetOnClick(
        [game,this] {
            if (game->GetResources().MusicCheckOn())
            {
                btnMusic.UpdateImage(
                    game->GetResources().GetImage(ImageID::All_BtnMusicOff),
                    game->GetResources().GetImage(ImageID::All_BtnMusicOff),
                    game->GetResources().GetImage(ImageID::All_BtnMusicOff)
                );
            }
            else {
                btnMusic.UpdateImage(
                    game->GetResources().GetImage(ImageID::All_BtnMusicOn),
                    game->GetResources().GetImage(ImageID::All_BtnMusicOn),
                    game->GetResources().GetImage(ImageID::All_BtnMusicOn)
                );
            }
            game->GetResources().ToggleMusic();
        }
    );
}

Screen::~Screen(){};

void Screen::AddButton(Button& button)
{ this->buttons.push_back(&button); }

void Screen::RenderButtons(sf::RenderTarget& target) const
{
    for (int i=0; i<this->buttons.size(); i++)
    { this->buttons[i]->Render(target); }
}

void Screen::ResetButton()
{
    for (int i=0; i<this->buttons.size(); i++)
    { this->buttons[i]->Reset(); }
}

void Screen::HandleEvent(const sf::Event& event, const sf::Vector2f mousePoint, float)
{
    for (int i=0; i<this->buttons.size(); i++)
    { this->buttons[i]->HandleEvent(event, mousePoint); }
}

void Screen::Update(float, sf::Vector2f mousePoint)     //  GameScreen sẽ dùng tới deltaTime 
{
    for (int i=0; i<this->buttons.size(); i++)
    { this->buttons[i]->Update(mousePoint); }
}