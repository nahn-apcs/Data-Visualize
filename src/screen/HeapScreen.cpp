#include <HeapScreen.hpp>
#include <MenuScreen.hpp>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <random>
#include <tinyfiledialogs.h>
#include <fstream>

std::mt19937 rng_4(std::chrono::steady_clock::now().time_since_epoch().count());

int rand_4(int l, int r) {
    return l + rng_4() % (r - l + 1);
}



void HeapScreen::run() {
    //Add_image
    ds.push_back(image(TextureID::app_bg, TextureID::app_bg, 0, 0, 0));//0
    ds.push_back(image(TextureID::back_menu_button, TextureID::back_menu_button, 10, 10, 0));//1
    ds.push_back(image(TextureID::create_button, TextureID::create_button_in, 0, 567, 0));//2
    ds.push_back(image(TextureID::insert_button, TextureID::insert_button_in, 0, 617, 0));//3
    ds.push_back(image(TextureID::extractmax_button, TextureID::extractmax_button_in, 0, 667, 0));//4
    ds.push_back(image(TextureID::update_button, TextureID::update_button_in, 0, 717, 0));//5
    ds.push_back(image(TextureID::delete_button, TextureID::delete_button_in, 0, 767, 0));//6
    ds.push_back(image(TextureID::tool_bar, TextureID::tool_bar, 0, 842, 0));//7
    ds.push_back(image(TextureID::time_bar, TextureID::time_bar, 468, 864, 0));//8
    ds.push_back(image(TextureID::speed_bar, TextureID::speed_bar, 1182, 864, 0));//9
    ds.push_back(image(TextureID::go_begin_disable, TextureID::go_begin_white, 190, 852, 0));//10
    ds.push_back(image(TextureID::step_backward_disable, TextureID::step_backward_white, 243, 852, 0));//11
    ds.push_back(image(TextureID::return_button, TextureID::return_button, 302, 854, 0));//12
    ds.push_back(image(TextureID::play_button, TextureID::play_button, 298, 852, 1));//13
    ds.push_back(image(TextureID::pause_button, TextureID::pause_button, 298, 852, 1));//14
    ds.push_back(image(TextureID::step_forward_disable, TextureID::step_forward_white, 353, 852, 0));//15
    ds.push_back(image(TextureID::go_end_disable, TextureID::go_end_white, 397, 852, 0));//16
    ds.push_back(image(TextureID::cuc, TextureID::cuc, 1182, 864, 0)); //cuc speed //17

    //create
    ds.push_back(image(TextureID::n_input, TextureID::n_input, 160, 567, 0));//18
    ds.push_back(image(TextureID::random_button, TextureID::random_button, 243, 574, 0));//19
    ds.push_back(image(TextureID::file_button, TextureID::file_button_in, 360, 567, 0));//20
    ds.push_back(image(TextureID::go_button, TextureID::go_button_in, 300, 567, 0));//21

    //insert
    ds.push_back(image(TextureID::go_button, TextureID::go_button_in, 270, 617, 0));//22
    ds.push_back(image(TextureID::val_input, TextureID::val_input, 160, 617, 0));//23

    //update
    ds.push_back(image(TextureID::i_input, TextureID::i_input, 160, 717, 0));//24
    ds.push_back(image(TextureID::val_input, TextureID::val_input, 270, 717, 0));//25
    ds.push_back(image(TextureID::go_button, TextureID::go_button_in, 380, 717, 0));//26


    //delete
    ds.push_back(image(TextureID::i_input, TextureID::i_input, 160, 767, 0));//27
    ds.push_back(image(TextureID::go_button, TextureID::go_button_in, 270, 767, 0));//28


    ds[27].input = 1;
    ds[27].box_pos_x = 225;
    ds[27].box_pos_y = 782;


    ds[24].input = 1;
    ds[24].box_pos_x = 225;
    ds[24].box_pos_y = 732;


    ds[25].input = 1;
    ds[25].box_pos_x = 334;
    ds[25].box_pos_y = 732;


    ds[23].input = 1;
    ds[23].box_pos_x = 225;
    ds[23].box_pos_y = 632;


    ds[18].input = 1;
    ds[18].box_pos_x = 212;
    ds[18].box_pos_y = 582;
    



    reset_heap();
    cur_state = 0;

    while(mWindow.isOpen()) {
        ProcessEvent();
        Update();
        if(Return  == 1) break;
        Render();
    }
}

