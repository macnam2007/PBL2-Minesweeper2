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
    btnReset(
        game->GetResources().GetImage(ImageID::G_BtnResetNormal),
        game->GetResources().GetImage(ImageID::G_BtnResetNormal),
        game->GetResources().GetImage(ImageID::G_BtnResetNormal),
        {640.f,140.f}
    ),
    spriteBg(game->GetResources().GetImage(ImageID::G_Bg)),
    overlay(game,this->overlayOn),
    PanelStat(
        game->GetResources().GetImage(ImageID::G_Panel),
        game->GetResources().GetFont(),
        {47.f,263.f}
    ),
    PanelResult(
        game->GetResources().GetImage(ImageID::G_Panel),
        game->GetResources().GetFont(),
        {903.f,263.f}
    )
{
    // Thêm nút vào danh sách quản lý
    AddButton(this->btnPause);
    AddButton(this->btnReset);

    // Thêm chức năng khi click nút
    this->btnPause.SetOnClick(
        [this] {
            this->game->GetResources().PlaySound(SoundID::BtnReleased);
            // lệnh thực hiện khi click chuột
            this->overlayOn = true;
        }
    );
    this->btnReset.SetOnClick(
        [this] {
            this->game->GetResources().PlaySound(SoundID::BtnReleased);
            this->timer.Reset();
            // lệnh thực hiện khi click chuột
            this->btnReset.UpdateImage(
                this->game->GetResources().GetImage(ImageID::G_BtnResetNormal),
                this->game->GetResources().GetImage(ImageID::G_BtnResetNormal),
                this->game->GetResources().GetImage(ImageID::G_BtnResetNormal)
            );
            this->board->ResetBoard();
        }
    );
    // Đặt vị trí của ảnh (Gốc của ảnh: góc trên bên trái)
    this->spriteBg.setPosition({0.f,0.f});

    // tạo dữ bàn chơi và quy luật
    CreateBoard();
    CreateMode();

    // đọc file;
    float time;
    int mapid = static_cast<int>(mapID), modeid = static_cast<int>(modeID);

    if (SaveManager::LoadGame(*this->board, *this->mode, mapid, modeid, time))
    {
        this->timer.SetTime(time);

    if (this->board->IsEnded())
        if (this->board->IsLosed())
        {
            this->btnReset.UpdateImage(
                this->game->GetResources().GetImage(ImageID::G_BtnResetLosed) ,
                this->game->GetResources().GetImage(ImageID::G_BtnResetLosed) ,
                this->game->GetResources().GetImage(ImageID::G_BtnResetLosed)
            );
        }
        else
        {
            this->btnReset.UpdateImage(
                this->game->GetResources().GetImage(ImageID::G_BtnResetWon) ,
                this->game->GetResources().GetImage(ImageID::G_BtnResetWon) ,
                this->game->GetResources().GetImage(ImageID::G_BtnResetWon)
            );
        }
    }
    else
    {
        CreateBoard();   // LoadGame lỗi giữa chừng có thể làm bàn dở dang, dựng lại cho sạch
        CreateMode();    // Tạo lại cho an toàn
    }

    this->bestRecord = SaveManager::GetRecord(mapid, modeid);


    // Thêm thông tin vào PanelStat
    this->PanelStat.SetTitle("STAT");
    std::vector<std::string> texts = {
        "3BV: " + std::to_string(this->board->GetCountClickTB()),
        "Best Score: " + std::to_string(this->bestRecord->score)
    };
    this->PanelStat.SetLines(texts);
}

void GameScreen::Render(sf::RenderWindow& window)
{
    window.draw(this->spriteBg);
    RenderGrid(window);
    RenderButtons(window);
    if (overlayOn)
    {this->overlay.Render(window);}

    if (this->board->IsEnded())
    {
        this->PanelStat.Render(window);
        this->PanelResult.Render(window);
        /* vẽ thông tin khi kết game */
    }
}

