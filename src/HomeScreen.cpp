#include "../headers/HomeScreen.hpp"
#include "../headers/Game.hpp"
#include "../headers/SelectMapScreen.hpp"
#include "SFML/System/Vector2.hpp"

HomeScreen::HomeScreen(Game* game)
    :Screen(game), 
    btnPlay(game->GetResources().GetImage(ImageID::H_BtnPlay1),
            game->GetResources().GetImage(ImageID::H_BtnPlay2),
            game->GetResources().GetImage(ImageID::H_BtnPlay3),
            {640.f, 360.f}),
    btnExit(game->GetResources().GetImage(ImageID::H_BtnExit1),
            game->GetResources().GetImage(ImageID::H_BtnExit2),
            game->GetResources().GetImage(ImageID::H_BtnExit3),
            {640.f, 544.f}),
    spriteBg(game->GetResources().GetImage(ImageID::H_Bg)),
    spriteIcon1(game->GetResources().GetImage(ImageID::H_IconFlag)),
    spriteIcon2(game->GetResources().GetImage(ImageID::H_IconBomb))
{
    // Thêm nút vào danh sách quản lý
    AddButton(btnPlay); 
    AddButton(btnExit);

    // Thiết lập chức năng cho nút
    btnPlay.SetOnClick(
        [game] {
            game->GetResources().PlaySound(SoundID::BtnReleased);
            // lệnh sau khi bấm nút
            game->ChangeScreen(std::make_unique<SelectMapScreen>(game)); // giống như new SelectMapScreen(map); nhưng k cần phải xóa khi sài xong
        }
    );
    btnExit.SetOnClick(
        [game] { game->Quit(); }
    );
    
    // đặt vị trí của ảnh (Gốc của ảnh: góc trên bên trái của ảnh)
    this->spriteBg.setPosition({0.f,0.f});
    this->spriteIcon1.setPosition({810.f,350.f});
    this->spriteIcon1.setRotation(sf::degrees(20.45f)); // Xoay ảnh cho đẹp
    this->spriteIcon2.setPosition({384.f,559.f});
    // this->sprite.setPosition(position);static_cast<int>(ImgScreen::Bg)
}

void HomeScreen::Render(sf::RenderWindow& window)
{

    window.draw(spriteBg); // vẽ nền 
    RenderButtons(window); // vẽ nút
    window.draw(spriteIcon1); // vẽ icon lá cờ đè lên nút play 
    window.draw(spriteIcon2); // vẽ icon bom trên nút exit 
}