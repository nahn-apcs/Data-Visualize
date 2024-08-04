#include <TrieScreen.hpp>
#include <MenuScreen.hpp>
#include <SFML/Graphics.hpp>

void TrieScreen::run() {
    ds.push_back(nhan(TextureID::app_bg, 0, 0));

    while(mWindow.isOpen()) {
        ProcessEvent();
        Update();
        Render();
    }
}

void TrieScreen::ProcessEvent() {
    sf::Event event;
    while(mWindow.pollEvent(event)) {
        if (event.type == sf::Event::Closed) mWindow.close();
    }
}

void TrieScreen::Update() {

}

void TrieScreen::Render() {
    mWindow.clear();
    for(int i = 0; i < ds.size(); i++) {
        sf::Sprite sprite;
        sprite.setPosition(ds[i].pos_x, ds[i].pos_y);
        sprite.setTexture(MyTexture.get(ds[i].a));

        mWindow.draw(sprite);
    }
    mWindow.display();
}