void HeapScreen::ProcessEvent() {
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

                        if(i >= 2 && i <= 6) {
                            for(int j = 2; j <= 6; j++) {
                                ds[j].clicked = 0;
                            }
                        }
                        else {
                            for(int j = 18; j <= 28; j++) {
                                ds[j].clicked = 0;
                            }
                        }
                        ds[i].clicked = 1;
                        if(i == 17) ds[i].pressed = 1;
                    }
                }
            }
        }
        if(event.type == sf::Event::MouseButtonReleased) 
        {
            if(event.mouseButton.button == sf::Mouse::Left) 
            {
                if(ds[17].pressed == 1) {
                    ds[17].pressed = 0;
                    ds[17].released = 1;
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
                ds[16].clicked = 1;
            }
            else if(event.key.code == sf::Keyboard::Down) {
                ds[10].clicked = 1;
            }
            else if(event.key.code == sf::Keyboard::Left) {
                ds[11].clicked = 1;
            }
            else if(event.key.code == sf::Keyboard::Right) {
                ds[15].clicked = 1;
            }
            else if(event.key.code == sf::Keyboard::Space) {
                if(ds[12].disable == 0) {
                    ds[12].clicked = 1;
                }
                else if(ds[13].disable == 0) {
                    ds[13].clicked = 1;
                }
                else if(ds[14].disable == 0) {
                    ds[14].clicked = 1;
                }
            }
            else if(event.key.code == sf::Keyboard::Enter) {
                if(ds[21].disable == 0) {
                    ds[21].clicked = 1;
                }
                else if(ds[22].disable == 0) {
                    ds[22].clicked = 1;
                }
                else if(ds[26].disable == 0) {
                    ds[26].clicked = 1;
                }
                else if(ds[28].disable == 0) {
                    ds[28].clicked = 1;
                }
            }
        }
    }
}


