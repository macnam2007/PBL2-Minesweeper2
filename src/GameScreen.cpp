#include "../headers/GameScreen.hpp"
#include "../headers/Game.hpp"
#include "../headers/SelectModeScreen.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Window/Event.hpp"
#include "../headers/BasicMode.hpp"
#include "../headers/DesertMode.hpp"
#include "../headers/JungleMode.hpp"
#include "../headers/BeeHiveMode.hpp"
#include "../headers/SquareBoard.hpp"
#include "../headers/HexBoard.hpp"
#include <cstddef>
#include <optional>
#include <string>
#include <cmath>

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
    spriteFrame(game->GetResources().GetImage(ImageID::G_Frame)),
    overlay(game,this->overlayOn),
    PanelStat(
        game->GetResources().GetImage(ImageID::G_Panel),
        game->GetResources().GetFont(FontID::PressStart2P),
        {47.f,263.f}
    ),
    PanelResult(
        game->GetResources().GetImage(ImageID::G_Panel),
        game->GetResources().GetFont(FontID::PressStart2P),
        {944.f,263.f}
    ),
    textTimer(
        game->GetResources().GetFont(FontID::DigitalNumber),
        "00:00",
        26 
    ),
    textMineLeft(
        game->GetResources().GetFont(FontID::DigitalNumber), 
        "00", 
        26
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
            SaveManager::SaveGame(this->board.get(), this->mode.get(), static_cast<int>(this->mapID), static_cast<int>(this->modeID), this->timer.GetTime());
        }
    );
    this->btnReset.SetOnClick(
        [this] {
            this->game->GetResources().PlaySound(SoundID::BtnReleased);
            // lệnh thực hiện khi click chuột
            this->timer.Reset();
            this->mode->Reset();
            this->btnReset.UpdateImage(
                this->game->GetResources().GetImage(ImageID::G_BtnResetNormal),
                this->game->GetResources().GetImage(ImageID::G_BtnResetNormal),
                this->game->GetResources().GetImage(ImageID::G_BtnResetNormal)
            );
            this->board->ResetBoard();
        }
    );
    // Đặt vị trí của ảnh nền (Gốc của ảnh: góc trên bên trái)
    this->spriteBg.setPosition({0.f,0.f});

    // Đặt vị trí thông tin thời gian , số mìn còn lại
    // Text hiển thị Đồng hồ
    this->textTimer.setFillColor(sf::Color::Black);
    // dời gốc ảnh về tâm
    sf::FloatRect bounds = this->textTimer.getLocalBounds();
    this->textTimer.setOrigin({
        bounds.position.x + bounds.size.x / 2.0f, 
        bounds.position.y + bounds.size.y / 2.0f
    });
    this->textTimer.setPosition({493.f, 140.f});

    // Text hiển thị số mìn còn lại
    this->textMineLeft.setFillColor(sf::Color::Black);
    // dời gốc ảnh về tâm
    bounds = this->textMineLeft.getLocalBounds(); 
    this->textMineLeft.setOrigin({
        bounds.position.x + bounds.size.x / 2.0f, 
        bounds.position.y + bounds.size.y / 2.0f
    });
    this->textMineLeft.setPosition({787.f, 140.f});

    // tạo dữ bàn chơi và quy luật
    CreateBoard();
    CreateMode();

    // đọc file;
    float time;
    int mapid = static_cast<int>(mapID), modeid = static_cast<int>(modeID);

    // đọc thông tin đã lưu nếu có (đọc trước vì Panel của bestRecord cần biết là đã từng có kỉ lục hay chưa)
    this->bestRecord = SaveManager::GetRecord(mapid, modeid);

    if (SaveManager::LoadGame(*this->board, *this->mode, mapid, modeid, time))
    {
        this->timer.SetTime(time);
        // nếu màn trc đó đã end thì bổ xung thông tin cho PanelResult

        if (this->board->IsEnded() && this->board->IsLosed())
        {
            this->btnReset.UpdateImage(
                this->game->GetResources().GetImage(ImageID::G_BtnResetLosed) ,
                this->game->GetResources().GetImage(ImageID::G_BtnResetLosed) ,
                this->game->GetResources().GetImage(ImageID::G_BtnResetLosed)
            );
            this->PanelResult.SetTitle("YOU LOSED");
            std::vector<std::string> texts = {
            "Time: " + FormatFloatTime(this->timer.GetTime()) +" sec",
            "3BV: " + std::to_string(this->board->GetCountClick()) + "/" + std::to_string(this->board->GetCountClickTB()),
            "3BV/s: " + std::to_string(this->board->GetCountClick()/this->timer.GetTime()),
            "Clicks: " + std::to_string(this->board->GetCountClick())
            };
            this->PanelResult.SetLines(texts);
        }
        else if (this->board->IsEnded() && !this->board->IsLosed())
        {
            this->btnReset.UpdateImage(
                this->game->GetResources().GetImage(ImageID::G_BtnResetWon) ,
                this->game->GetResources().GetImage(ImageID::G_BtnResetWon) ,
                this->game->GetResources().GetImage(ImageID::G_BtnResetWon)
            );
            this->PanelResult.SetTitle("YOU WON");
            float A = float(this->board->GetCountClickTB())/this->board->GetCountClick();
            float B = float(this->board->GetCountClickTB())/this->timer.GetTime();
            int score = A*B*1000;
            score = score<0?0:score;

            std::vector<std::string> texts = {
            "Time: " + FormatFloatTime(this->timer.GetTime()) +" sec",
            "3BV: " + std::to_string(this->board->GetCountClick()) + "/" + std::to_string(this->board->GetCountClickTB()),
            "3BV/s: " + std::to_string(this->board->GetCountClick()/this->timer.GetTime()),
            "Clicks: " + std::to_string(this->board->GetCountClick()),
            "Score: " + std::to_string(score),
            };

            if (this->bestRecord == std::nullopt || score > this->bestRecord->score || (score == this->bestRecord->score && this->timer.GetTime() < this->bestRecord->time))
            { texts.push_back("<<New Best Score>>");}

            this->PanelResult.SetLines(texts);
        }
    }
    else
    {
        std::cout<<"Không load đc Màn chơi!!!\n";
        CreateBoard();   // LoadGame lỗi giữa chừng có thể làm bàn dở dang, dựng lại cho sạch
        CreateMode();    // Tạo lại cho an toàn
        this->timer.Reset();
    }

    // Thêm thông tin vào PanelStat
    this->PanelStat.SetTitle("STAT");
    std::vector<std::string> texts = {
        "3BV: " + std::to_string(this->board->GetCountClick()) + "/" + std::to_string(this->board->GetCountClickTB())
    };
    // nếu lần đầu chơi thì k có best score
    if (this->bestRecord != std::nullopt)
    {
        texts.push_back("Best Score: " + std::to_string(this->bestRecord->score));
        texts.push_back("Time: " + FormatFloatTime(this->bestRecord->time));
    }
    else {texts.push_back("Best Score: Null");}
    this->PanelStat.SetLines(texts);

    if (this->board->IsEnded())
    {
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
}

