#include <BtreeScreen.hpp>
#include <MenuScreen.hpp>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <random>
#include <tinyfiledialogs.h>
#include <fstream>

std::mt19937 rng_3(std::chrono::steady_clock::now().time_since_epoch().count());

int rand_3(int l, int r) {
    return l + rng_3() % (r - l + 1);
}

void BtreeScreen::reset_big_node(big_node *tmp) {
    if(tmp == NULL) return;
    for(int i = 0; i < tmp->ds_big_node.size(); i++) {
        reset_big_node(tmp->ds_big_node[i]);
    }
    tmp->ds_big_node.clear();

    while(tmp->ds_small_node.size()) {
        delete tmp->ds_small_node.back();
        tmp->ds_small_node.pop_back();
    }

    while(tmp->ds_edge.size()) {
        delete tmp->ds_edge.back();
        tmp->ds_edge.pop_back();
    }
    tmp = NULL;
    return;
}
void BtreeScreen::reset_root() {
    reset_big_node(root);
    root = NULL;
}

BtreeScreen::big_node *BtreeScreen::copy_root(big_node *tmp) {
    big_node *new_big_node = new big_node;
    *new_big_node = *tmp;

    new_big_node->ds_big_node.clear();
    new_big_node->ds_edge.clear();
    new_big_node->ds_small_node.clear();

    for(int i = 0; i < tmp->ds_small_node.size(); i++) {
        small_node *new_small_node = new small_node;
        *new_small_node = *tmp->ds_small_node[i];
        new_big_node->ds_small_node.push_back(new_small_node);
    }

    for(int i = 0; i < tmp->ds_edge.size(); i++) {
        edge *new_edge = new edge;
        *new_edge = *tmp->ds_edge[i];
        new_big_node->ds_edge.push_back(new_edge);
    }

    for(int i = 0; i < tmp->ds_big_node.size(); i++) {
        new_big_node->ds_big_node.push_back(copy_root(tmp->ds_big_node[i]));
    }
    return new_big_node;
}

void BtreeScreen::dfs(big_node *tmp, small_node *block) {
    tmp->num_small_left = 0;
    tmp->num_big_node_left = 0;
    if(tmp->ds_big_node.size() == 0) {
        tmp->num_small_left = cnt_small_node;
        tmp->num_big_node_left = time_dfs;
        time_dfs++;

        for(int i = 0; i < tmp->ds_small_node.size(); i++) {
            if(tmp->ds_small_node[i] != block) cnt_small_node++;
        }
    }
    for(int i = 0; i < tmp->ds_big_node.size(); i++) {
        dfs(tmp->ds_big_node[i], block);
    }
}

void BtreeScreen::dfs_time(small_node *block) {
    time_dfs = 0;
    cnt_small_node = 0;

    dfs(root, block);
}



