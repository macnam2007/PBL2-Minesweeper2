#include <iostream>
#include "SFML/Graphics.hpp"
#include "SFML/Graphics/Texture.hpp"

class Panel
{
    private:
        sf::Sprite bg;
        const sf::Font* font;
        sf::Text title;
        std::vector<sf::Text> lines;
    
    public:
        Panel(const sf::Texture& bg,const sf::Font& font, sf::Vector2f point);
        void SetTitle(const std::string& title);
        void SetLines(const std::vector<std::string>& lines);
        void Render(sf::RenderTarget& target) const;
};