void GameScreen::Render(sf::RenderWindow& window)
{
    // vẽ nền
    window.draw(this->spriteBg);
    // vẽ thời gian
    this->spriteFrame.setPosition({418.f,110.f});
    window.draw(this->spriteFrame);
    window.draw(this->textTimer);
    // vẽ số mìn còn lại
    this->spriteFrame.setPosition({697.f,110.f});
    window.draw(this->spriteFrame);
    window.draw(this->textMineLeft);
    // vẽ bảng trò chơi
    RenderGrid(window);
    // vẽ đè lên của các GameMode
    this->mode->RenderEffects(window);
    // vẽ các nút trên màn hình
    RenderButtons(window);

    if (overlayOn)
    {this->overlay.Render(window);}

    if (this->board->IsEnded())
    {
        /* vẽ thông tin khi kết game */
        this->PanelStat.Render(window);
        this->PanelResult.Render(window);
    }
}

void GameScreen::HandleEvent(const sf::Event& event, sf::Vector2f mousePoint, float deltaTime)
{
    if (this->overlayOn)
    {
        this->overlay.HandleEvent(event, mousePoint);
        return;
    }

    Screen::HandleEvent(event, mousePoint,deltaTime);

    // Tương tác với bàn cờ
    if (!this->board->IsEnded())
    {
        if (const auto* mousePressed = event.getIf<sf::Event::MouseButtonPressed>())
        {
            std::optional<CellPoint> clickedPoint = this->board->MouseToCell(mousePoint);

            if (clickedPoint.has_value())
            {
                CellPoint point = clickedPoint.value();

                // Mode hiện tại có đang cho phép mở ô này hoặc lân cận hay không
                if (mousePressed->button == sf::Mouse::Button::Left && this->mode->CanInteract(point))
                {
                    OpenResult res = this->mode->CheckCellOpen(point); // nhận 3 giá trị Safe Blocked

                    if (res == OpenResult::Safe)
                    {
                        res = this->board->OpenAt(point);
                        this->board->UpCountCLick();
                    }

                    // Thiết lập hình ảnh nút reset khi thua/thắng, PanelResult
                    switch(res)
                    {
                        case OpenResult::Win:
                        {
                            this->btnReset.UpdateImage(
                                this->game->GetResources().GetImage(ImageID::G_BtnResetWon) ,
                                this->game->GetResources().GetImage(ImageID::G_BtnResetWon) ,
                                this->game->GetResources().GetImage(ImageID::G_BtnResetWon)
                            );
                            this->PanelResult.SetTitle("YOU WON");
                            float A = float(this->board->GetCountClickTB())/(this->board->GetCountClick()); // kiểu j cũng > 1 :))) thiết lập cả rồi
                            float B = float(this->board->GetCountClickTB())/(this->timer.GetTime()==0?1:this->timer.GetTime()); // lở chơi win với tốc độ siêu thành
                            int score = A*B*1000;
                            score = score<0?0:score;

                            float currentTime = this->timer.GetTime();
                            float BVs = (currentTime > 0.0f) ? (this->board->GetCountClickTB() / currentTime) : 0.0f;

                            std::vector<std::string> texts = {
                            "Time: " + FormatFloatTime(this->timer.GetTime()) +" sec",
                            "3BV: " + std::to_string(this->board->GetCountClick()) + "/" + std::to_string(this->board->GetCountClickTB()),
                            "3BV/s: " + FormatFloatTime(BVs),
                            "Clicks: " + std::to_string(this->board->GetCountClick()),
                            "Score: " + std::to_string(score),
                            };

                            if (this->bestRecord == std::nullopt || score > this->bestRecord->score || (score == this->bestRecord->score && this->timer.GetTime() < this->bestRecord->time))
                            { texts.push_back("<<New Best Score>>");}

                            this->PanelResult.SetLines(texts);

                            // lưu kỉ lục mới nếu có
                            if (!this->bestRecord || score > this->bestRecord->score || 
                            (score == this->bestRecord->score && this->timer.GetTime() < this->bestRecord->time)
                            ) {  SaveManager::UpdateRecord(static_cast<int>(this->mapID), static_cast<int>(this->modeID), score, this->timer.GetTime()); }

                            SaveManager::SaveGame(this->board.get(), this->mode.get(), static_cast<int>(this->mapID), static_cast<int>(this->modeID), this->timer.GetTime());
                            break;
                        }
                        case OpenResult::Lose:
                        {
                            this->btnReset.UpdateImage(
                                this->game->GetResources().GetImage(ImageID::G_BtnResetLosed) ,
                                this->game->GetResources().GetImage(ImageID::G_BtnResetLosed) ,
                                this->game->GetResources().GetImage(ImageID::G_BtnResetLosed)
                            );

                            float currentTime = this->timer.GetTime();
                            float BVs = (currentTime > 0.0f) ? (this->board->GetCountClickTB() / currentTime) : 0.0f;

                            this->PanelResult.SetTitle("YOU LOSED");
                            std::vector<std::string> texts = {
                            "Time: " + FormatFloatTime(this->timer.GetTime()) +" sec",
                            "3BV: " + std::to_string(this->board->GetCountClick()) + "/" + std::to_string(this->board->GetCountClickTB()),
                            "3BV/s: " + FormatFloatTime(BVs),
                            "Clicks: " + std::to_string(this->board->GetCountClick())
                            };
                            this->PanelResult.SetLines(texts);

                            SaveManager::SaveGame(this->board.get(), this->mode.get(), static_cast<int>(this->mapID), static_cast<int>(this->modeID), this->timer.GetTime());
                            break;
                        }
                        default: break;
                    }

                    // thiết lập PanelStat
                    if (this->board->IsEnded())
                    {
                        this->bestRecord = SaveManager::GetRecord(static_cast<int>(this->mapID), static_cast<int>(this->modeID));
                        std::vector<std::string> texts = {
                            "3BV: " + std::to_string(this->board->GetCountClick()) + "/" + std::to_string(this->board->GetCountClickTB())
                        };
                        // nếu lần đầu chơi thì k có best score
                        if (this->bestRecord != std::nullopt)
                        {
                            texts.push_back("Best Score: " + std::to_string(this->bestRecord->score));
                            texts.push_back("Time: " + FormatFloatTime(this->bestRecord->time));
                        }
                        else {texts.push_back("Best Score: Null");}
                        this->PanelStat.SetLines(texts);
                    }
                }

                else if (mousePressed->button == sf::Mouse::Button::Right)
                {
                    if (this->mode->NeedHold(point))
                    { this->CellHolding = point;}
                    else
                    {
                        if (this->mode->CanInteract(point))
                        { this->board->ToggleMark(point); }
                    }
                }
            }
        }
        else if (const auto* mouseReleased = event.getIf<sf::Event::MouseButtonReleased>())
        { 
            if (mouseReleased->button == sf::Mouse::Button::Right)
            { this->CellHolding = std::nullopt; }
        }
    }
}

