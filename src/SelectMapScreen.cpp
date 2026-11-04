#include "../headers/SelectMapScreen.hpp"
#include "../headers/SelectModeScreen.hpp"
#include "../headers/Game.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/System/Vector2.hpp"
#include "../headers/HomeScreen.hpp"

SelectMapScreen::SelectMapScreen(Game* game)
    : Screen(game),
    btnBack(game->GetResources().GetImage(ImageID::SMap_BtnBack),
            game->GetResources().GetImage(ImageID::SMap_BtnBack),
            game->GetResources().GetImage(ImageID::SMap_BtnBack),
            {79.f,76.f}),
    btnBasic(game->GetResources().GetImage(ImageID::SMap_BtnBasic),
            game->GetResources().GetImage(ImageID::SMap_BtnBasic),
            game->GetResources().GetImage(ImageID::SMap_BtnBasic),
            {208.f,410.f}),
    btnDesert(game->GetResources().GetImage(ImageID::SMap_BtnDesert),
            game->GetResources().GetImage(ImageID::SMap_BtnDesert),
            game->GetResources().GetImage(ImageID::SMap_BtnDesert),
            {496.f,410.f}),
    btnJungle(game->GetResources().GetImage(ImageID::SMap_BtnJungle),
            game->GetResources().GetImage(ImageID::SMap_BtnJungle),
            game->GetResources().GetImage(ImageID::SMap_BtnJungle),
            {789.f,410.f}),
    btnBeeHive(game->GetResources().GetImage(ImageID::SMap_BtnBeeHive),
            game->GetResources().GetImage(ImageID::SMap_BtnBeeHive),
            game->GetResources().GetImage(ImageID::SMap_BtnBeeHive),
            {1072.f,410.f}),
    spriteBg(game->GetResources().GetImage(ImageID::SMap_Bg))
{
    // Thêm nút vào danh sách quản lý
    AddButton(btnBack);
    AddButton(btnBasic);
    AddButton(btnDesert);
    AddButton(btnJungle);
    AddButton(btnBeeHive);

    // Thiết lập chức năng cho nút
    btnBack.SetOnClick(
        [game] {
            game->GetResources().PlaySound(SoundID::BtnReleased);
            // lệnh sau khi bấm nút
            game->ChangeScreen(std::make_unique<HomeScreen>(game)); // giống như new HomeScreen(map); nhưng k cần phải xóa khi sài xong
        }
    );
    btnBasic.SetOnClick(
        [game] {
            game->GetResources().PlaySound(SoundID::BtnReleased);
            // lệnh sau khi bấm nút
            game->ChangeScreen(std::make_unique<SelectModeScreen>(game,MapID::Basic));
        }
    );
    btnDesert.SetOnClick(
        [game] {
            game->GetResources().PlaySound(SoundID::BtnReleased);
            // lệnh sau khi bấm nút
            game->ChangeScreen(std::make_unique<SelectModeScreen>(game,MapID::Desert));
        }
    );
    btnJungle.SetOnClick(
        [game] {
            game->GetResources().PlaySound(SoundID::BtnReleased);
            // lệnh sau khi bấm nút
            game->ChangeScreen(std::make_unique<SelectModeScreen>(game,MapID::Jungle));
        }
    );
    btnBeeHive.SetOnClick(
        [game] {
            game->GetResources().PlaySound(SoundID::BtnReleased);
            // lệnh sau khi bấm nút
            game->ChangeScreen(std::make_unique<SelectModeScreen>(game,MapID::BeeHive));
        }
    );

    // Đặt vị trí của ảnh (Gốc của ảnh: góc trên bên trái)
    this->spriteBg.setPosition({0.f,0.f});
}

void SelectMapScreen::Render(sf::RenderWindow& window)
{
    window.draw(spriteBg);// vẽ nền
    RenderButtons(window);// vẽ nút
}