#include <HashScreen.hpp>
#include <MenuScreen.hpp>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <random>
#include <tinyfiledialogs.h>
#include <fstream>

std::mt19937 rng_1(std::chrono::steady_clock::now().time_since_epoch().count());

int rand_1(int l, int r) {
    return l + rng_1() % (r - l + 1);
}

void HashScreen::reset_node(node *tmp) {
    if(tmp->child != NULL) {
        reset_node(tmp->child);
    }
    delete tmp;
    tmp = NULL;
}
void HashScreen::reset_ds_node(std::vector<node>&tmp) {
    for(int i = 0; i < tmp.size(); i++) {
        if(tmp[i].child != NULL) {
            reset_node(tmp[i].child);
        }
    }

    tmp.clear();
}
HashScreen::node* HashScreen::copy_node(node *tmp) {
    node* new_tmp = new node;
    *new_tmp = *tmp;
    if(tmp -> child != NULL) {
        new_tmp -> child = copy_node(tmp -> child);
    }
    return new_tmp;
}

void HashScreen::copy_ds_node(std::vector<node>& new_ds_node) {
    new_ds_node.clear();
    for(int i = 0; i < ds_node.size(); i++) {
        node *tmp = new node;
        *tmp = ds_node[i];
        if(ds_node[i].child != NULL) {
            tmp -> child = copy_node(ds_node[i].child);
        }
        new_ds_node.push_back(*tmp);
    }
}

void HashScreen::new_ds_node(int m) {

    while(hash_state.size()) {
        reset_ds_node(hash_state.back());
        hash_state.pop_back();
    }

    reset_ds_node(ds_node);
    cur_state = 0;


    for(int i = 0; i < m; i++) {
        ds_node.push_back(node());
        if(m % 2) {
            int mid = m / 2;
            if(i == mid) {
                ds_node[i].pos_x = 720;
            }
            else if(i < mid) {
                ds_node[i].pos_x = 720 - 50 * (mid - i);
            }
            else {
                ds_node[i].pos_x = 720 + 50 * (i - mid);
            }
        }
        else {
            int mid = m / 2 - 1;
            if(i <= mid) {
                ds_node[i].pos_x = 695 - 50 * (mid - i);
            }
            else {
                ds_node[i].pos_x = 695 + 50 * (i - mid);
            }
        }
        ds_node[i].pos_y = 120;
        ds_node[i].val = i;
        ds_node[i].root = 1;
    }

    mod = m;
}


void HashScreen::run() {
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
    ds.push_back(image(TextureID::m_input, TextureID::m_input, 160, 617, 0));//17
    ds.push_back(image(TextureID::random_button, TextureID::random_button, 247, 624, 0));//18
    ds.push_back(image(TextureID::n_input, TextureID::n_input, 300, 617, 0));//19
    ds.push_back(image(TextureID::random_button, TextureID::random_button, 387, 624, 0));//20
    ds.push_back(image(TextureID::go_button, TextureID::go_button_in, 440, 617, 0));//21
    ds.push_back(image(TextureID::file_button, TextureID::file_button_in, 500, 617, 0));//22


    //insert
    ds.push_back(image(TextureID::val_input, TextureID::val_input, 160, 667, 0));//23
    ds.push_back(image(TextureID::go_button, TextureID::go_button_in, 270, 667, 0));//24

    //search
    ds.push_back(image(TextureID::val_input, TextureID::val_input, 160, 717, 0));//25
    ds.push_back(image(TextureID::go_button, TextureID::go_button_in, 270, 717, 0));//26

    //delete
    ds.push_back(image(TextureID::val_input, TextureID::val_input, 160, 767, 0));//27
    ds.push_back(image(TextureID::go_button, TextureID::go_button_in, 270, 767, 0));//28

    ds[17].input = 1;
    ds[17].box_pos_x = 212;
    ds[17].box_pos_y = 632;


    ds[19].input = 1;
    ds[19].box_pos_x = 352;
    ds[19].box_pos_y = 632;


    ds[23].input = 1;
    ds[23].box_pos_x = 225;
    ds[23].box_pos_y = 682;


    ds[25].input = 1;
    ds[25].box_pos_x = 225;
    ds[25].box_pos_y = 732;


    ds[27].input = 1;
    ds[27].box_pos_x = 225;
    ds[27].box_pos_y = 782;






    while(mWindow.isOpen()) {
        ProcessEvent();
        Update();
        if(Return  == 1) {
            new_ds_node(0);
            break;
        }
        Render();
    }
}

