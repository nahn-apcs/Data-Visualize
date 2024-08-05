#include <AvlScreen.hpp>
#include <MenuScreen.hpp>
#include <SFML/Graphics.hpp>

void AvlScreen::run() {
    ds.push_back(nhan(TextureID::app_bg, 0, 0));
    ds.push_back(nhan(TextureID::back_menu_button, 10, 10));

    while(mWindow.isOpen()) {
        ProcessEvent();
        if(Return  == 1) break;
        Update();
        Render();
    }
}

void AvlScreen::ProcessEvent() {
    sf::Event event;
    while(mWindow.pollEvent(event)) {
        if (event.type == sf::Event::Closed) mWindow.close();
        else if(event.type == sf::Event::MouseButtonPressed) 
        {
            if(event.mouseButton.button == sf::Mouse::Left) 
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(mWindow);
                sf::Sprite sprite;
                sprite.setPosition(ds[1].pos_x, ds[1].pos_y);
                sprite.setTexture(MyTexture.get(ds[1].a));
                if(sprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                    Return = 1;
                }
            }
        }
    }
}

void AvlScreen::Update() {

}

void AvlScreen::Render() {
    mWindow.clear();
    for(int i = 0; i < ds.size(); i++) {
        sf::Sprite sprite;
        sprite.setPosition(ds[i].pos_x, ds[i].pos_y);
        sprite.setTexture(MyTexture.get(ds[i].a));

        mWindow.draw(sprite);
    }
    mWindow.display();
}