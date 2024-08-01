#ifndef BEGIN_BUTTON.HPP
#define BEGIN_BUTTON.HPP
#include <SFML/Graphics.hpp>
#include <iostream>

class Begin_Button {
private:

    sf::Texture texture;
    sf::Sprite sprite;

public:
    Begin_Button(const std::string &textureFile, float pos_x = 0, float pos_y = 0) {
        if (!texture.loadFromFile(textureFile)) {
            std::cerr << "Error loading texture" << std::endl;
        } else {
            sprite.setTexture(texture);
            sprite.setPosition(pos_x, pos_y);
        }
    }

    void draw(sf::RenderWindow&);
    void move(float, float);
    void set_pos(int, int);
};




#endif