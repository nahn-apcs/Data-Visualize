#include <AvlScreen.hpp>
#include <MenuScreen.hpp>
#include <SFML/Graphics.hpp>

void AvlScreen::run() {
    ds.push_back(nhan(TextureID::app_bg, 0, 0));

    while(mWindow.isOpen()) {
        ProcessEvent();
        Update();
        Render();
    }
}

void AvlScreen::ProcessEvent() {
    sf::Event event;
    while(mWindow.pollEvent(event)) {
        if (event.type == sf::Event::Closed) mWindow.close();
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