void BtreeScreen::run() {
    //Add_image
    ds.push_back(image(TextureID::app_bg, TextureID::app_bg, 0, 0, 0));//0
    ds.push_back(image(TextureID::back_menu_button, TextureID::back_menu_button, 10, 10, 0));//1
    ds.push_back(image(TextureID::create_button, TextureID::create_button_in, 0, 617, 0));//2
    ds.push_back(image(TextureID::insert_button, TextureID::insert_button_in, 0, 667, 0));//3
    ds.push_back(image(TextureID::search_button, TextureID::search_button_in, 0, 717, 0));//4
    ds.push_back(image(TextureID::delete_button, TextureID::delete_button_in, 0, 767, 0));//5
    ds.push_back(image(TextureID::tool_bar, TextureID::tool_bar, 0, 842, 0));//6
    ds.push_back(image(TextureID::time_bar, TextureID::time_bar, 468, 864, 0));//7
    ds.push_back(image(TextureID::speed_bar, TextureID::speed_bar, 1182, 864, 0));//8
    ds.push_back(image(TextureID::go_begin_disable, TextureID::go_begin_white, 190, 852, 0));//9
    ds.push_back(image(TextureID::step_backward_disable, TextureID::step_backward_white, 243, 852, 0));//10
    ds.push_back(image(TextureID::return_button, TextureID::return_button, 302, 854, 0));//11
    ds.push_back(image(TextureID::play_button, TextureID::play_button, 298, 852, 1));//12
    ds.push_back(image(TextureID::pause_button, TextureID::pause_button, 298, 852, 1));//13
    ds.push_back(image(TextureID::step_forward_disable, TextureID::step_forward_white, 353, 852, 0));//14
    ds.push_back(image(TextureID::go_end_disable, TextureID::go_end_white, 397, 852, 0));//15
    ds.push_back(image(TextureID::cuc, TextureID::cuc, 1182, 864, 0)); //cuc speed //16


    //create
    ds.push_back(image(TextureID::n_input, TextureID::n_input, 160, 617, 0));//17
    ds.push_back(image(TextureID::random_button, TextureID::random_button, 247, 624, 0));//18
    ds.push_back(image(TextureID::go_button, TextureID::go_button_in, 300, 617, 0));//19
    ds.push_back(image(TextureID::file_button, TextureID::file_button_in, 360, 617, 0));//20

    //insert
    ds.push_back(image(TextureID::val_input, TextureID::val_input, 160, 667, 0));//21
    ds.push_back(image(TextureID::go_button, TextureID::go_button_in, 270, 667, 0));//22

    //search
    ds.push_back(image(TextureID::val_input, TextureID::val_input, 160, 717, 0));//23
    ds.push_back(image(TextureID::go_button, TextureID::go_button_in, 270, 717, 0));//24

    //delete
    ds.push_back(image(TextureID::val_input, TextureID::val_input, 160, 767, 0));//25
    ds.push_back(image(TextureID::go_button, TextureID::go_button_in, 270, 767, 0));//26

    ds[17].input = 1;
    ds[17].box_pos_x = 212;
    ds[17].box_pos_y = 632;

    ds[21].input = 1;
    ds[21].box_pos_x = 225;
    ds[21].box_pos_y = 682;


    ds[23].input = 1;
    ds[23].box_pos_x = 225;
    ds[23].box_pos_y = 732;


    ds[25].input = 1;
    ds[25].box_pos_x = 225;
    ds[25].box_pos_y = 782;


    


    while(mWindow.isOpen()) {
        ProcessEvent();
        Update();
        if(Return  == 1) {
            //clear_state();
            //clear_root(root);
            root = NULL;
            break;
        }
        Render();
    }
}

