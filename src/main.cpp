#include <ResourceID.hpp>
#include <iostream>
#include <SFML/Graphics.hpp>

int main() {
    TextureHolder tmp;
    tmp.load(TextureID::nen, "res/background/background1.png");

    sf::RenderWindow window(sf::VideoMode(1440, 900), "Nguyen Thanh Nhan - 23125014");

    sf::Sprite sprite;
    sprite.setTexture(tmp.get(TextureID::nen));

    while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }

        window.clear(sf::Color::Black); // Xóa màn hình
        window.draw(sprite);            // Vẽ sprite lên cửa sổ
        window.display();               // Hiển thị các nội dung đã vẽ
    }
}