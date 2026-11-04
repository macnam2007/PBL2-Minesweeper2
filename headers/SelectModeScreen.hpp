#pragma once

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Text.hpp"
#include "Screen.hpp"

class SelectModeScreen : public Screen
{
    private:
        Button btnBack;
        Button btnEasy;
        Button btnNormal;
        Button btnHard;
        Button btnExplore;
        Button btnStart;

        sf::Sprite spriteBg;
        sf::Sprite spriteBoxInfo;

        MapID map;
        ModeID mode;
    
    public:
        explicit SelectModeScreen(Game* game, MapID map, ModeID mode  = ModeID::Easy);
        void Render(sf::RenderWindow& window) override;
};