void GameScreen::Update(float deltaTime,sf::Vector2f mousePoint)
{
    if (this->overlayOn)
    { this->overlay.Update(mousePoint); }
    else
    {
        // sau 5s lưu file 1 lần
        if (!this->board->IsEnded())
        {
            this->timeSave -= deltaTime;
            if (this->timeSave<0)
            {
                this->timeSave = TIME_SAVE;
                SaveManager::SaveGame(this->board.get(), this->mode.get(), static_cast<int>(this->mapID), static_cast<int>(this->modeID), this->timer.GetTime());
            }
        }

        // Mode thực hiện sự kiện theo thời gian
        this->mode->OnTick(deltaTime);
        // Khi chuột đang đc giữ
        if (this->CellHolding != std::nullopt)
        {
            this->mode->OnLeftHold( *this->CellHolding, deltaTime);
            // khi mà mode đã xử lý xong CellHolding
            if (!this->mode->NeedHold(*this->CellHolding))
            { this->CellHolding = std::nullopt; }
        }

        // Cập nhật thời gian
        this->textTimer.setString(FormatBoardTime(this->timer.GetTime()));
        // Cập nhật số mìn còn lại 
        int flagCount = 0;
        for (int y = 0; y < this->board->GetRows(); y++) {
            for (int x = 0; x < this->board->GetCols(); x++) {
                if (this->board->GetCell(CellPoint(x,y))->GetState() == CellState::Flagged)
                { flagCount++; }
            }
        }
        int minesLeft = this->board->GetMineCount() - flagCount;
        this->textMineLeft.setString(FormatBoardMines(minesLeft));
        // cập nhật lại vị trí tâm
        sf::FloatRect bounds = this->textTimer.getLocalBounds();
        this->textTimer.setOrigin({
            bounds.position.x + bounds.size.x / 2.0f, 
            bounds.position.y + bounds.size.y / 2.0f
        });
        bounds = this->textMineLeft.getLocalBounds();
        this->textMineLeft.setOrigin({
            bounds.position.x + bounds.size.x / 2.0f, 
            bounds.position.y + bounds.size.y / 2.0f
        });
        
        Screen::Update(deltaTime,mousePoint);

        if (this->board->IsGenerated() && !this->board->IsEnded())
        { this->timer.Update(deltaTime); }
    }
}