void BtreeScreen::ProcessEvent() {
    sf::Event event;
    while(mWindow.pollEvent(event)) {
        if (event.type == sf::Event::Closed) mWindow.close();
        if(event.type == sf::Event::MouseButtonPressed) 
        {
            if(event.mouseButton.button == sf::Mouse::Left) 
            {
                for(int i = 1; i < ds.size(); i++) {

                    if(ds[i].disable) continue;

                    sf::Vector2i mousePos = sf::Mouse::getPosition(mWindow);
                    sf::Sprite sprite;

                    sprite.setPosition(ds[i].pos_x, ds[i].pos_y);
                    sprite.setTexture(MyTexture.get(ds[i].a));
                    if(sprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {

                        if(i >= 2 && i <= 5) {
                            for(int j = 2; j <= 5; j++) ds[j].clicked = 0;
                        }
                        else if(i >= 17 && i <= 26) {
                            for(int j = 17; j <= 26; j++) ds[j].clicked = 0;
                        }

                        ds[i].clicked = 1;
                        if(i == 16) ds[i].pressed = 1;
                    }
                }
            }
        }
        if(event.type == sf::Event::MouseButtonReleased) 
        {
            if(event.mouseButton.button == sf::Mouse::Left) 
            {
                if(ds[16].pressed == 1) {
                    ds[16].pressed = 0;
                    ds[16].released = 1;
                }
            }
        }
        if(event.type == sf::Event::TextEntered) {
            if((int)event.text.unicode == 8) {
                back_space = 1;
                so = 10;
            }
            else if((int)event.text.unicode >= 48 && (int)event.text.unicode <= 57) {
                so = static_cast<char>(event.text.unicode) - '0';
                back_space = 0;
            }
        }
        if(event.type == sf::Event::KeyPressed) {
            if(event.key.code == sf::Keyboard::Up) {
                ds[15].clicked = 1;
            }
            else if(event.key.code == sf::Keyboard::Down) {
                ds[9].clicked = 1;
            }
            else if(event.key.code == sf::Keyboard::Left) {
                ds[10].clicked = 1;
            }
            else if(event.key.code == sf::Keyboard::Right) {
                ds[14].clicked = 1;
            }
            else if(event.key.code == sf::Keyboard::Space) {
                if(ds[11].disable == 0) {
                    ds[11].clicked = 1;
                }
                else if(ds[12].disable == 0) {
                    ds[12].clicked = 1;
                }
                else if(ds[13].disable == 0) {
                    ds[13].clicked = 1;
                }
            }
            else if(event.key.code == sf::Keyboard::Enter) {
                if(ds[19].disable == 0) {
                    ds[19].clicked = 1;
                }
                else if(ds[22].disable == 0) {
                    ds[22].clicked = 1;
                }
                else if(ds[24].disable == 0) {
                    ds[24].clicked = 1;
                }
                else if(ds[26].disable == 0) {
                    ds[26].clicked = 1;
                }
            }
        }
    }
}


void BtreeScreen::Update() {

    if(ds[1].clicked == 1) Return = 1;

    for(int i = 2; i < ds.size(); i++) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(mWindow);
        sf::Sprite sprite;
        sprite.setPosition(ds[i].pos_x, ds[i].pos_y);
        sprite.setTexture(MyTexture.get(ds[i].a));

        if (sprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos)) || ds[i].clicked) {
            ds[i].change = 1;
        } else {
            ds[i].change = 0;
        }
    }

    //speed_button
    if(ds[16].pressed) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(mWindow);
        ds[16].pos_x = mousePos.x;
        ds[16].pos_x = std::max(ds[16].pos_x, 1182);
        ds[16].pos_x = std::min(ds[16].pos_x, 1317);
    }
    if(ds[16].released) {
        ds[16].released = 0;
        speed = 1.0 * (ds[16].pos_x - 1182) / 135 * 4 + 1;
    }

    //create button
    if(ds[2].clicked == 1) {
        ds[17].disable = ds[18].disable = ds[19].disable = ds[20].disable = 0;
    }
    else {
        ds[17].disable = ds[18].disable = ds[19].disable = ds[20].disable = 1;
        ds[17].text = "";
    }

    //insert_button
    if(ds[3].clicked == 1) {
        ds[21].disable = ds[22].disable = 0;
    }
    else {
        ds[21].disable = ds[22].disable = 1;
        ds[21].text = "";
    }

    //search_button
    if(ds[4].clicked == 1) {
        ds[23].disable = ds[24].disable = 0;
    }
    else {
        ds[23].disable = ds[24].disable = 1;
        ds[23].text = "";
    }

    //delete_button
    if(ds[5].clicked == 1) {
        ds[25].disable = ds[26].disable = 0;
    }
    else {
        ds[25].disable = ds[26].disable = 1;
        ds[25].text = "";
    }

    ////xử lí theo tác nhập từ bàn phím
    if(ds[17].disable == 0 && ds[17].clicked == 1) {
        std::string &s = ds[17].text;
        if(back_space == 1) {
            if(s.size()) s.pop_back();
        }
        else if(so >= 0 && so < 10) {
            s += char(so + '0');
        }
        modify_input(s, 1);
    }
    else if(ds[21].disable == 0 && ds[21].clicked == 1) {
        std::string &s = ds[21].text;
        if(back_space == 1) {
            if(s.size()) s.pop_back();
        }
        else if(so >= 0 && so < 10) {
            s += char(so + '0');
        }
        modify_input(s, 2);
    }
    else if(ds[23].disable == 0 && ds[23].clicked == 1) {
        std::string &s = ds[23].text;
        if(back_space == 1) {
            if(s.size()) s.pop_back();
        }
        else if(so >= 0 && so < 10) {
            s += char(so + '0');
        }
        modify_input(s, 2);
    }
    else if(ds[25].disable == 0 && ds[25].clicked == 1) {
        std::string &s = ds[25].text;
        if(back_space == 1) {
            if(s.size()) s.pop_back();
        }
        else if(so >= 0 && so < 10) {
            s += char(so + '0');
        }
        modify_input(s, 2);
    }

    so = 10;
    back_space = 0;

    //random n create
    if(ds[18].disable == 0 && ds[18].clicked == 1) {
        int tmp = rand_3(1, 30);
        ds[17].text = std::to_string(tmp);
        ds[18].clicked = 0;
    }


    //full_back
    if(ds[9].clicked) {
        //cur_state = 0;
        ds[9].clicked = 0;
        is_play = 0;
        ds[12].disable = 0;
        ds[13].disable = 1;
        ds[11].disable = 1;
    }


    //back_step

    if(ds[10].clicked) {
        //cur_state -= 30;
        //if(cur_state < 0) cur_state = 0;
        ds[10].clicked = 0;
        is_play = 0;
        ds[12].disable = 0;
        ds[13].disable = 1;
        ds[11].disable = 1;
    }

    //return
    if(ds[11].clicked) {
        //cur_state = 0;
        ds[11].clicked = 0;
        ds[11].disable = 1;
        ds[13].disable = 0;
        is_play = 1;
    }

    //play
    if(ds[12].clicked) {
        ds[12].clicked = 0;
        ds[12].disable = 1;
        ds[13].disable = 0;
        is_play = 1;
        ds[11].disable = 1;
    }


    //pause
    if(ds[13].clicked) {
        ds[13].clicked = 0;
        is_play = 0;
        ds[12].disable = 0;
        ds[13].disable = 1;
        ds[11].disable = 1;
    }


    //go_step
    if(ds[14].clicked) {
        ds[14].clicked = 0;
        //cur_state += 30;
        //if(cur_state >= avl_state.size()) cur_state = avl_state.size() - 1;
        is_play = 0;
        ds[12].disable = 0;
        ds[13].disable = 1;
        ds[11].disable = 1;
    }

    //full_go
    if(ds[15].clicked) {
        ds[15].clicked = 0;
        //cur_state = avl_state.size() - 1;
        is_play = 0;
        ds[12].disable = 0;
        ds[13].disable = 1;
        ds[11].disable = 1;
    }



}


