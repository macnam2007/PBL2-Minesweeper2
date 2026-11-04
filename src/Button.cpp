#include "../headers/Button.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/System/Vector2.hpp"

Button::Button(
    const sf::Texture& normal,
    const sf::Texture& hover,
    const sf::Texture& pressed,
    const sf::Vector2f position
) :  textures{&normal,&hover,&pressed} , sprite(normal)
{
    // đưa gốc về giữa hình ảnh
    this->sprite.setOrigin(sf::Vector2f(normal.getSize())/2.f);
    this->sprite.setPosition(position);
}

void Button::ChangeState(State state)
{
    if (this->state == state)
        return;

    this->state = state;
    this->sprite.setTexture(*this->textures[static_cast<int>(state)]);
}

void Button::SetOnClick(std::function<void()> f)
{ this->onClick = std::move(f); }

bool Button::Contains(sf::Vector2f point) const 
{ return this->sprite.getGlobalBounds().contains(point); }

void Button::HandleEvent(const sf::Event& event, sf::Vector2f mousePoint)
{
    // kiểm tra tương tác của click chuột 
    switch(this->state)
    {
        case State::normal:
            // Không cần sử dụng 
            break;

        case State::hover:
            if ( const auto* down = event.getIf<sf::Event::MouseButtonPressed>() )
            {
                if ( down->button == sf::Mouse::Button::Left && this->Contains(mousePoint))
                {
                    ChangeState(State::pressed);
                }
            }
            break;
        
        case State::pressed:
            if ( const auto* up = event.getIf<sf::Event::MouseButtonReleased>() )
            {
                if ( up->button == sf::Mouse::Button::Left)
                {
                    ChangeState(Contains(mousePoint) ? State::hover : State::normal);

                    if (this->Contains(mousePoint) && onClick)
                    { onClick(); }
                }
            }
            break;
    }
}

// cập nhật vị trí nút
void Button::UpdatePosition(const sf::Vector2f& point)
{ this->sprite.setPosition(point); }

// thay đổi hình ảnh của nut
void Button::UpdateImage(const sf::Texture& normal, const sf::Texture& hover, const sf::Texture& pressed)
{
    this->textures[static_cast<int>(State::normal)] = &normal;
    this->textures[static_cast<int>(State::hover)] = &hover;
    this->textures[static_cast<int>(State::pressed)] = &pressed;

    this->sprite.setTexture(*this->textures[static_cast<int>(this->state)]);
}

// cập nhật trạng thái nút theo frame 
void Button::Update(sf::Vector2f mousePoint)
{
    if (this->state == State::pressed)
    { return; }

    ChangeState(Contains(mousePoint) ? State::hover : State::normal);
}

void Button::Render(sf::RenderTarget& target) const
{ target.draw(this->sprite); }

void Button::Reset()
{
    ChangeState(State::normal);
}