void GameScreen::CreateBoard()
{
    if (this->mapID != MapID::BeeHive)
    {
        switch(this->modeID)
        {
            case ModeID::Easy:  
                this->board = std::make_unique<SquareBoard>(50,sf::Vector2f(640,420),9,9,10);
                break;
            case ModeID::Normal:
                this->board = std::make_unique<SquareBoard>(30,sf::Vector2f(640,440),16,16,40);
                break;
            case ModeID::Hard:
                this->board = std::make_unique<SquareBoard>(30,sf::Vector2f(640,440),30,16,99);
                break;
            case ModeID::Explore:
                //... chưa làm
                break;
        }
    }
    else {
        switch(this->modeID)
        {
            case ModeID::Easy:  
                this->board = std::make_unique<HexBoard>(35,sf::Vector2f(640,420),9,9,10);
                break;
            case ModeID::Normal:
                this->board = std::make_unique<HexBoard>(20,sf::Vector2f(640,440),16,16,40);
                break;
            case ModeID::Hard:
                this->board = std::make_unique<HexBoard>(20,sf::Vector2f(640,440),30,16,99);
                break;
            case ModeID::Explore:// không có trong Map Bee's Hive
                break;
        }
    }
}

void GameScreen::CreateMode()
{
    // vì board lưu trong unique_ptr nền board.get() lấy AbstractBoard*
    switch(this->mapID)
    {
        case MapID::Basic:
            this->mode = std::make_unique<BasicMode>(this->board.get(), &this->player);
            break;
        case MapID::Desert:
            this->mode = std::make_unique<DesertMode>(this->board.get(), &this->player,this->game->GetResources().GetImage(ImageID::G_Sand));
            break;
        case MapID::Jungle:
            this->mode = std::make_unique<JungleMode>(this->board.get(), &this->player,this->game->GetResources().GetImage(ImageID::G_Root));
            break;
        case MapID::BeeHive:
            this->mode = std::make_unique<BeeHiveMode>(this->board.get(), &this->player);
            break;
    }

    if (this->board && this->mode) 
    {
        // board thông qua Validator để biết Luật mở ô
        this->board->SetValidator([this](CellPoint point) {
            return this->mode->CheckCellOpen(point) != OpenResult::Blocked; 
        });

        // board thông qua onHitManeValidator để đết biết có cần bảo vệ mạng không
        this->board->SetHitMineValidator([this](CellPoint point) {
            return this->mode->OnHitMine(point);
        });
    }
}