void HeapScreen::Update() {

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

    //create
    if(ds[2].clicked == 1) {
        ds[18].clicked = 1;
        ds[18].disable = ds[19].disable = ds[20].disable = ds[21].disable = 0;
    }
    else {
        ds[18].disable = ds[19].disable = ds[20].disable = ds[21].disable = 1;
        ds[18].text = "";
    }

    //insert
    if(ds[3].clicked == 1) {
        ds[22].disable = ds[23].disable = 0;
    }
    else {
        ds[22].disable = ds[23].disable = 1;
        ds[23].text = "";
    }



    //update
    if(ds[5].clicked == 1) {
        ds[24].disable = ds[25].disable = ds[26].disable = 0;
    }
    else {
        ds[24].disable = ds[25].disable = ds[26].disable = 1;
        ds[24].text = "";
        ds[25].text = "";
    }

    //delete
    if(ds[6].clicked == 1) {
        ds[27].disable = ds[28].disable = 0;
    }
    else {
        ds[27].disable = ds[28].disable = 1;
        ds[27].text = "";
    }


    //xử lí theo tác nhập từ bàn phím
    if(ds[2].clicked) {
        std::string& s = ds[18].text;
        if(back_space == 1) {
            if(s.size()) s.pop_back();
        }
        else if(so >= 0 && so < 10) {
            s += char(so + '0');
        }
        modify_input(s, 1);
    }
    else if(ds[23].disable == 0 && ds[23].clicked) {
        std::string& s = ds[23].text;
        if(back_space == 1) {
            if(s.size()) s.pop_back();
        }
        else if(so >= 0 && so < 10) {
            s += char(so + '0');
        }
        modify_input(s, 3);
    }
    else if(ds[24].disable == 0 && ds[24].clicked) {
        std::string& s = ds[24].text;
        if(back_space == 1) {
            if(s.size()) s.pop_back();
        }
        else if(so >= 0 && so < 10) {
            s += char(so + '0');
        }
        modify_input(s, 2);
    }
    else if(ds[25].disable == 0 && ds[25].clicked) {
        std::string& s = ds[25].text;
        if(back_space == 1) {
            if(s.size()) s.pop_back();
        }
        else if(so >= 0 && so < 10) {
            s += char(so + '0');
        }
        modify_input(s, 3);      
    }
    else if(ds[27].disable == 0 && ds[27].clicked) {
        std::string& s = ds[27].text;
        if(back_space == 1) {
            if(s.size()) s.pop_back();
        }
        else if(so >= 0 && so < 10) {
            s += char(so + '0');
        }
        modify_input(s, 2);      
    }

    if(ds[19].clicked) {
        int tmp = rand_4(1, 30);
        ds[18].text = std::to_string(tmp);
        ds[19].clicked = 0;
    }

    back_space = 0;
    so = 10;

    if(ds[20].clicked) {

        const char *filterPatterns[] = { "*.txt"};
        const char *filePath = tinyfd_openFileDialog(
            "Open an Image File",         // Tiêu đề
            "",                           // Không có đường dẫn và tên file mặc định
            1,                            // 3 kiểu file lọc
            filterPatterns,               // Mảng chứa các kiểu file lọc
            NULL,                         // Không có mô tả cho các kiểu file
            0                             // Không cho phép chọn nhiều file
        );

        if(filePath != NULL) {
            std::ifstream fin;
            fin.open(filePath);

            std::vector<int>a;

            if(fin.is_open()) {
                std::string s;
                int d = 0;
                while(getline(fin, s)) {
                    int tmp = 0;
                    for(int i = 0; i < s.size(); i++) {
                        if(s[i] == ' ') {
                            a.push_back(tmp);
                            tmp = 0;
                        }
                        else {
                            tmp = tmp * 10 + s[i] - '0';
                        }
                    }
                    a.push_back(tmp);
                }
            }
            if(a.size() <= 30) {
                ds_node.clear();
                tree_state.clear();
                postition_state.clear();
                tree_state.push_back(ds_node);
                postition_state.push_back(ds_node);
                cur_state = 0;
                for(int i = 0; i < a.size(); i++) {
                    insert_heap(a[i]);
                }
                is_play = 1;
                ds[14].disable = 0;
                ds[13].disable = 1;
                ds[12].disable = 1;
            }


            fin.close();
        }

        ds[20].clicked = 0;
    }

    if(ds[21].clicked) {
        tree_state.clear();
        postition_state.clear();
        cur_state = 0;
        ds_node.clear();
        tree_state.push_back(ds_node);
        postition_state.push_back(ds_node);
        int n = 0;
        for(int i = 0; i < ds[18].text.size(); i++) {
            n = n * 10 + ds[18].text[i] - '0';
        }
        for(int i = 1; i <= n; i++) {
            int val = rand_4(1, 99);
            insert_heap(val);
        }
        ds[21].clicked = 0;
        is_play = 1;
        ds[14].disable = 0;
        ds[13].disable = 1;
        ds[12].disable = 1;
    }

    if(ds[22].clicked) {
        tree_state.clear();
        postition_state.clear();
        tree_state.push_back(ds_node);
        postition_state.push_back(ds_node);
        cur_state = 0;
        int val = 0;
        for(int i = 0; i < ds[23].text.size(); i++) {
            val = val * 10 + ds[23].text[i] - '0';
        }    
        insert_heap(val);
        ds[22].clicked = 0;
        is_play = 1;
        ds[14].disable = 0;
        ds[13].disable = 1;
        ds[12].disable = 1;
    }

    if(ds[4].clicked) {
        tree_state.clear();
        postition_state.clear();
        tree_state.push_back(ds_node);
        postition_state.push_back(ds_node);
        cur_state = 0;
        extract_max();
        ds[4].clicked = 0;
        is_play = 1;
        ds[14].disable = 0;
        ds[13].disable = 1;
        ds[12].disable = 1;
    }

    if(ds[26].clicked) {
        tree_state.clear();
        postition_state.clear();
        tree_state.push_back(ds_node);
        postition_state.push_back(ds_node);
        cur_state = 0;

        int vt = 0;

        for(int i = 0; i < ds[24].text.size(); i++) {
            vt = vt * 10 + ds[24].text[i] - '0';
        }

        int val = 0;
        for(int i = 0; i < ds[25].text.size(); i++) {
            val = val * 10 + ds[25].text[i] - '0';
        }


        update_pos(vt, val);
        ds[26].clicked = 0;
        is_play = 1;
        ds[14].disable = 0;
        ds[13].disable = 1;
        ds[12].disable = 1;
    }


    if(ds[28].clicked) {
        tree_state.clear();
        postition_state.clear();
        tree_state.push_back(ds_node);
        postition_state.push_back(ds_node);
        cur_state = 0;

        int vt = 0;

        for(int i = 0; i < ds[27].text.size(); i++) {
            vt = vt * 10 + ds[27].text[i] - '0';
        }

        delete_pos(vt);
        ds[28].clicked = 0;
        is_play = 1;
        ds[14].disable = 0;
        ds[13].disable = 1;
        ds[12].disable = 1;
    }

    //full_back
    if(ds[10].clicked) {
        cur_state = 0;
        ds[10].clicked = 0;
        is_play = 0;
        ds[13].disable = 0;
        ds[14].disable = 1;
        ds[12].disable = 1;
    }


    //back_step

    if(ds[11].clicked) {
        cur_state -= 30;
        if(cur_state < 0) cur_state = 0;
        ds[11].clicked = 0;
        is_play = 0;
        ds[13].disable = 0;
        ds[14].disable = 1;
        ds[12].disable = 1;
    }

    //return
    if(ds[12].clicked) {
        cur_state = 0;
        ds[12].clicked = 0;
        ds[12].disable = 1;
        ds[14].disable = 0;
        is_play = 1;
    }

    //play
    if(ds[13].clicked) {
        ds[13].clicked = 0;
        ds[13].disable = 1;
        ds[14].disable = 0;
        is_play = 1;
        ds[12].disable = 1;
    }


    //pause
    if(ds[14].clicked) {
        ds[14].clicked = 0;
        is_play = 0;
        ds[13].disable = 0;
        ds[14].disable = 1;
        ds[12].disable = 1;
    }


    //go_step
    if(ds[15].clicked) {
        ds[15].clicked = 0;
        cur_state += 30;
        if(cur_state >= tree_state.size()) cur_state = tree_state.size() - 1;
        is_play = 0;
        ds[13].disable = 0;
        ds[14].disable = 1;
        ds[12].disable = 1;
    }

    //full_go
    if(ds[16].clicked) {
        ds[16].clicked = 0;
        cur_state = tree_state.size() - 1;
        is_play = 0;
        ds[13].disable = 0;
        ds[14].disable = 1;
        ds[12].disable = 1;
    }


    //speed_button
    if(ds[17].pressed) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(mWindow);
        ds[17].pos_x = mousePos.x;
        ds[17].pos_x = std::max(ds[17].pos_x, 1182);
        ds[17].pos_x = std::min(ds[17].pos_x, 1317);
    }
    if(ds[17].released) {
        ds[17].released = 0;
        speed = 1.0 * (ds[17].pos_x - 1182) / 135 * 4 + 1;
    }

}

