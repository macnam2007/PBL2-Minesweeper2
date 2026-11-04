#pragma once

#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/System/Vector2.hpp"
#include <SFML/Graphics.hpp>
#include <functional>

class Button
{
    private:
        enum class State {normal, hover, pressed};

        const sf::Texture* textures[3];
        sf::Sprite sprite;
        State state = State::normal;

        std::function<void()> onClick; // onClick chứa 1 hàm bất kỳ

        // thay đổi trạng thái và hình ảnh nút theo trạng thái 
        void ChangeState(State state);
    
    public:
        // 3 ảnh và vị trí tâm của nút ChangeState hợp lý: hàm này làm cả hai việc bạn nói là đổi biến state và đổi ảnh của sprite. Nó cũng giống cách
        Button(const sf::Texture& normal, const sf::Texture& hover, const sf::Texture& pressed, const sf::Vector2f position);
        
        // thiết lập tương tác khi click nút
        void SetOnClick(std::function<void()> f);

        // kiểm tra chuột có đang nằm trong nút không 
        bool Contains(sf::Vector2f point) const;

        // kiểm tra sự tương tác với nút [CLICK CHUỘT] / [NHẢ CHUỘT] 
        void HandleEvent(const sf::Event& event, sf::Vector2f mousePoint);

        // cập nhật vị trí nút
        void UpdatePosition(const sf::Vector2f& point);

        // cập nhật hình ảnh của nút
        void UpdateImage(const sf::Texture& normal, const sf::Texture& hover, const sf::Texture& pressed);

        // cập nhật trạng thái của nút theo  
        void Update(sf::Vector2f mousePoint);

        // vẽ nút 
        void Render(sf::RenderTarget& target) const;  
        
        // reset trạng thái của Nút 
        void Reset();
};