void GameScreen::RenderGrid(sf::RenderWindow& window) const
{
    auto drawCell = [&](ImageID imageID, sf::Vector2f pointDraw) {
        sf::Sprite spr(this->game->GetResources().GetImage(imageID));
        sf::FloatRect bounds = spr.getLocalBounds();
        
        float scaleX, scaleY;
        if (this->mapID == MapID::BeeHive) {
            float hexWidth = std::sqrt(3.0f) * this->board->GetSizeCell();
            float hexHeight = 2.0f * this->board->GetSizeCell();
            scaleX = hexWidth / bounds.size.x;
            scaleY = hexHeight / bounds.size.y;
        } else {
            scaleX = this->board->GetSizeCell() / bounds.size.x;
            scaleY = this->board->GetSizeCell() / bounds.size.y;
        }
        
        spr.setScale({scaleX, scaleY});
        spr.setOrigin({bounds.size.x / 2.0f, bounds.size.y / 2.0f}); // Luôn dời về chính tâm của ảnh
        spr.setPosition(pointDraw);
        window.draw(spr);
    };

    for (int y = 0; y < this->board->GetRows(); y++)
    {
        for (int x = 0; x < this->board->GetCols(); x++)
        {
            Cell* cell = this->board->GetCell(CellPoint(x,y));
            sf::Vector2f pointDraw = this->board->CellToPixel(CellPoint(x,y));
            
            if (cell->GetState() == CellState::Openned)
            {
                drawCell(ImageID::G_Openned, pointDraw);
                if (cell->GetNumber() != 0) {
                    drawCell(static_cast<ImageID>(static_cast<int>(ImageID::G_Number0)+cell->GetNumber()), pointDraw);
                }
                continue; 
            }
            
            drawCell(ImageID::G_Hidded, pointDraw);
            
            if (cell->GetState() == CellState::Flagged) { drawCell(ImageID::G_Flag, pointDraw); }
            else if (cell->GetState() == CellState::Questioned) { drawCell(ImageID::G_Question, pointDraw); }
        }
    }

    if (this->board->IsEnded() && this->board->IsLosed())
    {
        for (int y = 0; y < this->board->GetRows(); y++)
        {
            for (int x = 0; x < this->board->GetCols(); x++)
            {
                Cell* cell = this->board->GetCell(CellPoint(x,y));
                sf::Vector2f pointDraw = this->board->CellToPixel(CellPoint(x,y));
    
                if (cell->CheckIsMine() && cell->GetState() != CellState::Flagged) {
                    drawCell(ImageID::G_Openned, pointDraw);
                    drawCell(ImageID::G_Bomb, pointDraw);
                }
                if (cell->GetState() == CellState::Flagged && !cell->CheckIsMine()) {
                    drawCell(ImageID::G_Wrong, pointDraw);
                    drawCell(ImageID::G_Flag, pointDraw);
                }
            }
        }
        for (const CellPoint& point : this->board->GetCellPointDead()) {
            sf::Vector2f pointDraw = this->board->CellToPixel(point);
            drawCell(ImageID::G_Hidded, pointDraw);
            drawCell(ImageID::G_Dead, pointDraw);
        }
    }
}

std::string GameScreen::FormatFloatTime(float time) const
{
    std::stringstream ss;
    ss << std::fixed << std::setprecision(4) << time;
    return ss.str();
}

std::string GameScreen::FormatBoardTime(float time) const
{
    int minutes = static_cast<int>(time) / 60; // Lấy số phút
    int seconds = static_cast<int>(time) % 60; // Lấy số giây lẻ
    
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << minutes << ":" 
       << std::setfill('0') << std::setw(2) << seconds;
    return ss.str();
}

std::string GameScreen::FormatBoardMines(int count) const
{
    std::stringstream ss;
    if (count < 0)
    { ss << "-" << std::setfill('0') << std::setw(2) << -count; }
    else
    { ss << std::setfill('0') << std::setw(2) << count; }
    return ss.str();
}