void GameScreen::HandleEvent(const sf::Event& event, sf::Vector2f mousePoint)
{
    if (this->overlayOn)
    {
        this->overlay.HandleEvent(event, mousePoint);
        return;
    }

    Screen::HandleEvent(event, mousePoint);

    // Tương tác với bàn cờ
    if (!this->board->IsEnded())
    {
        if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>())
        {
            std::optional<CellPoint> clickedPoint = this->board->MouseToCell(mousePoint);

            if (clickedPoint.has_value())
            {
                CellPoint point = clickedPoint.value();

                if (mousePressed->button == sf::Mouse::Button::Left)
                {
                    OpenResult res = this->board->OpenAt(point);
                    // Thiết lập hình ảnh nút reset nếu thua
                    switch(res)
                    {
                        case OpenResult::Lose:
                            this->btnReset.UpdateImage(
                                this->game->GetResources().GetImage(ImageID::G_BtnResetLosed) ,
                                this->game->GetResources().GetImage(ImageID::G_BtnResetLosed) ,
                                this->game->GetResources().GetImage(ImageID::G_BtnResetLosed)
                            );
                            break;
                        case OpenResult::Win:
                            this->btnReset.UpdateImage(
                                this->game->GetResources().GetImage(ImageID::G_BtnResetWon) ,
                                this->game->GetResources().GetImage(ImageID::G_BtnResetWon) ,
                                this->game->GetResources().GetImage(ImageID::G_BtnResetWon)
                            );
                            break;
                        default: break;
                    }
                }

                else if (mousePressed->button == sf::Mouse::Button::Right)
                {
                    this->board->ToggleMark(point);
                }
            }
        }
    }
}

void GameScreen::Update(float deltaTime,sf::Vector2f mousePoint)
{
    if (this->overlayOn)
    { this->overlay.Update(mousePoint); }
    else
    {
        Screen::Update(deltaTime,mousePoint);

        if (!this->board->IsGenerated() && !this->board->IsEnded())
        { this->timer.Update(deltaTime); }
    }
}

void GameScreen::CreateBoard()
{
    switch(this->mapID)
    {
        case MapID::Basic:
            switch(this->modeID)
            {
                case ModeID::Easy:  
                    this->board = std::make_unique<SquareBoard>(50,sf::Vector2f(415,200),9,9,10);
                    break;
                case ModeID::Normal:
                    this->board = std::make_unique<SquareBoard>(30,sf::Vector2f(400,188),16,16,40);
                    break;
                case ModeID::Hard:
                    this->board = std::make_unique<SquareBoard>(30,sf::Vector2f(210,188),30,16,99);
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
            sprite.setPosition(pointDraw);
            if (cell->GetState() == CellState::Openned)
            {
                // Vẽ ô nền của Cell (trạng thái mở)
                sprite.setTexture(this->game->GetResources().GetImage(ImageID::G_Openned));
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

    if (this->board->IsEnded() && this->board->IsLosed())
    {
        // vẽ các ô mìn khi thua
        for (int y = 0; y < this->board->GetRows(); y++)
        {
            for (int x = 0; x < this->board->GetCols(); x++)
            {
                Cell* cell = this->board->GetCell(CellPoint(x,y));
                sf::Vector2f pointDraw = this->board->CellToPixel(CellPoint(x,y));
                sprite.setPosition(pointDraw);
    
                // Hiện các ô mìn
                if (cell->CheckIsMine() && cell->GetState() != CellState::Flagged)
                {
                    sprite.setTexture(this->game->GetResources().GetImage(ImageID::G_Openned));
                    window.draw(sprite);
                    sprite.setTexture(this->game->GetResources().GetImage(ImageID::G_Bomb));
                    window.draw(sprite);
                }
                // Hiện các cờ đánh sai
                if (cell->GetState() == CellState::Flagged && !cell->CheckIsMine())
                {
                    sprite.setTexture(this->game->GetResources().GetImage(ImageID::G_Wrong));
                    window.draw(sprite);
                    sprite.setTexture(this->game->GetResources().GetImage(ImageID::G_Flag));
                    window.draw(sprite);
                }
            }
        }
    
        // vẽ quả mìn bị đẳm (đè lên ảnh cũ)
        for (const CellPoint& point : this->board->GetCellPointDead())
        {
            Cell* cell = this->board->GetCell(point);
            sf::Vector2f pointDraw = this->board->CellToPixel(point);
            sprite.setPosition(pointDraw);
    
            sprite.setTexture(this->game->GetResources().GetImage(ImageID::G_Wrong));
            window.draw(sprite);
            sprite.setTexture(this->game->GetResources().GetImage(ImageID::G_Bomb));
            window.draw(sprite);
        }
    }
}
