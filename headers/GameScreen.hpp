#pragma once

#include "Game.hpp"
#include "OverlayScreen.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/System/Vector2.hpp"
#include "Screen.hpp"

#include "Board.hpp"
#include "Player.hpp"
#include "Timer.hpp"
#include "GameMode.hpp"
#include "Panel.hpp"
#include "../headers/SaveManager.hpp"

class GameScreen : public Screen
{
    private:
        Button btnPause;
        Button btnReset;
    
        MapID mapID;
        ModeID modeID;

        sf::Sprite spriteBg;

        OverlayScreen overlay;
        bool overlayOn=false;

        std::unique_ptr<AbstractBoard> board; // này là con trỏ lớp trừu tượng duy nhất
        Player player;
        Timer timer;
        std::unique_ptr<GameMode> mode; // này là còn trỏ duy nhất 

        Panel PanelStat;
        Panel PanelResult;

        std::optional<Record> bestRecord;

    
    public:

        explicit GameScreen(Game* game, MapID mapID, ModeID modeID);
        void Render(sf::RenderWindow& window) override;
        
        void HandleEvent(const sf::Event& event, sf::Vector2f mousePoint) override;
        void Update(float deltaTime, sf::Vector2f mousePoint) override;

        void CreateBoard();
        void CreateMode();

        void RenderGrid(sf::RenderWindow& window) const;
        std::string FormatFloatTime(float time) const;
};
