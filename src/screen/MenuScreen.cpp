#include <MenuScreen.hpp>
#include <HeapScreen.hpp>
#include <AvlScreen.hpp>
#include <GraphScreen.hpp>
#include <TrieScreen.hpp>
#include <BtreeScreen.hpp>
#include <HashScreen.hpp>


void MenuScreen::run() 
{

    mWindow.setFramerateLimit(60);

    AddTexture();

    ds.push_back(nhan(TextureID::menu_bg, TextureID::menu_bg, 0, 0));
    ds.push_back(nhan(TextureID::hash, TextureID::border_hash, 122, 216));
    ds.push_back(nhan(TextureID::avl, TextureID::border_avl, 570, 216));
    ds.push_back(nhan(TextureID::btree, TextureID::border_btree, 1018, 216));
    ds.push_back(nhan(TextureID::heap, TextureID::border_heap, 122, 560));
    ds.push_back(nhan(TextureID::trie, TextureID::border_trie, 570, 560));
    ds.push_back(nhan(TextureID::graph, TextureID::border_graph, 1018, 560));

    //HeapScreen heapscreen(mWindow);

    while(mWindow.isOpen()) {
        ProcessEvent();
        Update();
        Render();
    }
}

void MenuScreen::ProcessEvent() 
{
    sf::Event event;
    while(mWindow.pollEvent(event)) {
        if (event.type == sf::Event::Closed) mWindow.close();
        else if(event.type == sf::Event::MouseButtonPressed) 
        {
            if(event.mouseButton.button == sf::Mouse::Left) {
                for(int i = 1; i < ds.size(); i++) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(mWindow);
                    sf::Sprite sprite;
                    sprite.setPosition(ds[i].pos_x, ds[i].pos_y);
                    sprite.setTexture(MyTexture.get(ds[i].a));

                    if (i == 1 && sprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                        HashScreen hashscreen(mWindow, MyTexture);
                        hashscreen.run();
                    }
                    else if (i == 2 && sprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                        AvlScreen avlscreen(mWindow, MyTexture);
                        avlscreen.run();
                    }
                    else if (i == 3 && sprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                        BtreeScreen btreescreen(mWindow, MyTexture);
                        btreescreen.run();
                    }
                    else if (i == 4 && sprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                        HeapScreen heapscreen(mWindow, MyTexture);
                        heapscreen.run();
                    }
                    else if (i == 5 && sprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                        TrieScreen triescreen(mWindow, MyTexture);
                        triescreen.run();
                    }
                    else if (i == 6 && sprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                        GraphScreen graphscreen(mWindow, MyTexture);
                        graphscreen.run();
                    }
                }
            }
        }
    }
}

void MenuScreen::Update() 
{
    for(int i = 0; i < ds.size(); i++) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(mWindow);
        sf::Sprite sprite;
        sprite.setPosition(ds[i].pos_x, ds[i].pos_y);
        sprite.setTexture(MyTexture.get(ds[i].a));

        if (sprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
            ds[i].change = 1;
        } else {
            ds[i].change = 0;
        }
    }
}

void MenuScreen::Render() 
{
    mWindow.clear();

    for(int i = 0; i < ds.size(); i++) {
        sf::Sprite sprite;
        sprite.setPosition(ds[i].pos_x, ds[i].pos_y);
        if(ds[i].change == 1) {
            sprite.setTexture(MyTexture.get(ds[i].b));
        }
        else {
            sprite.setTexture(MyTexture.get(ds[i].a));
        }
        mWindow.draw(sprite);
    }

    mWindow.display();
}

void MenuScreen::AddTexture() {
    MyTexture.load(TextureID::menu_bg, "res/background/background1.png");
    MyTexture.load(TextureID::hash, "res/menu_button/hash_button.png");
    MyTexture.load(TextureID::avl, "res/menu_button/avl_button.png");
    MyTexture.load(TextureID::btree, "res/menu_button/btree_button.png");
    MyTexture.load(TextureID::heap, "res/menu_button/heap_button.png");
    MyTexture.load(TextureID::trie, "res/menu_button/trie_button.png");
    MyTexture.load(TextureID::graph, "res/menu_button/graph_button.png");
    MyTexture.load(TextureID::border_hash, "res/menu_button/border_hash_button.png");
    MyTexture.load(TextureID::border_avl, "res/menu_button/border_avl_button.png");
    MyTexture.load(TextureID::border_btree, "res/menu_button/border_btree_button.png");
    MyTexture.load(TextureID::border_heap, "res/menu_button/border_heap_button.png");
    MyTexture.load(TextureID::border_trie, "res/menu_button/border_trie_button.png");
    MyTexture.load(TextureID::border_graph, "res/menu_button/border_graph_button.png");
    MyTexture.load(TextureID::app_bg, "res/background/background2.png");
    MyTexture.load(TextureID::back_menu_button, "res/menu_button/arrow.png");
}