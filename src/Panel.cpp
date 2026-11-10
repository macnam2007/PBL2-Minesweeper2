#include "../headers/Panel.hpp"
#include "SFML/Graphics/RenderStates.hpp"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/Graphics/Texture.hpp"

Panel::Panel(const sf::Texture& bg,const sf::Font& font, sf::Vector2f point)
    : bg(bg), font(&font), title(font)
{
    this->bg.setPosition(point);
    
    // cài đặt tiêu đề
    title.setCharacterSize(22);
    title.setFillColor(sf::Color::Black);

    float bgWidth = this->bg.getGlobalBounds().size.x;
    float textWidth = title.getLocalBounds().size.x;
    float centerX = point.x + (bgWidth - textWidth) / 2.f;

    title.setPosition({centerX, point.y + 15.f});
}

void Panel::SetTitle(const std::string& newTitle)
{
    this->title.setString(newTitle); 

    float bgWidth = bg.getGlobalBounds().size.x;
    float bgX = bg.getPosition().x;

    float textWidth = this->title.getLocalBounds().size.x; 

    float centerX = bgX + (bgWidth - textWidth) / 2.f;

    float currentY = this->title.getPosition().y; 
    this->title.setPosition({centerX, currentY});
}

void Panel::SetLines(const std::vector<std::string>& lines)
{
    this->lines.clear();

    // thiết lập vị trí bắt đầu theo Title
    float X = bg.getPosition().x + 10.f;
    float Y = bg.getPosition().y + 70.f;
    float Space = 24.f;

    sf::Text text(*font);
    for (int i =0; i<lines.size(); i++)
    {
        text.setString(lines[i]);
        text.setCharacterSize(16);
        text.setFillColor(sf::Color(0, 0, 0));
        text.setPosition({X, Y+(i*Space)});

        this->lines.push_back(text);
    }
}

void Panel::Render(sf::RenderTarget& target) const
{
    target.draw(this->bg);
    target.draw(title);
    
    for (const sf::Text& line : lines)
    { target.draw(line); }
}