void HeapScreen::modify_input(std::string &s, int t) {
    if(t == 1) {
        //n
        int tmp = 0;
        for(int i = 0; i < s.size(); i++) {
            tmp = tmp * 10 + s[i] - '0';
        }
        if(tmp > 30) tmp = 30;
        s = std::to_string(tmp);
    }
    else if(t == 2) {
        //i
        int tmp = 0;
        for(int i = 0; i < s.size(); i++) {
            tmp = tmp * 10 + s[i] - '0';
        }
        if(tmp > ds_node.size()) tmp = ds_node.size();
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

void HeapScreen::render_image() {
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

void HeapScreen::Render() {
    mWindow.clear();
    render_image();
    cur_state = std::min(cur_state, (int)tree_state.size() - 1);
    if(cur_state >= 0 && cur_state < tree_state.size()) {
        draw_heap(tree_state[cur_state], postition_state[cur_state]);
        draw_time_bar();
    }



    mWindow.display();
    cur_state = cur_state + is_play * (int)speed;
    cur_state = std::min(cur_state, (int)tree_state.size() - 1);
    if(cur_state == tree_state.size() - 1) {
        is_play = 0;
        ds[13].disable = ds[14].disable = 1;
        ds[12].disable = 0;
    }
}




//reset
void HeapScreen::reset_heap() {
    ds_node.clear();
    tree_state.clear();
    postition_state.clear();
}

void HeapScreen::dfs_inoder(int u, int block) {
    int v = 2 * u;
    if(v <= ds_node.size()) {
        dfs_inoder(v, block);
    }
    if(u != block) ds_node[u - 1].vt = ++time_vi;
    v = 2 * u + 1;
    if(v <= ds_node.size()) {
        dfs_inoder(v, block);
    }
}

void HeapScreen::insert_heap(int val) {
    node tmp(0, 0, val);
    ds_node.push_back(tmp);

    time_vi = 0;

    if(ds_node.size()) {
        dfs_inoder(1, 0);


        animation_shift_tree_new_node(0.3f);

        up_heapify(ds_node.size());
    }
}

void HeapScreen::animation_shift_tree_new_node(float time) {

    for(int i = 0; i < ds_node.size(); i++) ds_node[i].now = ds_node[i].next = 0;
    if(ds_node.size()) ds_node.back().now = 1;

    //tree_state.clear();
    //tree_state.push_back(ds_node);
    //postition_state.push_back(ds_node);

    for(int t = 1; t <= 60 * time; t++) {
        std::vector<node>tmp;
        for(int i = 0; i < ds_node.size(); i++) {

            float en_pos_x;
            float en_pos_y;

            if(ds_node.size() % 2 == 0) {
                int mid = ds_node.size() / 2;
                if(ds_node[i].vt <= mid) {
                    en_pos_x = 720 - (mid - ds_node[i].vt) * 34 - 17;
                }
                else {
                    en_pos_x = 720 + (ds_node[i].vt - (mid + 1)) * 34 + 17;
                }
            }
            else {
                int mid = ds_node.size() / 2 + 1;
                en_pos_x = 720 + (ds_node[i].vt - mid) * 34;
            }

            int hi = 0;
            while((1<<hi) <= i + 1) {
                hi++;
            }

            en_pos_y = 120 + (hi - 1) * 60; //len = 50, base = 30

            float new_pos_x = ds_node[i].pos_x + (en_pos_x - ds_node[i].pos_x) * 1.0 * t / (60 * time);
            float new_pos_y = ds_node[i].pos_y + (en_pos_y - ds_node[i].pos_y) * 1.0 * t / (60 * time);
            tmp.push_back(ds_node[i]);
            tmp.back().pos_x = new_pos_x;
            tmp.back().pos_y = new_pos_y;

            if(i == ds_node.size() - 1) {

                tmp.back().pos_x = en_pos_x;
                tmp.back().pos_y = en_pos_y;

                int u = i + 1;
                int cha = u / 2;
                float new_per_node = ds_node[i].per_node + 255 * 1.0 * t / (60 * time);
                tmp.back().per_node = new_per_node;

                if(cha >= 1) {
                    if(2 * cha == u) {
                        float new_per_edge_l = ds_node[cha].per_edge_l + 255 * 1.0 * t / (60 * time);
                        tmp[cha - 1].per_edge_l = new_per_edge_l;
                    }
                    else {
                        float new_per_edge_r = ds_node[cha].per_edge_r + 255 * 1.0 * t / (60 * time);
                        tmp[cha - 1].per_edge_r = new_per_edge_r;
                    }
                }
            }
        }

        tree_state.push_back(tmp);
        postition_state.push_back(tmp);
    }


    set_ds_node();
    tree_state.push_back(ds_node);
    postition_state.push_back(ds_node);
}

void HeapScreen::up_heapify(int u) {
    int cha = u / 2;
    if(cha < 1) return;

    for(int i = 0; i < ds_node.size(); i++) {
        ds_node[i].now = ds_node[i].next = 0;
    }
    ds_node[u - 1].now = 1;



    if(ds_node[cha - 1].val < ds_node[u - 1].val) {


        animation_swap_node(cha, u, 0.3f);
        up_heapify(u / 2);
    }
    return;
}

void HeapScreen::animation_swap_node(int cha, int u, float time) {

    for(int i = 0; i < ds_node.size(); i++) {
        ds_node[i].next = ds_node[i].now = 0;
    }
    ds_node[u - 1].now = 1;
    ds_node[cha - 1].next = 1;


    tree_state.push_back(ds_node);
    postition_state.push_back(ds_node);


    for(int t = 1; t <= 30 * time; t++) {
        std::vector<node>tmp;
        for(int i = 0; i < ds_node.size(); i++) {
            tmp.push_back(ds_node[i]);
            if(i == cha - 1) {
                tmp.back().per_next = t / (30 * time);
            }
        }
        tree_state.push_back(tmp);
        postition_state.push_back(ds_node);
    }

    ds_node[cha - 1].per_next = 1;

    for(int t = 1; t <= 60 * time; t++) {
        std::vector<node>tmp;

        postition_state.push_back(ds_node);
        for(int i = 0; i < ds_node.size(); i++) {
            tmp.push_back(ds_node[i]);
            if(i + 1 == u) {
                float new_pos_x = ds_node[i].pos_x + (ds_node[cha - 1].pos_x - ds_node[i].pos_x) * 1.0 * t / (60 * time);
                float new_pos_y = ds_node[i].pos_y + (ds_node[cha - 1].pos_y - ds_node[i].pos_y) * 1.0 * t / (60 * time);

                tmp.back().pos_x = new_pos_x;
                tmp.back().pos_y = new_pos_y;
            }
            else if(i + 1 == cha) {
                float new_pos_x = ds_node[i].pos_x + (ds_node[u - 1].pos_x - ds_node[i].pos_x) * 1.0 * t / (60 * time);
                float new_pos_y = ds_node[i].pos_y + (ds_node[u - 1].pos_y - ds_node[i].pos_y) * 1.0 * t / (60 * time);

                tmp.back().pos_x = new_pos_x;
                tmp.back().pos_y = new_pos_y;
            }
        }
        tree_state.push_back(tmp);
    }

    std::swap(ds_node[u - 1].val, ds_node[cha - 1].val);
    ds_node[u - 1].now = 0;
    ds_node[u - 1].next = 1;

    ds_node[cha - 1].now = 1;
    ds_node[cha - 1].next = 0;

    ds_node[cha - 1].per_next = 0;
    ds_node[u - 1].per_next = 1;

    postition_state.push_back(ds_node);
    tree_state.push_back(ds_node);

    for(int t = 1; t <= 30 * time; t++) {
        std::vector<node>tmp;
        for(int i = 0; i < ds_node.size(); i++) {
            tmp.push_back(ds_node[i]);
            if(i == u - 1) {
                tmp.back().per_next = (30 * time - t) / (30 * time);
            }
        }
        tree_state.push_back(tmp);
        postition_state.push_back(ds_node);
    }

    ds_node[u - 1].per_next = 0;



    postition_state.push_back(ds_node);
    tree_state.push_back(ds_node);
}

void HeapScreen::set_ds_node() {
    for(int i = 0; i < ds_node.size(); i++) {

        if(ds_node.size() % 2 == 0) {
            int mid = ds_node.size() / 2;
            if(ds_node[i].vt <= mid) {
                ds_node[i].pos_x = 720 - (mid - ds_node[i].vt) * 34 - 17;
            }
            else {
                ds_node[i].pos_x = 720 + (ds_node[i].vt - (mid + 1)) * 34 + 17;
            }
        }
        else {
            int mid = ds_node.size() / 2 + 1;
            ds_node[i].pos_x = 720 + (ds_node[i].vt - mid) * 34;
        }

        int hi = 0;
        while((1<<hi) <= i + 1) {
            hi++;
        }

        ds_node[i].pos_y = 120 + (hi - 1) * 60;

        ds_node[i].per_node = 255;
        if(2 * (i + 1) <= ds_node.size()) {
            ds_node[i].per_edge_l = 255;
        }
        if(2 * (i + 1) + 1 <= ds_node.size()) {
            ds_node[i].per_edge_r = 255;
        }
    }
}

//draw

// Hàm để tính chiều dài của đường thẳng
float HeapScreen::length(const sf::Vector2f& start, const sf::Vector2f& end) {
    // Tính hiệu giữa tọa độ x và y của hai điểm
    float dx = end.x - start.x;
    float dy = end.y - start.y;
    
    // Tính chiều dài theo định lý Pythagoras
    return std::sqrt(dx * dx + dy * dy);
}

// Hàm để tính góc của đường thẳng (tính bằng radian)
float HeapScreen::angle(const sf::Vector2f& start, const sf::Vector2f& end) {
    // Tính hiệu giữa tọa độ x và y của hai điểm
    float dy = end.y - start.y;
    float dx = end.x - start.x;
    
    // Tính góc theo hàm atan2
    return std::atan2(dy, dx) * 180 / 3.14159265f;
}

void HeapScreen::draw_heap(std::vector<node>& tree_stage, std::vector<node>& postition_stage) {
    draw_node(1, tree_stage, postition_stage);
}


void HeapScreen::draw_node(int u, std::vector<node>& tree_state, std::vector<node>& postition_state) {
    if(u > tree_state.size()) return;

    // Vẽ cạnh nối đến con trái
    int v = 2 * u;
    if(v <= tree_state.size()) {

        sf::Vector2f start(postition_state[u - 1].pos_x, postition_state[u - 1].pos_y);
        sf::Vector2f end(postition_state[v - 1].pos_x, postition_state[v - 1].pos_y);
    
        // Tạo hình chữ nhật để làm đường thẳng dày hơn
        sf::RectangleShape Line;
        float thickness = 3.5f; // Độ dày của đường thẳng
        float lineLength = length(start, end);

        // Thiết lập kích thước và góc cho hình chữ nhật
        Line.setSize(sf::Vector2f(lineLength, thickness));
        Line.setFillColor(sf::Color(0, 0, 0, postition_state[u - 1].per_edge_l));
        Line.setOrigin(0, thickness / 2); // Đặt tâm hình chữ nhật để xoay

        // Đặt vị trí và góc cho hình chữ nhật
        Line.setPosition(start);
        Line.setRotation(angle(start, end)); // Chuyển đổi từ radian sang độ

        mWindow.draw(Line);

        draw_node(v, tree_state, postition_state);
    }

    // Vẽ cạnh nối đến con phải
    v = 2 * u + 1;
    if(v <= tree_state.size()) {

        sf::Vector2f start(postition_state[u - 1].pos_x, postition_state[u - 1].pos_y);
        sf::Vector2f end(postition_state[v - 1].pos_x, postition_state[v - 1].pos_y);
    
        // Tạo hình chữ nhật để làm đường thẳng dày hơn
        sf::RectangleShape Line;
        float thickness = 3.5f; // Độ dày của đường thẳng
        float lineLength = length(start, end);

        // Thiết lập kích thước và góc cho hình chữ nhật
        Line.setSize(sf::Vector2f(lineLength, thickness));
        Line.setFillColor(sf::Color(0, 0, 0, postition_state[u - 1].per_edge_r));
        Line.setOrigin(0, thickness / 2); // Đặt tâm hình chữ nhật để xoay

        // Đặt vị trí và góc cho hình chữ nhật
        Line.setPosition(start);
        Line.setRotation(angle(start, end)); // Chuyển đổi từ radian sang độ

        mWindow.draw(Line);

        draw_node(v, tree_state, postition_state);
    }



    // Vẽ node hiện tại
    sf::CircleShape circle(15, 100);
    circle.setOrigin(circle.getRadius(), circle.getRadius());
    if(tree_state[u - 1].now == 0) {
        circle.setFillColor(sf::Color(255, 255, 255, tree_state[u - 1].per_node));
        circle.setOutlineColor(sf::Color(0, 0, 0, tree_state[u - 1].per_node));
    }
    else {
        circle.setFillColor(sf::Color(255, 165, 0, tree_state[u - 1].per_node));
        circle.setOutlineColor(sf::Color(255, 165, 0, tree_state[u - 1].per_node));
    }
    circle.setOutlineThickness(3);
    circle.setPosition(tree_state[u - 1].pos_x, tree_state[u - 1].pos_y);




    //Vẽ giá trị
    sf::Text Value;
    Value.setFont(MyFont.get(FontID::ptsan));
    Value.setString(std::to_string(tree_state[u - 1].val));
    Value.setCharacterSize(16);
    if(tree_state[u - 1].now == 0) Value.setFillColor(sf::Color(0, 0, 0, tree_state[u - 1].per_node));
    else {
        Value.setFillColor(sf::Color(255, 255, 255, tree_state[u - 1].per_node));
    }

    sf::FloatRect textRect = Value.getLocalBounds();
    Value.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top  + textRect.height / 2.0f);
    Value.setPosition(tree_state[u - 1].pos_x, tree_state[u - 1].pos_y);


    //Vẽ ID
    sf::Text ID;
    ID.setFont(MyFont.get(FontID::ptsan));
    ID.setString(std::to_string(u));
    ID.setCharacterSize(16);
    ID.setFillColor(sf::Color(255, 0, 0, tree_state[u - 1].per_node));

    textRect = ID.getLocalBounds();
    ID.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top  + textRect.height / 2.0f);
    ID.setPosition(tree_state[u - 1].pos_x, tree_state[u - 1].pos_y + 30);


    if(tree_state[u - 1].next) { 
        float ratio = tree_state[u - 1].per_next; 
        circle.setOutlineColor(sf::Color(ratio * 255, ratio * 165, 0, tree_state[u - 1].per_node));
        Value.setFillColor(sf::Color(ratio * 255, ratio * 165, 0, tree_state[u - 1].per_node));
    }

    mWindow.draw(circle);
    mWindow.draw(Value);
    mWindow.draw(ID);

    return;

}

void HeapScreen::draw_time_bar() {


    //rgb(116,255,188)
    float per = 1.0 * (cur_state + 1) / (int)tree_state.size();

    sf::RectangleShape Line;

    Line.setSize(sf::Vector2f(500.0f * per, 15));
    Line.setFillColor(sf::Color(116, 225, 188));

    // Đặt vị trí và góc cho hình chữ nhật
    Line.setPosition(468, 864);

    mWindow.draw(Line);

}

void HeapScreen::extract_max() {
    if(ds_node.size()) {
        if(ds_node.size() > 1) animation_swap_node(1, ds_node.size(), 0.3f);
        delete_node((int)ds_node.size());
    }
}

void HeapScreen::delete_node(int u) {
    time_vi = 0;
    dfs_inoder(1, u);
    animation_shift_tree_delete_node(0.3f);
    down_heapify(1);
}

void HeapScreen::animation_shift_tree_delete_node(float time) {


    //tree_state.clear();
    //tree_state.push_back(ds_node);
    //postition_state.push_back(ds_node);

    for(int t = 1; t <= 60 * time; t++) {
        std::vector<node>tmp;
        for(int i = 0; i < ds_node.size(); i++) {


            float en_pos_x;
            float en_pos_y;

            if((ds_node.size() - 1) % 2 == 0) {
                int mid = (ds_node.size() - 1) / 2;
                if(ds_node[i].vt <= mid) {
                    en_pos_x = 720 - (mid - ds_node[i].vt) * 34 - 17;
                }
                else {
                    en_pos_x = 720 + (ds_node[i].vt - (mid + 1)) * 34 + 17;
                }
            }
            else {
                int mid = (ds_node.size() - 1) / 2 + 1;
                en_pos_x = 720 + (ds_node[i].vt - mid) * 34;
            }

            int hi = 0;
            while((1<<hi) <= i + 1) {
                hi++;
            }

            en_pos_y = 120 + (hi - 1) * 60; //len = 50, base = 30

            float new_pos_x = ds_node[i].pos_x + (en_pos_x - ds_node[i].pos_x) * 1.0 * t / (60 * time);
            float new_pos_y = ds_node[i].pos_y + (en_pos_y - ds_node[i].pos_y) * 1.0 * t / (60 * time);
            tmp.push_back(ds_node[i]);
            tmp.back().pos_x = new_pos_x;
            tmp.back().pos_y = new_pos_y;

            if(i == ds_node.size() - 1) {

                tmp.back().pos_x = ds_node[i].pos_x;
                tmp.back().pos_y = ds_node[i].pos_y;

                int u = i + 1;
                int cha = u / 2;
                float new_per_node = 255 - 255 * 1.0 * t / (60 * time);
                tmp.back().per_node = new_per_node;

                if(cha >= 1) {
                    if(2 * cha == u) {
                        float new_per_edge_l = 255 - 255 * 1.0 * t / (60 * time);
                        tmp[cha - 1].per_edge_l = new_per_edge_l;
                    }
                    else {
                        float new_per_edge_r = 255 - 255 * 1.0 * t / (60 * time);
                        tmp[cha - 1].per_edge_r = new_per_edge_r;
                    }
                }
            }
        }

        tree_state.push_back(tmp);
        postition_state.push_back(tmp);
    }


    set_ds_node_delete();
    tree_state.push_back(ds_node);
    postition_state.push_back(ds_node);
}

void HeapScreen::set_ds_node_delete() {
    ds_node.pop_back();
    for(int i = 0; i < ds_node.size(); i++) {

        if(ds_node.size() % 2 == 0) {
            int mid = ds_node.size() / 2;
            if(ds_node[i].vt <= mid) {
                ds_node[i].pos_x = 720 - (mid - ds_node[i].vt) * 34 - 17;
            }
            else {
                ds_node[i].pos_x = 720 + (ds_node[i].vt - (mid + 1)) * 34 + 17;
            }
        }
        else {
            int mid = ds_node.size() / 2 + 1;
            ds_node[i].pos_x = 720 + (ds_node[i].vt - mid) * 34;
        }

        int hi = 0;
        while((1<<hi) <= i + 1) {
            hi++;
        }

        ds_node[i].pos_y = 120 + (hi - 1) * 60; //len = 50, base = 30

        ds_node[i].per_node = 255;
        if(2 * (i + 1) <= ds_node.size()) {
            ds_node[i].per_edge_l = 255;
        }
        if(2 * (i + 1) + 1 <= ds_node.size()) {
            ds_node[i].per_edge_r = 255;
        }
    }
}

void HeapScreen::down_heapify(int u) {
    int L = 2 * u;
    int R = 2 * u + 1;
    if(R <= ds_node.size()) {
        if(ds_node[L - 1].val > ds_node[R - 1].val) {
            if(ds_node[L - 1].val > ds_node[u - 1].val) {
                animation_swap_node(L, u, 0.3f);
                down_heapify(L);
            }
        }
        else {
            if(ds_node[R - 1].val > ds_node[u - 1].val) {
                animation_swap_node(R, u, 0.3f);
                down_heapify(R);
            }
        }
    }
    else if(L <= ds_node.size()) {
        if(ds_node[L - 1].val > ds_node[u - 1].val) {
            animation_swap_node(L, u, 0.3f);
            down_heapify(L);
        }
    }
    return;
}

void HeapScreen::update_pos(int vt, int val) {


    if(vt > ds_node.size() || vt == 0) return;

    ds_node[vt - 1].val = val;
    for(int i = 0; i < ds_node.size(); i++) {
        ds_node[i].now = ds_node[i].next = 0;
    }
    ds_node[vt - 1].now = 1;
    //tree_state.clear();
    //postition_state.clear();
    tree_state.push_back(ds_node);
    postition_state.push_back(ds_node);

    up_heapify(vt);

    int cur = -1;
    for(int i = 0; i < ds_node.size(); i++) {
        if(ds_node[i].now == 1) cur = i + 1;
    }
    down_heapify(cur);

    return;
}

void HeapScreen::delete_pos(int vt) {

    if(vt > ds_node.size() || vt == 0) return;
    
    int val = ds_node[0].val + 1;
    ds_node[vt - 1].val = val;
    for(int i = 0; i < ds_node.size(); i++) {
        ds_node[i].now = ds_node[i].next = 0;
    }
    ds_node[vt - 1].now = 1;
    //tree_state.clear();
    //postition_state.clear();
    tree_state.push_back(ds_node);
    postition_state.push_back(ds_node);

    up_heapify(vt);

    extract_max();
    

    return;
}

