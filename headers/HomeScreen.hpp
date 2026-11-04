#pragma once

#include "Screen.hpp"

class HomeScreen : public Screen
{
    private:
        Button btnPlay;
        Button btnExit;

        sf::Sprite spriteBg;
        sf::Sprite spriteIcon1;
        sf::Sprite spriteIcon2;
    public:
        explicit HomeScreen(Game* game);
        void Render(sf::RenderWindow& window) override;
};