void BtreeScreen::modify_input(std::string &s, int t) {
    if(t == 1) {
        //n
        int tmp = 0;
        for(int i = 0; i < s.size(); i++) {
            tmp = tmp * 10 + s[i] - '0';
        }
        if(tmp > 30) tmp = 30;
        s = std::to_string(tmp);
    }
    else {
        //val
        int tmp = 0;
        for(int i = 0; i < s.size(); i++) {
            tmp = tmp * 10 + s[i] - '0';
        }
        if(tmp > 99) tmp = 99;
        s = std::to_string(tmp);
    }
}


void BtreeScreen::render_image() {
    for(int i = 0; i < ds.size(); i++) {
        if(ds[i].disable) continue;
        sf::Sprite sprite;
        sprite.setPosition(ds[i].pos_x, ds[i].pos_y);
        if(ds[i].change == 1) {
            sprite.setTexture(MyTexture.get(ds[i].b));
        }
        else {
            sprite.setTexture(MyTexture.get(ds[i].a));
        }
        mWindow.draw(sprite);

        if(ds[i].input) {
            sf::Text text;
            text.setFont(MyFont.get(FontID::ptsan));
            text.setString(ds[i].text);
            text.setCharacterSize(15);
            text.setFillColor(sf::Color::Black);

            text.setPosition(ds[i].box_pos_x, ds[i].box_pos_y);

            mWindow.draw(text);
        }
    }
}

void BtreeScreen::Render() {
    mWindow.clear();
    render_image();

    /*
    cur_state = std::min(cur_state, (int)avl_state.size() - 1);
    if(cur_state >= 0 && cur_state < avl_state.size()) {
        draw_avl(avl_state[cur_state]);
        draw_time_bar();
    }
    */



    mWindow.display();
    /*
    cur_state = cur_state + is_play * (int)speed;
    cur_state = std::min(cur_state, (int)avl_state.size() - 1);
    if(cur_state == avl_state.size() - 1) {
        is_play = 0;
        ds[12].disable = ds[13].disable = 1;
        ds[11].disable = 0;
    }
    */
}







// Hàm để tính chiều dài của đường thẳng
float BtreeScreen::length(const sf::Vector2f& start, const sf::Vector2f& end) {
    // Tính hiệu giữa tọa độ x và y của hai điểm
    float dx = end.x - start.x;
    float dy = end.y - start.y;
    
    // Tính chiều dài theo định lý Pythagoras
    return std::sqrt(dx * dx + dy * dy);
}

// Hàm để tính góc của đường thẳng (tính bằng radian)
float BtreeScreen::angle(const sf::Vector2f& start, const sf::Vector2f& end) {
    // Tính hiệu giữa tọa độ x và y của hai điểm
    float dy = end.y - start.y;
    float dx = end.x - start.x;
    
    // Tính góc theo hàm atan2
    return std::atan2(dy, dx) * 180 / 3.14159265f;
}
/*


void BtreeScreen::draw_time_bar() {


    //rgb(116,255,188)
    float per = 1.0 * (cur_state + 1) / (int)avl_state.size();

    sf::RectangleShape Line;

    Line.setSize(sf::Vector2f(500.0f * per, 15));
    Line.setFillColor(sf::Color(116, 225, 188));

    // Đặt vị trí và góc cho hình chữ nhật
    Line.setPosition(468, 864);

    mWindow.draw(Line);

}
*/







