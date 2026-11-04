#include "../headers/SelectMapScreen.hpp"
#include "../headers/SelectModeScreen.hpp"
#include "../headers/Game.hpp"
#include "../headers/GameScreen.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/System/Vector2.hpp"

SelectModeScreen::SelectModeScreen(Game* game, MapID map, ModeID mode)
    : Screen(game),map(map),mode(mode),
    btnBack(
        game->GetResources().GetImage(ImageID::SMode_BtnBack),
        game->GetResources().GetImage(ImageID::SMode_BtnBack),
        game->GetResources().GetImage(ImageID::SMode_BtnBack),
        {79.f,76.f}),
    btnEasy(
        game->GetResources().GetImage(ImageID::SMode_BtnEasy),
        game->GetResources().GetImage(ImageID::SMode_BtnEasy),
        game->GetResources().GetImage(ImageID::SMode_BtnEasy),
        {243.f,200.f}),
    btnNormal(
        game->GetResources().GetImage(ImageID::SMode_BtnNormal),
        game->GetResources().GetImage(ImageID::SMode_BtnNormal),
        game->GetResources().GetImage(ImageID::SMode_BtnNormal),
        {243.f,320.f}),
    btnHard(
        game->GetResources().GetImage(ImageID::SMode_BtnHard),
        game->GetResources().GetImage(ImageID::SMode_BtnHard),
        game->GetResources().GetImage(ImageID::SMode_BtnHard),
        {243.f,440.f}),
    btnExplore(
        game->GetResources().GetImage(ImageID::SMode_BtnExplore),
        game->GetResources().GetImage(ImageID::SMode_BtnExplore),
        game->GetResources().GetImage(ImageID::SMode_BtnExplore),
        {243.f,560.f}),
    btnStart(
        game->GetResources().GetImage(ImageID::SMode_BtnStart),
        game->GetResources().GetImage(ImageID::SMode_BtnStart),
        game->GetResources().GetImage(ImageID::SMode_BtnStart),
        {813.f,519.f}),
    spriteBg(game->GetResources().GetImage(ImageID::SMode_Bg)),
    spriteBoxInfo(game->GetResources().GetImage(ImageID::SMode_BoxInfo))
{
    // Thêm nút vào danh sách quản lý
    AddButton(btnBack);
    AddButton(btnEasy);
    AddButton(btnNormal);
    AddButton(btnHard);
    AddButton(btnStart);
    if (map!=MapID::BeeHive)
    { AddButton(btnExplore); }
    else { // Cập nhật lại vị trí các nút cho Map Bee's Hive
        btnEasy.UpdatePosition({243.f,240.f});
        btnNormal.UpdatePosition({243.f,382.f});
        btnHard.UpdatePosition({243.f,524.f});
    }

    // Thiết lập chức năng cho nút
    btnBack.SetOnClick(
        [game] {
            game->GetResources().PlaySound(SoundID::BtnReleased);
            // Lệnh sau khi bấm nút
            game->ChangeScreen(std::make_unique<SelectMapScreen>(game));
        }
    );
    btnEasy.SetOnClick(
        [game,this] {
            game->GetResources().PlaySound(SoundID::BtnReleased);
            // Lệnh sau khi bấm nút
            this->mode = ModeID::Easy;
        }
    );
    btnNormal.SetOnClick(
        [game,this] {
            game->GetResources().PlaySound(SoundID::BtnReleased);
            // Lệnh sau khi bấm nút
            this->mode = ModeID::Normal;
        }
    );
    btnHard.SetOnClick(
        [game,this] {
            game->GetResources().PlaySound(SoundID::BtnReleased);
            // Lệnh sau khi bấm nút
            this->mode = ModeID::Hard;
        }
    );
    btnExplore.SetOnClick(
        [game,this] {
            game->GetResources().PlaySound(SoundID::BtnReleased);
            // Lệnh sau khi bấm nút
            this->mode = ModeID::Explore;
        }
    );
    btnStart.SetOnClick(
        [game,this] {
            game->GetResources().PlaySound(SoundID::BtnReleased);
            // Lệnh sau khi bấm nút
            game->ChangeScreen(std::make_unique<GameScreen>(game,this->map,this->mode));
        }
    );

    // đặt vị trí của ảnh (Gốc ảnh: góc trên bên trái)
    this->spriteBg.setPosition({0.f,0.f});
    this->spriteBoxInfo.setPosition({512.f,152.f});
}

void SelectModeScreen::Render(sf::RenderWindow& window)
{
    window.draw(this->spriteBg);
    window.draw(this->spriteBoxInfo);
    RenderButtons(window);
}