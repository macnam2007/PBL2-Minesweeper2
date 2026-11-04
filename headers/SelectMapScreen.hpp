#pragma once

#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "Screen.hpp"

class SelectMapScreen : public Screen
{
    private:
        Button btnBack;

        Button btnBasic;
        Button btnDesert;
        Button btnJungle;
        Button btnBeeHive;

        sf::Sprite spriteBg;
    
    public:
        explicit SelectMapScreen(Game* game);
        void Render(sf::RenderWindow& window) override;
};