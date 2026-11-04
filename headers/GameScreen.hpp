#pragma once

#include "OverlayScreen.hpp"
#include "SFML/System/Vector2.hpp"
#include "Screen.hpp"

class GameScreen : public Screen
{
    private:
        Button btnPause;
    
        MapID map;
        ModeID mode;

        sf::Sprite spriteBg;

        OverlayScreen overlay;
        bool overlayOn=false;
    
    public:
        explicit GameScreen(Game* game, MapID map, ModeID mode);
        void Render(sf::RenderWindow& window) override;
        
        void HandleEvent(const sf::Event& event, sf::Vector2f mousePoint) override;
        void Update(float deltaTime, sf::Vector2f mousePoint) override;
};
