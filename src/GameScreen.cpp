#include "../headers/GameScreen.hpp"
#include "../headers/Game.hpp"
#include "../headers/SelectModeScreen.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Event.hpp"

#include "../headers/BasicMode.hpp"
#include "../headers/SquareBoard.hpp"

GameScreen::GameScreen(Game* game, MapID mapID, ModeID modeID)
    : Screen(game), mapID(mapID), modeID(modeID),
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

    // tạo dữ bàn chơi và quy luật
    CreateBoard();
    CreateMode();


}

void GameScreen::Render(sf::RenderWindow& window)
{
    window.draw(this->spriteBg);
    RenderGrid(window);
    RenderButtons(window);
    if (overlayOn)
    {this->overlay.Render(window);}
}

void GameScreen::HandleEvent(const sf::Event& event, sf::Vector2f mousePoint)
{
    if (this->overlayOn)
    {
        this->overlay.HandleEvent(event, mousePoint);
        return;
    }

    Screen::HandleEvent(event, mousePoint);

    if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>())
    {
        std::optional<CellPoint> clickedPoint = this->board->MouseToCell(mousePoint);

        if (clickedPoint.has_value())
        {
            CellPoint pt = clickedPoint.value();
            Cell* cell = this->board->GetCell(pt);

            // Kiểm tra nút trái (Trong SFML 3.0 enum được gom vào sf::Mouse::Button)
            if (mousePressed->button == sf::Mouse::Button::Left)
            {
                if (cell->GetState() == CellState::Hidden)
                {
                    this->board->Generate(pt); 
                    
                    if (cell->CheckIsMine()) {
                        cell->OpenCell(); // Boom!
                    } 
                    else if (cell->GetNumber() == 0) {
                        this->board->FloodFill(pt);
                    } 
                    else {
                        cell->OpenCell();
                    }
                }
            }
            // Kiểm tra nút phải
            else if (mousePressed->button == sf::Mouse::Button::Right)
            {
                if (cell->GetState() == CellState::Hidden) {
                    cell->ChangeState(CellState::Flagged);
                } 
                else if (cell->GetState() == CellState::Flagged) {
                    cell->ChangeState(CellState::Questioned);
                } 
                else if (cell->GetState() == CellState::Questioned) {
                    cell->ChangeState(CellState::Hidden);
                }
            }
        }
    }

}

void GameScreen::Update(float deltaTime,sf::Vector2f mousePoint)
{
    if (this->overlayOn)
    { this->overlay.Update(deltaTime, mousePoint); }
    else
    { Screen::Update(deltaTime, mousePoint); }
}

void GameScreen::CreateBoard()
{
    switch(this->mapID)
    {
        case MapID::Basic:
            switch(this->modeID)
            {
                case ModeID::Easy:  
                    this->board = std::make_unique<SquareBoard>(20,sf::Vector2f(100,200),9,9,10);
                    break;
                case ModeID::Normal:
                    this->board = std::make_unique<SquareBoard>(20,sf::Vector2f(100,200),16,16,40);
                    break;
                case ModeID::Hard:
                    this->board = std::make_unique<SquareBoard>(20,sf::Vector2f(100,200),30,16,99);
                    break;
                case ModeID::Explore:
                    //... chưa làm
                    break;
            }

            break;
        case MapID::Desert:

            break;
        case MapID::Jungle:

            break;
        case MapID::BeeHive:

            break;
    }
}

void GameScreen::CreateMode()
{
    // vì board lưu trong unique_ptr nền board.get() lấy AbstractBoard*
    this->mode = std::make_unique<BasicMode>(this->board.get(), &this->player);
}

void GameScreen::RenderGrid(sf::RenderWindow& window) const
{
    // chỉnh kích thước ảnh
    // Chọn 1 ảnh bất kỳ vì các ảnh điều như nhau
    sf::Sprite sprite(this->game->GetResources().GetImage(ImageID::G_Openned));

    // Lấy kích thước pixel gốc của tấm ảnh
    float originalWidth = static_cast<float>(sprite.getTexture().getSize().x);
    float originalHeight = static_cast<float>(sprite.getTexture().getSize().y);

    //tỷ lệ của ảnh mới so với ảnh gốc cho trục X và Y
    float scaleX = this->board->GetSizeCell() / originalWidth;
    float scaleY = this->board->GetSizeCell() / originalHeight; 

    // scale ảnh
    sprite.setScale({scaleX, scaleY});
    
    for (int y = 0; y < this->board->GetRows(); y++)
    {
        for (int x = 0; x < this->board->GetCols(); x++)
        {
            Cell* cell = this->board->GetCell(CellPoint(x,y));
            sf::Vector2f pointDraw = this->board->CellToPixel(CellPoint(x,y));
            if (cell->GetState() == CellState::Openned)
            {
                // Vẽ ô nền của Cell (trạng thái mở)
                sprite.setTexture(this->game->GetResources().GetImage(ImageID::G_Openned));
                sprite.setPosition(pointDraw);
                window.draw(sprite);
                if (cell->GetNumber() != 0)
                {
                    sprite.setTexture(this->game->GetResources().GetImage(static_cast<ImageID>(static_cast<int>(ImageID::G_number0)+cell->GetNumber())));
                    window.draw(sprite);
                }
                continue; 
            }
            // vẽ nền của ô Cell (trạng thái đóng)
            sprite.setTexture(this->game->GetResources().GetImage(ImageID::G_Hidded));
            sprite.setPosition(pointDraw);
            window.draw(sprite);
            
            switch(cell->GetState())
            {
                case CellState::Flagged:
                    sprite.setTexture(this->game->GetResources().GetImage(ImageID::G_Flag));
                    window.draw(sprite);
                    break;
                case CellState::Questioned:
                    sprite.setTexture(this->game->GetResources().GetImage(ImageID::G_Question));
                    window.draw(sprite);
                    break;
            }
        }
    }
}