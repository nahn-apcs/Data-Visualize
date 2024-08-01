#include <SFML/Graphics.hpp>
#include <iostream>
#include <begin_button.hpp>

void Begin_Button::draw(sf::RenderWindow& window) {
    window.draw(sprite);
    return;
}

void Begin_Button::move(float offset_x, float offset_y) {
    sprite.move(offset_x, offset_y);
}

void Begin_Button::set_pos(int a, int b) {
    sprite.setPosition(a, b);
}