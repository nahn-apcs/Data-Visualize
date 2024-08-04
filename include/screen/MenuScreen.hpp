#ifndef MENU_SCREEN_HPP
#define MENU_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <ResourceID.hpp>
#include <iostream>

class MenuScreen 
{
public:
    MenuScreen(): mWindow(sf::VideoMode(1440, 900), "23125014 - Nguyen Thanh Nhan",sf::Style::Close) {}
    void            run();



private:
    sf::RenderWindow mWindow;
    void                    ProcessEvent();
    void                    Update();
    void                    Render();
    void                    AddTexture();
    TextureHolder    MyTexture;

    struct nhan {
        int pos_x;
        int pos_y;
        TextureID a;
        TextureID b;
        bool change;

        nhan(TextureID _a, TextureID _b, int _pos_x = 0, int _pos_y = 0): a(_a), b(_b), change(0), pos_x(_pos_x), pos_y(_pos_y) {}
    };

    std::vector<nhan> ds;
};


#endif //MENU_SCREEN_HPP