void HashScreen::ProcessEvent() {
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
                        else if(i >= 17 && i <= 28) {
                            for(int j = 17; j <= 28; j++) ds[j].clicked = 0;
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
                if(ds[21].disable == 0) {
                    ds[21].clicked = 1;
                }
                else if(ds[24].disable == 0) {
                    ds[24].clicked = 1;
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


void HashScreen::Update() {

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
        ds[17].disable = ds[18].disable = ds[19].disable = ds[20].disable = ds[21].disable = ds[22].disable = 0;
    }
    else {
        ds[17].disable = ds[18].disable = ds[19].disable = ds[20].disable = ds[21].disable = ds[22].disable = 1;
        ds[17].text = "";
        ds[19].text = "";
    }

    //insert_button
    if(ds[3].clicked == 1) {
        ds[23].disable = ds[24].disable = 0;
    }
    else {
        ds[23].disable = ds[24].disable = 1;
        ds[23].text = "";
    }

    //search_button
    if(ds[4].clicked == 1) {
        ds[25].disable = ds[26].disable = 0;
    }
    else {
        ds[25].disable = ds[26].disable = 1;
        ds[25].text = "";
    }

    //delete_button
    if(ds[5].clicked == 1) {
        ds[27].disable = ds[28].disable = 0;
    }
    else {
        ds[27].disable = ds[28].disable = 1;
        ds[27].text = "";
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
    else if(ds[19].disable == 0 && ds[19].clicked == 1) {
        std::string &s = ds[19].text;
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
        modify_input(s, 3);
    }
    else if(ds[25].disable == 0 && ds[25].clicked == 1) {
        std::string &s = ds[25].text;
        if(back_space == 1) {
            if(s.size()) s.pop_back();
        }
        else if(so >= 0 && so < 10) {
            s += char(so + '0');
        }
        modify_input(s, 3);
    }
    else if(ds[27].disable == 0 && ds[27].clicked == 1) {
        std::string &s = ds[27].text;
        if(back_space == 1) {
            if(s.size()) s.pop_back();
        }
        else if(so >= 0 && so < 10) {
            s += char(so + '0');
        }
        modify_input(s, 3);
    }

    so = 10;
    back_space = 0;


    //random m create
    if(ds[18].disable == 0 && ds[18].clicked == 1) {
        int tmp = rand_1(1, 20);
        ds[17].text = std::to_string(tmp);
        ds[18].clicked = 0;
    }

    //random n create
    if(ds[20].disable == 0 && ds[20].clicked == 1) {
        int tmp = rand_1(1, 30);
        ds[19].text = std::to_string(tmp);
        ds[20].clicked = 0;
    }


    //go create
    if(ds[21].disable == 0 && ds[21].clicked == 1) {
        int m = 0;
        for(int i = 0; i < ds[17].text.size(); i++) {
            m = m * 10 + ds[17].text[i] - '0';
        }
        new_ds_node(m);

        int n = 0;
        for(int i = 0; i < ds[19].text.size(); i++) {
            n = n * 10 + ds[19].text[i] - '0';
        }

        for(int i = 1; i <= n; i++) {
            int val = rand_1(1, 99);
            insert_hash(val);
        }

        while(hash_state.size()) {
            reset_ds_node(hash_state.back());
            hash_state.pop_back();
        }
        cur_state = 0;

        reset_color();

        std::vector<node>tmp;
        copy_ds_node(tmp);
        hash_state.push_back(tmp);

        ds[21].clicked = 0;
        is_play = 1;
        ds[13].disable = 0;
        ds[12].disable = 1;
        ds[11].disable = 1;
    }

    //file create
    if(ds[22].disable == 0 && ds[22].clicked == 1) {
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
            int m = 0;

            if(fin.is_open()) {
                std::string s;
                int d = 0;
                while(getline(fin, s)) {
                    d++;
                    if(d == 1) {
                        for(int i = 0; i < s.size(); i++) {
                            m = m * 10 + s[i] - '0';
                        }
                    }
                    else {
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
            }

            new_ds_node(m);
            cur_state = 0;

            for(int i = 0; i < a.size(); i++) {
                insert_hash(a[i]);
            }

            while(hash_state.size()) {
                reset_ds_node(hash_state.back());
                hash_state.pop_back();
            }
            cur_state = 0;

            reset_color();

            std::vector<node>tmp;
            copy_ds_node(tmp);
            hash_state.push_back(tmp);


            fin.close();
        }

        ds[22].clicked = 0;
        is_play = 1;
        ds[13].disable = 0;
        ds[12].disable = 1;
        ds[11].disable = 1;
    }

    //go insert
    if(ds[24].clicked && ds[24].disable == 0) {
        while(hash_state.size()) {
            reset_ds_node(hash_state.back());
            hash_state.pop_back();
        }
        cur_state = 0;
        int tmp = 0;
        for(int i = 0; i < ds[23].text.size(); i++) {
            tmp = tmp * 10 + ds[23].text[i] - '0';
        }
        insert_hash(tmp);
        ds[24].clicked = 0;
        is_play = 1;
        ds[13].disable = 0;
        ds[12].disable = 1;
        ds[11].disable = 1;
    }


    //go search 
    if(ds[26].clicked && ds[26].disable == 0) {
        while(hash_state.size()) {
            reset_ds_node(hash_state.back());
            hash_state.pop_back();
        }
        cur_state = 0;
        int tmp = 0;
        for(int i = 0; i < ds[25].text.size(); i++) {
            tmp = tmp * 10 + ds[25].text[i] - '0';
        }
        search_hash(tmp);
        ds[26].clicked = 0;
        is_play = 1;
        ds[13].disable = 0;
        ds[12].disable = 1;
        ds[11].disable = 1;
    }


    //go delete
    if(ds[28].clicked && ds[28].disable == 0) {
        int tmp = 0;
        while(hash_state.size()) {
            reset_ds_node(hash_state.back());
            hash_state.pop_back();
        }
        cur_state = 0;
        for(int i = 0; i < ds[27].text.size(); i++) {
            tmp = tmp * 10 + ds[27].text[i] - '0';
        }
        delete_hash(tmp);
        ds[28].clicked = 0;
        is_play = 1;
        ds[13].disable = 0;
        ds[12].disable = 1;
        ds[11].disable = 1;
    }

    //full_back
    if(ds[9].clicked) {
        cur_state = 0;
        ds[9].clicked = 0;
        is_play = 0;
        ds[12].disable = 0;
        ds[13].disable = 1;
        ds[11].disable = 1;
    }


    //back_step

    if(ds[10].clicked) {
        cur_state -= 30;
        if(cur_state < 0) cur_state = 0;
        ds[10].clicked = 0;
        is_play = 0;
        ds[12].disable = 0;
        ds[13].disable = 1;
        ds[11].disable = 1;
    }

    //return
    if(ds[11].clicked) {
        cur_state = 0;
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
        cur_state += 30;
        if(cur_state >= hash_state.size()) cur_state = hash_state.size() - 1;
        is_play = 0;
        ds[12].disable = 0;
        ds[13].disable = 1;
        ds[11].disable = 1;
    }

    //full_go
    if(ds[15].clicked) {
        ds[15].clicked = 0;
        cur_state = hash_state.size() - 1;
        is_play = 0;
        ds[12].disable = 0;
        ds[13].disable = 1;
        ds[11].disable = 1;
    }



}

void HashScreen::modify_input(std::string &s, int t) {
    if(t == 1) {
        //m
        int tmp = 0;
        for(int i = 0; i < s.size(); i++) {
            tmp = tmp * 10 + s[i] - '0';
        }
        if(tmp > 20) tmp = 20;
        s = std::to_string(tmp);
    }
    else if(t == 2) {
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


void HashScreen::render_image() {
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

void HashScreen::Render() {
    mWindow.clear();
    render_image();
    cur_state = std::min(cur_state, (int)hash_state.size() - 1);
    if(cur_state >= 0 && cur_state < hash_state.size()) {
        draw_hash(hash_state[cur_state]);
        draw_time_bar();
    }



    mWindow.display();
    cur_state = cur_state + is_play * (int)speed;
    cur_state = std::min(cur_state, (int)hash_state.size() - 1);
    if(cur_state == hash_state.size() - 1) {
        is_play = 0;
        ds[12].disable = ds[13].disable = 1;
        ds[11].disable = 0;
    }
}

void HashScreen::animation_shift_node_down(int block, float time) {
    if(ds_node[block].child) {
        for(int i = 1; i <= 60 * time; i++) {
            std::vector<node>tmp;
            copy_ds_node(tmp);

            node *cur = tmp[block].child;
            while(cur) {
                cur -> pos_y += 1.0 * i / (60 * time) * 50;
                cur = cur -> child;
            }

            hash_state.push_back(tmp);
        }
    }
    return;
}

void HashScreen::set_ds_node(int block) {
    int now_y = 120;
    int now_x = ds_node[block].pos_x;
    node *cur = ds_node[block].child;
    while(cur) {
        now_y += 50;
        cur -> pos_y = now_y;
        cur -> pos_x = now_x;

        cur = cur -> child;
    }
}

void HashScreen::reset_color_node(node *tmp) {
    tmp->co_shape = sf::Color::White;
    tmp->co_outline = sf::Color::Black;
    tmp->co_val = sf::Color::Black;
    tmp->state = 0;

    if(tmp->child != NULL) {
        reset_color_node(tmp->child);
    }
}

void HashScreen::reset_color() {
    for(int i = 0; i < mod; i++) {
        ds_node[i].co_shape = sf::Color::White;
        ds_node[i].co_outline = sf::Color::Black;
        ds_node[i].co_val = sf::Color::Black;
        ds_node[i].state = 0;

        if(ds_node[i].child != NULL) {
            reset_color_node(ds_node[i].child);
        }
    }
    return;
}

void HashScreen::change_color(node *tmp, float time, int t) {
    //den(0,0,0)
    //trang(255, 255, 255)
    //cam(255, 165, 0)
    //xanh(0, 200, 0)
    if(t == 0) {
        sf::Color cur_chu = tmp->co_val;
        sf::Color cur_nen = tmp->co_shape;
        sf::Color cur_vien = tmp->co_outline;
        for(int i = 1; i <= 60 * time; i++) {

            //chu
            tmp->co_val.r = cur_chu.r + 1.0 * (0 - cur_chu.r) * i / (60 * time);
            tmp->co_val.g = cur_chu.g + 1.0 * (0 - cur_chu.g) * i / (60 * time);
            tmp->co_val.b = cur_chu.b + 1.0 * (0 - cur_chu.b) * i / (60 * time);


            //nen
            tmp->co_shape.r = cur_nen.r + 1.0 * (255 - cur_nen.r) * i / (60 * time);
            tmp->co_shape.g = cur_nen.g + 1.0 * (255 - cur_nen.g) * i / (60 * time);
            tmp->co_shape.b = cur_nen.b + 1.0 * (255 - cur_nen.b) * i / (60 * time);


            //vien
            tmp->co_outline.r = cur_vien.r + 1.0 * (0 - cur_vien.r) * i / (60 * time);
            tmp->co_outline.g = cur_vien.g + 1.0 * (0 - cur_vien.g) * i / (60 * time);
            tmp->co_outline.b = cur_vien.b + 1.0 * (0 - cur_vien.b) * i / (60 * time);

            std::vector<node>new_ds; 
            copy_ds_node(new_ds);
            hash_state.push_back(new_ds);
        }
        tmp->co_val = sf::Color(0, 0, 0);
        tmp->co_shape = sf::Color(255, 255, 255);
        tmp->co_outline = sf::Color(0, 0, 0);
        std::vector<node>new_ds; 
        copy_ds_node(new_ds);
        hash_state.push_back(new_ds);
    }
    else if(t == 1) {
        sf::Color cur_chu = tmp->co_val;
        sf::Color cur_nen = tmp->co_shape;
        sf::Color cur_vien = tmp->co_outline;
        for(int i = 1; i <= 60 * time; i++) {

            //chu
            tmp->co_val.r = cur_chu.r + 1.0 * (255 - cur_chu.r) * i / (60 * time);
            tmp->co_val.g = cur_chu.g + 1.0 * (165 - cur_chu.g) * i / (60 * time);
            tmp->co_val.b = cur_chu.b + 1.0 * (0 - cur_chu.b) * i / (60 * time);


            //nen
            tmp->co_shape.r = cur_nen.r + 1.0 * (255 - cur_nen.r) * i / (60 * time);
            tmp->co_shape.g = cur_nen.g + 1.0 * (255 - cur_nen.g) * i / (60 * time);
            tmp->co_shape.b = cur_nen.b + 1.0 * (255 - cur_nen.b) * i / (60 * time);


            //vien
            tmp->co_outline.r = cur_vien.r + 1.0 * (255 - cur_vien.r) * i / (60 * time);
            tmp->co_outline.g = cur_vien.g + 1.0 * (165 - cur_vien.g) * i / (60 * time);
            tmp->co_outline.b = cur_vien.b + 1.0 * (0 - cur_vien.b) * i / (60 * time);

            std::vector<node>new_ds; 
            copy_ds_node(new_ds);
            hash_state.push_back(new_ds);
        }  
        tmp->co_val = sf::Color(255, 165, 0);
        tmp->co_shape = sf::Color(255, 255, 255);
        tmp->co_outline = sf::Color(255, 165, 0);
        std::vector<node>new_ds; 
        copy_ds_node(new_ds);
        hash_state.push_back(new_ds);      
    }
    else if(t == 2) {
        sf::Color cur_chu = tmp->co_val;
        sf::Color cur_nen = tmp->co_shape;
        sf::Color cur_vien = tmp->co_outline;
        for(int i = 1; i <= 60 * time; i++) {

            //chu
            tmp->co_val.r = cur_chu.r + 1.0 * (255 - cur_chu.r) * i / (60 * time);
            tmp->co_val.g = cur_chu.g + 1.0 * (255 - cur_chu.g) * i / (60 * time);
            tmp->co_val.b = cur_chu.b + 1.0 * (255 - cur_chu.b) * i / (60 * time);


            //nen
            tmp->co_shape.r = cur_nen.r + 1.0 * (255 - cur_nen.r) * i / (60 * time);
            tmp->co_shape.g = cur_nen.g + 1.0 * (165 - cur_nen.g) * i / (60 * time);
            tmp->co_shape.b = cur_nen.b + 1.0 * (0 - cur_nen.b) * i / (60 * time);


            //vien
            tmp->co_outline.r = cur_vien.r + 1.0 * (255 - cur_vien.r) * i / (60 * time);
            tmp->co_outline.g = cur_vien.g + 1.0 * (165 - cur_vien.g) * i / (60 * time);
            tmp->co_outline.b = cur_vien.b + 1.0 * (0 - cur_vien.b) * i / (60 * time);

            std::vector<node>new_ds; 
            copy_ds_node(new_ds);
            hash_state.push_back(new_ds);
        }
        tmp->co_val = sf::Color(255, 255, 255);
        tmp->co_shape = sf::Color(255, 165, 0);
        tmp->co_outline = sf::Color(255, 165, 0);
        std::vector<node>new_ds; 
        copy_ds_node(new_ds);
        hash_state.push_back(new_ds);
    }
    else {
        sf::Color cur_chu = tmp->co_val;
        sf::Color cur_nen = tmp->co_shape;
        sf::Color cur_vien = tmp->co_outline;
        for(int i = 1; i <= 60 * time; i++) {

            //chu
            tmp->co_val.r = cur_chu.r + 1.0 * (255 - cur_chu.r) * i / (60 * time);
            tmp->co_val.g = cur_chu.g + 1.0 * (255 - cur_chu.g) * i / (60 * time);
            tmp->co_val.b = cur_chu.b + 1.0 * (255 - cur_chu.b) * i / (60 * time);


            //nen
            tmp->co_shape.r = cur_nen.r + 1.0 * (106 - cur_nen.r) * i / (60 * time);
            tmp->co_shape.g = cur_nen.g + 1.0 * (220 - cur_nen.g) * i / (60 * time);
            tmp->co_shape.b = cur_nen.b + 1.0 * (127 - cur_nen.b) * i / (60 * time);


            //vien
            tmp->co_outline.r = cur_vien.r + 1.0 * (106 - cur_vien.r) * i / (60 * time);
            tmp->co_outline.g = cur_vien.g + 1.0 * (220 - cur_vien.g) * i / (60 * time);
            tmp->co_outline.b = cur_vien.b + 1.0 * (127 - cur_vien.b) * i / (60 * time);

            std::vector<node>new_ds; 
            copy_ds_node(new_ds);
            hash_state.push_back(new_ds);
        }
        tmp->co_val = sf::Color(255, 255, 255);
        tmp->co_shape = sf::Color(106, 220, 127);
        tmp->co_outline = sf::Color(106, 220, 127);
        std::vector<node>new_ds; 
        copy_ds_node(new_ds);
        hash_state.push_back(new_ds);
    }
    tmp->state = t;
}


void HashScreen::insert_hash(int val) {
    if(mod == 0) return;


    //reset hết màu lại
    reset_color();
    std::vector<node>tmp; 
    copy_ds_node(tmp);
    hash_state.push_back(tmp);

    int block = val % mod;

    //tô màu của block đó thành màu cam
    change_color(&ds_node[block], 0.3f, 1);


    animation_shift_node_down(block, 0.3f);

    node *cur = new node;
    cur -> child = ds_node[block].child;
    cur -> val = val;
    ds_node[block].child = cur;

    set_ds_node(block);

    copy_ds_node(tmp);
    hash_state.push_back(tmp);

    change_color(ds_node[block].child, 0.3f, 2);

    return;
}

void HashScreen::dfs_find(node *tmp, node *par, int need) {
    if(par != NULL) {
        change_color(par, 0.3f, 1);
    }
    if(tmp == NULL) return;
    change_color(tmp, 0.3f, 2);
    if(par != NULL && tmp -> val == need) {
        change_color(tmp, 0.3f, 3);
    }
    else {
        dfs_find(tmp->child, tmp, need);
    }
}

void HashScreen::search_hash(int val) {
    if(mod == 0) return;
    reset_color();
    std::vector<node>tmp; 
    copy_ds_node(tmp);
    hash_state.push_back(tmp);

    int block = val % mod;

    dfs_find(&ds_node[block], NULL, val);
}

void HashScreen::animation_shift_node_up(node *tmp, float time) {
    for(int i = 1; i <= 60 * time; i++) {
        int cnt = 0;
        node *cur = tmp -> child;
        while(cur) {
            cnt++;
            cur->pos_y = tmp->pos_y + cnt * 50 - 1.0 * 50 * i / (60 * time);

            cur = cur->child;
        }

        tmp->per_edge = 255 - 255 * 1.0 * i / (60 * time);
        tmp->co_outline.a = 255 - 255 * 1.0 * i / (60 * time);
        tmp->co_shape.a = 255 - 255 * 1.0 * i / (60 * time);
        tmp->co_val.a = 255 - 255 * 1.0 * i / (60 * time);

        std::vector<node>new_ds; 
        copy_ds_node(new_ds);
        hash_state.push_back(new_ds);
    }
}

void HashScreen::dfs_delete(node *tmp, node *par, int need) {
    if(par != NULL) {
        change_color(par, 0.3f, 1);
    }
    if(tmp == NULL) return;
    change_color(tmp, 0.3f, 2);
    if(par != NULL && tmp->val == need) {
        animation_shift_node_up(tmp, 0.3f);
        par->child = tmp->child;
        delete tmp;
        tmp = NULL;
    }
    else {
        dfs_delete(tmp->child, tmp, need);
    }
}

void HashScreen::delete_hash(int val) {
    if(mod == 0) return;

    reset_color();
    std::vector<node>tmp; 
    copy_ds_node(tmp);
    hash_state.push_back(tmp);

    int block = val % mod;

    dfs_delete(&ds_node[block], NULL, val);

    set_ds_node(block);
    copy_ds_node(tmp);
    hash_state.push_back(tmp);

}

// Hàm để tính chiều dài của đường thẳng
float HashScreen::length(const sf::Vector2f& start, const sf::Vector2f& end) {
    // Tính hiệu giữa tọa độ x và y của hai điểm
    float dx = end.x - start.x;
    float dy = end.y - start.y;
    
    // Tính chiều dài theo định lý Pythagoras
    return std::sqrt(dx * dx + dy * dy);
}

// Hàm để tính góc của đường thẳng (tính bằng radian)
float HashScreen::angle(const sf::Vector2f& start, const sf::Vector2f& end) {
    // Tính hiệu giữa tọa độ x và y của hai điểm
    float dy = end.y - start.y;
    float dx = end.x - start.x;
    
    // Tính góc theo hàm atan2
    return std::atan2(dy, dx) * 180 / 3.14159265f;
}

void HashScreen::draw_edge(node *tmp) {
    //Vẽ cạnh

    sf::Vector2f start(tmp -> pos_x, tmp -> pos_y);
    sf::Vector2f end(tmp -> child -> pos_x, tmp -> child -> pos_y);

    //Cạnh là 1 rectangle
    sf::RectangleShape Line;
    //độ dày
    float thickness = 2.5f;
    //độ dài cạnh
    float len = length(start, end) - 15 - 9;//15:radius, 9::triagle

    // Thiết lập kích thước và góc cho hình chữ nhật
    Line.setSize(sf::Vector2f(len, thickness));
    Line.setFillColor(sf::Color(0, 0, 0, tmp->per_edge));
    Line.setOrigin(0, thickness / 2); // Đặt tâm hình chữ nhật để xoay

    // Đặt vị trí và góc cho hình chữ nhật
    Line.setPosition(start);
    Line.setRotation(angle(start, end)); // Chuyển đổi từ radian sang độ

    mWindow.draw(Line);

    end.y -= 17;

    //vẽ dấu mũi tên
    sf::Vector2f direction = end - start;
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    sf::Vector2f unitDirection = direction / length;
    sf::Vector2f normal(-unitDirection.y, unitDirection.x);

    // Xác định hai đỉnh của mũi tên
    sf::Vector2f arrowPoint1 = end - unitDirection * 7.0f + normal * (7.0f / 2);
    sf::Vector2f arrowPoint2 = end - unitDirection * 7.0f - normal * (7.0f / 2);

    // Tạo mũi tên bằng hình tam giác
    sf::ConvexShape arrowHead;
    arrowHead.setPointCount(3);
    arrowHead.setPoint(0, end);
    arrowHead.setPoint(1, arrowPoint1);
    arrowHead.setPoint(2, arrowPoint2);
    arrowHead.setFillColor(sf::Color(0, 0, 0, tmp->per_edge)); // Màu mũi tên

    // Vẽ mũi tên
    mWindow.draw(arrowHead);
}

void HashScreen::draw_node(node *tmp) {
    if(tmp -> child != NULL) {
        draw_edge(tmp);
        draw_node(tmp -> child);
    }


    // Vẽ node hiện tại
    sf::CircleShape circle(15, 100);
    circle.setOrigin(circle.getRadius(), circle.getRadius());
    circle.setOutlineThickness(3);
    circle.setPosition(tmp -> pos_x, tmp -> pos_y);
    circle.setOutlineColor(tmp -> co_outline);
    circle.setFillColor(tmp -> co_shape);


    //Vẽ giá trị
    sf::Text Value;
    Value.setFont(MyFont.get(FontID::ptsan));
    Value.setString(std::to_string(tmp -> val));
    Value.setCharacterSize(16);
    Value.setFillColor(tmp -> co_val);

    sf::FloatRect textRect = Value.getLocalBounds();
    Value.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top  + textRect.height / 2.0f);
    Value.setPosition(tmp -> pos_x, tmp -> pos_y);


    mWindow.draw(circle);
    mWindow.draw(Value);
}

void HashScreen::draw_hash(std::vector<node>& hash_state) {
    for(int i = 0; i < hash_state.size(); i++) {

        if(hash_state[i].child != NULL) {

            //vẽ cạnh
            draw_edge(&hash_state[i]);


            draw_node(hash_state[i].child);
        }



        // Vẽ node hiện tại
        sf::RectangleShape square(sf::Vector2f(30.0f, 30.0f));
        square.setOutlineThickness(3);
        square.setOrigin(15, 15);
        square.setPosition(hash_state[i].pos_x, hash_state[i].pos_y);
        square.setOutlineColor(hash_state[i].co_outline);
        square.setFillColor(hash_state[i].co_shape);


        //Vẽ giá trị
        sf::Text Value;
        Value.setFont(MyFont.get(FontID::ptsan));
        Value.setString(std::to_string(hash_state[i].val));
        Value.setCharacterSize(16);
        Value.setFillColor(hash_state[i].co_val);

        sf::FloatRect textRect = Value.getLocalBounds();
        Value.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top  + textRect.height / 2.0f);
        Value.setPosition(hash_state[i].pos_x, hash_state[i].pos_y);


        mWindow.draw(square);
        mWindow.draw(Value);
    }
}


void HashScreen::draw_time_bar() {


    //rgb(116,255,188)
    float per = 1.0 * (cur_state + 1) / (int)hash_state.size();

    sf::RectangleShape Line;

    Line.setSize(sf::Vector2f(500.0f * per, 15));
    Line.setFillColor(sf::Color(116, 225, 188));

    // Đặt vị trí và góc cho hình chữ nhật
    Line.setPosition(468, 864);

    mWindow.draw(Line);

}








