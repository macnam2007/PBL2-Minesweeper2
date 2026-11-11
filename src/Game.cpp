#include "../headers/Game.hpp"
#include "../headers/HomeScreen.hpp"
#include "../headers/SelectMapScreen.hpp"
#include "../headers/SelectModeScreen.hpp"
#include <algorithm>
#include <iostream>
#include <optional>

Game::Game()
    : window(sf::VideoMode({1280u, 720u}), "Minesweeper",
             sf::Style::Titlebar | sf::Style::Close)
{
    this->window.setVerticalSyncEnabled(true);

    if (!this->resource.LoadFont() || !this->resource.LoadImagesGlobal() ||
        !this->resource.LoadSounds() || !this->resource.LoadMusic())
    {
        std::cerr << "Không nạp được tài nguyên!!!\n";
        this->running = false;
        return;
    }

    this->resource.PlayMusic();
    this->current = std::make_unique<HomeScreen>(this);
}

Game::~Game() {};

void Game::Run()
{
    while (this->running && this->window.isOpen())
    {
        // Chặn trần 0.1s: kéo cửa sổ hoặc máy đứng hình thì cát/rễ không dồn một lúc [dùng cho GameScreen]
        this->deltaTime = std::min(this->clock.restart().asSeconds(), 0.1f);

        sf::Vector2f mouse = this->window.mapPixelToCoords(sf::Mouse::getPosition(this->window));

        this->current->Update(this->deltaTime, mouse); // cập nhật trạng thái các nút/chữ/.. trên màn hình

        while (const std::optional event = this->window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                Quit();
            else
                this->current->HandleEvent(*event, mouse);
        }
        if (!this->running) break; // nếu Quit() chạy thì không cần vẽ và thoát luôn

        this->window.clear(); // làm sạch màn hình cũ 
        this->current->Render(this->window); // vẽ màn hình mới 
        this->window.display(); // cập nhật màn hình để hiện lên những gì đã vẽ

        ChangePendingScreen();  // luôn ở cuối frame
    }
}

void Game::ChangeScreen(std::unique_ptr<Screen> next)
{
    // Lưu tạm màn hình tiếp theo 
    this->pending = std::move(next);
}

void Game::ChangePendingScreen()
{
    // Khi đã xử lý xong màn cũ, hàm này được chạy để thay thế màn hiện tại
    if (this->pending)
        this->current = std::move(this->pending);
}

void Game::Quit()
{
    this->running = false;
    this->window.close();
}

bool Game::ToggleMusic() { return resource.ToggleMusic(); }
bool Game::ToggleVoice() { return resource.ToggleSound(); }