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
    AddFont();

    ds.push_back(nhan(TextureID::menu_bg, TextureID::menu_bg, 0, 0));
    ds.push_back(nhan(TextureID::hash, TextureID::border_hash, 122, 216));
    ds.push_back(nhan(TextureID::avl, TextureID::border_avl, 570, 216));
    ds.push_back(nhan(TextureID::btree, TextureID::border_btree, 1018, 216));
    ds.push_back(nhan(TextureID::heap, TextureID::border_heap, 122, 560));
    ds.push_back(nhan(TextureID::trie, TextureID::border_trie, 570, 560));
    ds.push_back(nhan(TextureID::graph, TextureID::border_graph, 1018, 560));


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
        if(event.type == sf::Event::MouseButtonPressed) 
        {
            if(event.mouseButton.button == sf::Mouse::Left) {
                for(int i = 1; i < ds.size(); i++) {

                    sf::Vector2i mousePos = sf::Mouse::getPosition(mWindow);
                    sf::Sprite sprite;
                    sprite.setPosition(ds[i].pos_x, ds[i].pos_y);
                    sprite.setTexture(MyTexture.get(ds[i].a));

                    if (i == 1 && sprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                        HashScreen hashscreen(mWindow, MyTexture, MyFont);
                        hashscreen.run();
                    }
                    else if (i == 2 && sprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                        AVLScreen avlscreen(mWindow, MyTexture, MyFont);
                        avlscreen.run();
                    }
                    else if (i == 3 && sprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                        BtreeScreen btreescreen(mWindow, MyTexture);
                        btreescreen.run();
                    }
                    else if (i == 4 && sprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                        HeapScreen heapscreen(mWindow, MyTexture, MyFont);
                        heapscreen.run();

                    }
                    else if (i == 5 && sprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                        TrieScreen triescreen(mWindow, MyTexture, MyFont);
                        triescreen.run();
                    }
                    else if (i == 6 && sprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                        GraphScreen graphscreen(mWindow, MyTexture, MyFont);
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
    MyTexture.load(TextureID::cuc, "res/app_button/cuc.png");
    MyTexture.load(TextureID::go_begin_disable, "res/app_button/go_begin_disable.png");
    MyTexture.load(TextureID::go_begin_white, "res/app_button/go_begin_white.png");
    MyTexture.load(TextureID::go_end_disable, "res/app_button/go_end_disable.png");
    MyTexture.load(TextureID::go_end_white, "res/app_button/go_end_white.png");
    MyTexture.load(TextureID::pause_button, "res/app_button/pause_button.png");
    MyTexture.load(TextureID::play_button, "res/app_button/play_button.png");
    MyTexture.load(TextureID::return_button, "res/app_button/return_button.png");
    MyTexture.load(TextureID::speed_bar, "res/app_button/speed_bar.png");
    MyTexture.load(TextureID::step_backward_disable, "res/app_button/step_backward_disable.png");
    MyTexture.load(TextureID::step_backward_white, "res/app_button/step_backward_white.png");
    MyTexture.load(TextureID::step_forward_disable, "res/app_button/step_forward_disable.png");
    MyTexture.load(TextureID::step_forward_white, "res/app_button/step_forward_white.png");
    MyTexture.load(TextureID::time_bar, "res/app_button/time_bar.png");
    MyTexture.load(TextureID::tool_bar, "res/app_button/tool_bar.png");
    MyTexture.load(TextureID::create_button, "res/app_button/create_button.png");
    MyTexture.load(TextureID::create_button_in, "res/app_button/create_button_in.png");
    MyTexture.load(TextureID::delete_button, "res/app_button/delete_button.png");
    MyTexture.load(TextureID::delete_button_in, "res/app_button/delete_button_in.png");
    MyTexture.load(TextureID::extractmax_button, "res/app_button/extractmax_button.png");
    MyTexture.load(TextureID::extractmax_button_in, "res/app_button/extractmax_button_in.png");
    MyTexture.load(TextureID::insert_button, "res/app_button/insert_button.png");
    MyTexture.load(TextureID::insert_button_in, "res/app_button/insert_button_in.png");
    MyTexture.load(TextureID::update_button, "res/app_button/update_button.png");
    MyTexture.load(TextureID::update_button_in, "res/app_button/update_button_in.png");
    MyTexture.load(TextureID::n_input, "res/app_button/n_input.png");
    MyTexture.load(TextureID::random_button, "res/app_button/random_button.png");
    MyTexture.load(TextureID::file_button, "res/app_button/file_button.png");
    MyTexture.load(TextureID::file_button_in, "res/app_button/file_button_in.png");
    MyTexture.load(TextureID::go_button_in, "res/app_button/go_button_in.png");
    MyTexture.load(TextureID::go_button, "res/app_button/go_button.png");
    MyTexture.load(TextureID::val_input, "res/app_button/val_input.png");
    MyTexture.load(TextureID::i_input, "res/app_button/i_input.png");
    MyTexture.load(TextureID::search_button, "res/app_button/search_button.png");
    MyTexture.load(TextureID::search_button_in, "res/app_button/search_button_in.png");
    MyTexture.load(TextureID::m_input, "res/app_button/m_input.png");
    MyTexture.load(TextureID::s_input, "res/app_button/s_input.png");
    MyTexture.load(TextureID::fixed_off, "res/app_button/fixed_off.png");
    MyTexture.load(TextureID::fixed_off_in, "res/app_button/fixed_off_in.png");
    MyTexture.load(TextureID::fixed_on, "res/app_button/fixed_on.png");
    MyTexture.load(TextureID::fixed_on_in, "res/app_button/fixed_on_in.png");
    MyTexture.load(TextureID::matrix, "res/app_button/matrix.png");
    MyTexture.load(TextureID::matrix_in, "res/app_button/matrix_in.png");
    MyTexture.load(TextureID::cc, "res/app_button/cc.png");
    MyTexture.load(TextureID::cc_in, "res/app_button/cc_in.png");
    MyTexture.load(TextureID::mst, "res/app_button/mst.png");
    MyTexture.load(TextureID::mst_in, "res/app_button/mst_in.png");
}


void MenuScreen::AddFont() {
    MyFont.load(FontID::arial_bold, "res/fonts/arial_bold.ttf");
    MyFont.load(FontID::arial, "res/fonts/arial.ttf");
    MyFont.load(FontID::consolas, "res/fonts/Consolas-Bold.ttf");
    MyFont.load(FontID::ptsan, "res/fonts/PTSans-Bold.ttf");
}