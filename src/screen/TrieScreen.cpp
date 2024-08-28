#include <TrieScreen.hpp>
#include <MenuScreen.hpp>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <random>
#include <tinyfiledialogs.h>
#include <fstream>

std::mt19937 rng_5(std::chrono::steady_clock::now().time_since_epoch().count());

int rand_5(int l, int r) {
    return l + rng_5() % (r - l + 1);
}

void TrieScreen::clear_root(node *tmp) {
    if(tmp == NULL) return;
    for(int i = 0; i < 26; i++) {
        clear_root(tmp->c[i]);
    }
    delete tmp;
}

TrieScreen::node *TrieScreen::copy_root(node *tmp) {
    if(tmp == NULL) return NULL;
    node *new_node = new node;
    *new_node = *tmp;
    for(int i = 0; i < 26; i++) {
        new_node->c[i] = copy_root(tmp->c[i]);
        if(new_node->c[i] != NULL) {
            new_node->c[i]->par = new_node;
        }
    }
    return new_node;
}

void TrieScreen::clear_state() {
    while(trie_state.size()) {
        clear_root(trie_state.back());
        trie_state.pop_back();
    }
}

//nhớ reset tại time_dfs trước khi dfs
void TrieScreen::dfs_time(node *tmp, int hi, node *block) {
    if(tmp == NULL) return;
    if(tmp != block) {
        tmp->vt = 0;
        tmp->hi = hi;
    }
    int ok = 0;

    for(int i = 0; i < 26; i++) {
        if(tmp->c[i] != NULL) {
            if(tmp != block) {
                dfs_time(tmp->c[i], hi + 1, block);
                if(tmp->c[i] == block) {
                    if(block->vt == 0) ok = 1;
                }
                else ok = 1;
            }
            else {
                dfs_time(tmp->c[i], hi, block);
                ok = 1;
            }
        }
    }
    if(!ok && tmp != block) tmp->vt = ++time_dfs;
}

int TrieScreen::dfs_minn(node *tmp, node *block) {
    if(tmp == NULL) return 10000000;
    int minn = 10000000;
    if(tmp != block) {
        minn = tmp->nx_pos_x;
    }
    for(int i = 0; i < 26; i++) {
        minn = std::min(minn, dfs_minn(tmp->c[i], block));
    }
    return minn;
}

int TrieScreen::dfs_maxx(node *tmp, node *block) {
    if(tmp == NULL) return -10000000;
    int maxx = -10000000;
    if(tmp != block) {
        maxx = tmp->nx_pos_x;
    }
    for(int i = 0; i < 26; i++) {
        maxx = std::max(maxx, dfs_maxx(tmp->c[i], block));
    }
    return maxx;
}

void TrieScreen::set_last_postition(node *tmp, node *block) {
    if(tmp == NULL) return;

    for(int i = 0; i < 26; i++) {
        set_last_postition(tmp->c[i], block);
    }

    if(tmp == block) return;

    if(tmp->vt == 0) {
        int minn = 10000000;
        int maxx = -10000000;

        for(int i = 0; i < 26; i++) {
            minn = std::min(minn, dfs_minn(tmp->c[i], block));
            maxx = std::max(maxx, dfs_maxx(tmp->c[i], block));
        }
        tmp->cur_pos_x = tmp->pre_pos_x = tmp->nx_pos_x = (maxx + minn) / 2;
    }
    else {
        if(time_dfs % 2 == 0) {
            int mid = time_dfs / 2;
            tmp->cur_pos_x = tmp->pre_pos_x = tmp->nx_pos_x = 695 + (tmp->vt - mid) * 50;
        }
        else {
            int mid = (time_dfs + 1) / 2;
            tmp->cur_pos_x = tmp->pre_pos_x = tmp->nx_pos_x = 720 + (tmp->vt - mid) * 50;
        }
    }
    tmp->cur_pos_y = tmp->pre_pos_y = tmp->nx_pos_y = 80 + (tmp->hi - 1) * 60;

    return;
}

void TrieScreen::set_new_postition(node *tmp, node *block) {
    if(tmp == NULL) return;

    for(int i = 0; i < 26; i++) {
        set_new_postition(tmp->c[i], block);
    }

    if(tmp == block) return;

    if(tmp->vt == 0) {
        int minn = 10000000;
        int maxx = -10000000;

        for(int i = 0; i < 26; i++) {
            minn = std::min(minn, dfs_minn(tmp->c[i], block));
            maxx = std::max(maxx, dfs_maxx(tmp->c[i], block));
        }
        tmp->nx_pos_x = (maxx + minn) / 2;
    }
    else {
        if(time_dfs % 2 == 0) {
            int mid = time_dfs / 2;
            tmp->nx_pos_x = 695 + (tmp->vt - mid) * 50;
        }
        else {
            int mid = (time_dfs + 1) / 2;
            tmp->nx_pos_x = 720 + (tmp->vt - mid) * 50;
        }
    }
    tmp->nx_pos_y = 80 + (tmp->hi - 1) * 60;

    return;
}

void TrieScreen::dfs_shift_tree(node *tmp, float per) {
    if(tmp == NULL) return;
    tmp->cur_pos_x = tmp->pre_pos_x + per * (tmp->nx_pos_x - tmp->pre_pos_x);
    tmp->cur_pos_y = tmp->pre_pos_y + per *(tmp->nx_pos_y - tmp->pre_pos_y);

    for(int i = 0; i < 26; i++) {
        dfs_shift_tree(tmp->c[i], per);
    }
}

void TrieScreen::shift_tree_insert(node *need, float des, float des_len, float time) {
    float cur = need->per_edge;
    float cur_len = need->per_len_edge;
    for(int i = 1; i <= 60 * time; i++) {
        need->co_outline.a = cur + (des - cur) * 1.0 * i / (60 * time);
        need->per_edge = cur + (des - cur) * 1.0 * i / (60 * time);
        need->co_shape.a = cur + (des - cur) * 1.0 * i / (60 * time);
        need->co_val.a = cur + (des - cur) * 1.0 * i / (60 * time);
        need->per_len_edge = cur_len + (des_len - cur_len) * 1.0 * i / (60 * time);

        dfs_shift_tree(root, 1.0 * i / (60 * time));
        trie_state.push_back(copy_root(root));
    }
}

void TrieScreen::dfs_insert(node *tmp, node *par, const std::string &s, int cur) {
    if(par != NULL) {
        change_color_and_edge(par, 0.2, 1, 1);
    }
    change_color_and_edge(tmp, 0.2, 2, 1);
    tmp->cnt++;
    if(cur >= s.size()) {
        if(tmp->ch.back() != '*') tmp->ch += '*';
        tmp->en++;
        change_color_and_edge(tmp, 0.2, 1, 1);
        change_color_and_edge(tmp, 0.2, 0, 0);
        return;
    }
    int ki_tu = s[cur] - 'a';

    if(tmp->c[ki_tu] == NULL) {
        node *new_node = new node;
        tmp->c[ki_tu] = new_node;
        new_node->par = tmp;
        new_node->ch = s[cur];
        time_dfs = 0;
        dfs_time(root, 1, NULL);
        set_new_postition(root, NULL);

        new_node->cur_pos_x = new_node->pre_pos_x = new_node->nx_pos_x;
        new_node->cur_pos_y = new_node->pre_pos_y = new_node->nx_pos_y;
        shift_tree_insert(new_node, 255, 1, 0.3f);

        set_last_postition(root, NULL);
    }

    dfs_insert(tmp->c[ki_tu], tmp, s, cur + 1);
    change_color_and_edge(tmp, 0.2, 1, 1);
    change_color_and_edge(tmp, 0.2, 0, 0);
    return;
}

void TrieScreen::reset_color(node *tmp) {
    if(tmp == NULL) return;
    tmp->co_outline = sf::Color::Black;
    tmp->co_shape = sf::Color::White;
    tmp->co_val = sf::Color::Black;
    tmp->per_progress = 0;

    for(int i = 0; i < 26; i++) {
        reset_color(tmp->c[i]);
    }
}

void TrieScreen::insert_trie(const std::string& s) {
    dfs_insert(root, NULL, s, 0);
}

bool TrieScreen::dfs_search(node *tmp, node *par, const std::string& s, int cur) {
    if(par != NULL) {
        change_color_and_edge(par, 0.3f, 1, 1);
    }
    if(tmp == NULL) return 0;

    change_color_and_edge(tmp, 0.3f, 2, 1);

    if(cur == (int)s.size()) {
        if(tmp->en > 0) {
            change_color_and_edge(tmp, 0.3f, 3, 1);
            return 1;
        }
        else {
            change_color_and_edge(tmp, 0.3f, 0, 0);
            return 0;
        }
    }
    else {
        int kt = s[cur] - 'a';
        if(tmp->c[kt] == NULL) {
            change_color_and_edge(tmp, 0.3f, 0, 0);
            return 0;
        }
        else {
            int ok = dfs_search(tmp->c[kt], tmp, s, cur + 1);
            change_color_and_edge(tmp, 0.3f, 2, 1);

            if(ok == 0) {
                change_color_and_edge(tmp, 0.3f, 0, 0);
                return 0;
            }
            else {
                change_color_and_edge(tmp, 0.3f, 1, 1);
                return 1;
            }
        }
    }
}


void TrieScreen::search_trie(const std::string& s) {
    dfs_search(root, NULL, s, 0);
}

bool TrieScreen::dfs_delete(node *tmp, node *par, const std::string &s, int cur) {
    if(par != NULL) {
        change_color_and_edge(par, 0.3f, 1, 1);
    }
    if(tmp == NULL) return 0;

    change_color_and_edge(tmp, 0.3f, 2, 1);

    if(cur == (int)s.size()) {
        if(tmp->en > 0) {
            tmp->en--;
            tmp->cnt--;
            if(tmp->en == 0 && tmp->ch.back() == '*') tmp->ch.pop_back();

            if(tmp->cnt == 0) {
                time_dfs = 0;
                dfs_time(root, 1, tmp);
                set_new_postition(root, tmp);

                shift_tree_insert(tmp, 0, 0, 0.3f);

                set_last_postition(root, tmp);

                for(int i = 0; i < 26; i++) {
                    if(par->c[i] == tmp) {
                        par->c[i] = NULL;
                    }
                }

                delete tmp;
                tmp = NULL;
            }
            else {
                change_color_and_edge(tmp, 0.3f, 0, 0);
            }
            return 1;
        }
        else {
            change_color_and_edge(tmp, 0.3f, 0, 0);
            return 0;
        }
    }
    else {
        int kt = s[cur] - 'a';
        if(tmp->c[kt] == NULL) {
            change_color_and_edge(tmp, 0.3f, 0, 0);
            return 0;
        }
        else {
            int ok = dfs_delete(tmp->c[kt], tmp, s, cur + 1);
            change_color_and_edge(tmp, 0.3f, 2, 1);
            if(ok == 0) {
                change_color_and_edge(tmp, 0.3f, 0, 0);
                return 0;
            }
            else {
                tmp->cnt--;
                if(tmp->cnt == 0) {
                    time_dfs = 0;
                    dfs_time(root, 1, tmp);
                    set_new_postition(root, tmp);

                    shift_tree_insert(tmp, 0, 0, 0.3f);

                    set_last_postition(root, tmp);

                    for(int i = 0; i < 26; i++) {
                        if(par->c[i] == tmp) {
                            par->c[i] = NULL;
                        }
                    }

                    delete tmp;
                    tmp = NULL;
                }
                else {
                    change_color_and_edge(tmp, 0.3f, 0, 0);
                }
                return 1;
            }
        }
    }
}


void TrieScreen::delete_trie(const std::string &s) {
    dfs_delete(root, NULL, s, 0);
}





void TrieScreen::run() {
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
    ds.push_back(image(TextureID::s_input, TextureID::s_input, 160, 667, 0));//21
    ds.push_back(image(TextureID::go_button, TextureID::go_button_in, 320, 667, 0));//22

    //search
    ds.push_back(image(TextureID::s_input, TextureID::s_input, 160, 717, 0));//23
    ds.push_back(image(TextureID::go_button, TextureID::go_button_in, 320, 717, 0));//24

    //delete
    ds.push_back(image(TextureID::s_input, TextureID::s_input, 160, 767, 0));//25
    ds.push_back(image(TextureID::go_button, TextureID::go_button_in, 320, 767, 0));//26

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


    //nhớ khởi tạo root trước
    root = new node;
    root->cur_pos_x = root->pre_pos_x = root->nx_pos_x = 720;
    root->cur_pos_y = root->pre_pos_y = root->nx_pos_y = 80;
    root->co_outline = sf::Color::Black;
    root->co_shape = sf::Color::White;
    root->co_val = sf::Color::Black;
    root->cnt = 100000;

    trie_state.push_back(copy_root(root));


    while(mWindow.isOpen()) {
        ProcessEvent();
        Update();
        if(Return  == 1) {
            clear_state();
            clear_root(root);
            root = NULL;
            break;
        }
        Render();
    }
}

void TrieScreen::ProcessEvent() {
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
                chu = 26;
            }
            else if((int)event.text.unicode >= 97 && (int)event.text.unicode <= 122) {
                chu = static_cast<char>(event.text.unicode) - 'a';
                back_space = 0;
                so = 10;
            }
            else if((int)event.text.unicode >= 48 && (int)event.text.unicode <= 57) {
                so = static_cast<char>(event.text.unicode) - '0';
                back_space = 0;
                chu = 26;
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


void TrieScreen::Update() {

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
        else if(chu >= 0 && chu < 26) {
            s += char(chu + 'a');
        }
        modify_input(s, 2);
    }
    else if(ds[23].disable == 0 && ds[23].clicked == 1) {
        std::string &s = ds[23].text;
        if(back_space == 1) {
            if(s.size()) s.pop_back();
        }
        else if(chu >= 0 && chu < 26) {
            s += char(chu + 'a');
        }
        modify_input(s, 2);
    }
    else if(ds[25].disable == 0 && ds[25].clicked == 1) {
        std::string &s = ds[25].text;
        if(back_space == 1) {
            if(s.size()) s.pop_back();
        }
        else if(chu >= 0 && chu < 26) {
            s += char(chu + 'a');
        }
        modify_input(s, 2);
    }

    so = 10;
    back_space = 0;
    chu = 26;

    //random n create
    if(ds[18].disable == 0 && ds[18].clicked == 1) {
        int tmp = rand_5(1, 30);
        ds[17].text = std::to_string(tmp);
        ds[18].clicked = 0;
    }

    //go create
    if(ds[19].disable == 0 && ds[19].clicked == 1) {
        clear_state();
        clear_root(root);
        root = new node;
        root->cur_pos_x = root->pre_pos_x = root->nx_pos_x = 720;
        root->cur_pos_y = root->pre_pos_y = root->nx_pos_y = 80;
        root->co_outline = sf::Color::Black;
        root->co_shape = sf::Color::White;
        root->co_val = sf::Color::Black;
        root->cnt = 100000;
        trie_state.push_back(copy_root(root));
        cur_state = 0;

        int n = 0;
        for(int i = 0; i < ds[17].text.size(); i++) {
            n = n * 10 + ds[17].text[i] - '0';
        }

        for(int i = 1; i <= n; i++) {
            int len = rand_5(1, 7);
            std::string s = "";
            for(int j = 1; j <= len; j++) {
                s += char(rand_5(0, 25) + 'a');
            }
            insert_trie(s);
        }

        reset_color(root);
        clear_state();
        cur_state = 0;
        trie_state.push_back(copy_root(root));

        ds[19].clicked = 0;
        is_play = 1;
        ds[13].disable = 0;
        ds[12].disable = 1;
        ds[11].disable = 1;
    }

    //file create
    if(ds[20].disable == 0 && ds[20].clicked == 1) {
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

            clear_state();
            clear_root(root);
            root = new node;
            root->cur_pos_x = root->pre_pos_x = root->nx_pos_x = 720;
            root->cur_pos_y = root->pre_pos_y = root->nx_pos_y = 80;
            root->co_outline = sf::Color::Black;
            root->co_shape = sf::Color::White;
            root->co_val = sf::Color::Black;
            root->cnt = 100000;
            trie_state.push_back(copy_root(root));
            cur_state = 0;


            if(fin.is_open()) {
                std::string s;
                while(getline(fin, s)) {
                    insert_trie(s);
                }
            }

            reset_color(root);
            clear_state();
            cur_state = 0;
            trie_state.push_back(copy_root(root));



            fin.close();
        }

        ds[20].clicked = 0;
        is_play = 1;
        ds[13].disable = 0;
        ds[12].disable = 1;
        ds[11].disable = 1;
    }

    //go insert
    if(ds[22].clicked && ds[22].disable == 0) {
        reset_color(root);
        clear_state();
        trie_state.push_back(copy_root(root));
        cur_state = 0;

        insert_trie(ds[21].text);

        ds[22].clicked = 0;
        is_play = 1;
        ds[13].disable = 0;
        ds[12].disable = 1;
        ds[11].disable = 1;
    }


    //go search 
    if(ds[24].clicked && ds[24].disable == 0) {
        reset_color(root);
        clear_state();
        trie_state.push_back(copy_root(root));
        cur_state = 0;

        search_trie(ds[23].text);

        ds[24].clicked = 0;
        is_play = 1;
        ds[13].disable = 0;
        ds[12].disable = 1;
        ds[11].disable = 1;
    }


    //go delete
    if(ds[26].clicked && ds[26].disable == 0) {
        reset_color(root);
        clear_state();
        trie_state.push_back(copy_root(root));
        cur_state = 0;

        delete_trie(ds[25].text);

        ds[26].clicked = 0;
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
        if(cur_state >= trie_state.size()) cur_state = trie_state.size() - 1;
        is_play = 0;
        ds[12].disable = 0;
        ds[13].disable = 1;
        ds[11].disable = 1;
    }

    //full_go
    if(ds[15].clicked) {
        ds[15].clicked = 0;
        cur_state = trie_state.size() - 1;
        is_play = 0;
        ds[12].disable = 0;
        ds[13].disable = 1;
        ds[11].disable = 1;
    }



}


void TrieScreen::modify_input(std::string &s, int t) {
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
        //s
        while(s.size() > 10) s.pop_back();
    }
}


void TrieScreen::render_image() {
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

void TrieScreen::Render() {
    mWindow.clear();
    render_image();
    cur_state = std::min(cur_state, (int)trie_state.size() - 1);
    if(cur_state >= 0 && cur_state < trie_state.size()) {
        draw_trie(trie_state[cur_state]);
        draw_time_bar();
    }



    mWindow.display();
    cur_state = cur_state + is_play * (int)speed;
    cur_state = std::min(cur_state, (int)trie_state.size() - 1);
    if(cur_state == trie_state.size() - 1) {
        is_play = 0;
        ds[12].disable = ds[13].disable = 1;
        ds[11].disable = 0;
    }
}



void TrieScreen::change_color_and_edge(node *tmp, float time, int t, float des) {
    //den(0,0,0)
    //trang(255, 255, 255)
    //cam(255, 165, 0)
    //xanh(0, 200, 0)
    if(t == 0) {
        sf::Color cur_chu = tmp->co_val;
        sf::Color cur_nen = tmp->co_shape;
        sf::Color cur_vien = tmp->co_outline;
        float cur = tmp->per_progress;
        for(int i = 1; i <= 60 * time; i++) {

            //edge
            tmp->per_progress = cur + 1.0 * (des - cur) * i / (60 * time);

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

            trie_state.push_back(copy_root(root));
        }
        tmp->per_progress = des;
        tmp->co_val = sf::Color(0, 0, 0);
        tmp->co_shape = sf::Color(255, 255, 255);
        tmp->co_outline = sf::Color(0, 0, 0);
        trie_state.push_back(copy_root(root));
    }
    else if(t == 1) {
        sf::Color cur_chu = tmp->co_val;
        sf::Color cur_nen = tmp->co_shape;
        sf::Color cur_vien = tmp->co_outline;
        float cur = tmp->per_progress;
        for(int i = 1; i <= 60 * time; i++) {

            //edge
            tmp->per_progress = cur + 1.0 * (des - cur) * i / (60 * time);

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

            trie_state.push_back(copy_root(root));
        }  
        tmp->per_progress = des;
        tmp->co_val = sf::Color(255, 165, 0);
        tmp->co_shape = sf::Color(255, 255, 255);
        tmp->co_outline = sf::Color(255, 165, 0);
        trie_state.push_back(copy_root(root));   
    }
    else if(t == 2) {
        sf::Color cur_chu = tmp->co_val;
        sf::Color cur_nen = tmp->co_shape;
        sf::Color cur_vien = tmp->co_outline;
        float cur = tmp->per_progress;
        for(int i = 1; i <= 60 * time; i++) {

            //edge
            tmp->per_progress = cur + 1.0 * (des - cur) * i / (60 * time);

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

            trie_state.push_back(copy_root(root));
        }
        tmp->per_progress = des;
        tmp->co_val = sf::Color(255, 255, 255);
        tmp->co_shape = sf::Color(255, 165, 0);
        tmp->co_outline = sf::Color(255, 165, 0);
        trie_state.push_back(copy_root(root));
    }
    else {
        sf::Color cur_chu = tmp->co_val;
        sf::Color cur_nen = tmp->co_shape;
        sf::Color cur_vien = tmp->co_outline;
        float cur = tmp->per_progress;
        for(int i = 1; i <= 60 * time; i++) {

            //edge
            tmp->per_progress = cur + 1.0 * (des - cur) * i / (60 * time);

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

            trie_state.push_back(copy_root(root));
        }
        tmp->per_progress = des;
        tmp->co_val = sf::Color(255, 255, 255);
        tmp->co_shape = sf::Color(106, 220, 127);
        tmp->co_outline = sf::Color(106, 220, 127);
        trie_state.push_back(copy_root(root));
    }
}







// Hàm để tính chiều dài của đường thẳng
float TrieScreen::length(const sf::Vector2f& start, const sf::Vector2f& end) {
    // Tính hiệu giữa tọa độ x và y của hai điểm
    float dx = end.x - start.x;
    float dy = end.y - start.y;
    
    // Tính chiều dài theo định lý Pythagoras
    return std::sqrt(dx * dx + dy * dy);
}

// Hàm để tính góc của đường thẳng (tính bằng radian)
float TrieScreen::angle(const sf::Vector2f& start, const sf::Vector2f& end) {
    // Tính hiệu giữa tọa độ x và y của hai điểm
    float dy = end.y - start.y;
    float dx = end.x - start.x;
    
    // Tính góc theo hàm atan2
    return std::atan2(dy, dx) * 180 / 3.14159265f;
}



void TrieScreen::draw_edge(node *tmp) {


    //Vẽ cạnh

    sf::Vector2f center_1(tmp->par->cur_pos_x, tmp->par->cur_pos_y);
    sf::Vector2f center_2(tmp->cur_pos_x, tmp->cur_pos_y);

    sf::Vector2f direction = center_2 - center_1;
    sf::Vector2f unitDirection = direction / length(center_1, center_2);

    sf::Vector2f start = center_1 + unitDirection * 15.0f;
    sf::Vector2f end = center_2;
    end.y -= 15;




    //Cạnh là 1 rectangle
    sf::RectangleShape Line;
    //độ dày
    float thickness = 3.5f;
    //độ dài cạnh
    float len = length(start, end);

    // Thiết lập kích thước và góc cho hình chữ nhật
    Line.setSize(sf::Vector2f(len * tmp->per_len_edge, thickness));
    Line.setFillColor(sf::Color(0, 0, 0, tmp->per_edge));
    Line.setOrigin(0, thickness / 2); // Đặt tâm hình chữ nhật để xoay

    // Đặt vị trí và góc cho hình chữ nhật
    Line.setPosition(start);
    Line.setRotation(angle(start, end)); // Chuyển đổi từ radian sang độ

    mWindow.draw(Line);

    len = len * tmp->per_progress;
    if(len > 0) {
        // Thiết lập kích thước và góc cho hình chữ nhật
        Line.setSize(sf::Vector2f(len, thickness));
        Line.setFillColor(sf::Color(255, 165, 0, tmp->per_edge));
        Line.setOrigin(0, thickness / 2); // Đặt tâm hình chữ nhật để xoay

        // Đặt vị trí và góc cho hình chữ nhật
        Line.setPosition(start);
        Line.setRotation(angle(start, end)); // Chuyển đổi từ radian sang độ

        mWindow.draw(Line);
    } 
}

void TrieScreen::draw_trie(node *tmp) {
    if(tmp == NULL) return;
    if(tmp->par != NULL) {
        draw_edge(tmp);
    }

    for(int i = 0; i < 26; i++) {
        if(tmp->c[i] != NULL) {
            draw_trie(tmp->c[i]);
        }
    }


    // Vẽ node hiện tại
    sf::CircleShape circle(15, 100);
    circle.setOrigin(circle.getRadius(), circle.getRadius());
    circle.setOutlineThickness(3);
    circle.setPosition(tmp -> cur_pos_x, tmp -> cur_pos_y);
    circle.setOutlineColor(tmp -> co_outline);
    circle.setFillColor(tmp -> co_shape);


    //Vẽ giá trị
    sf::Text Value;
    Value.setFont(MyFont.get(FontID::arial_bold));
    Value.setString(tmp->ch);
    Value.setCharacterSize(16);
    Value.setFillColor(tmp -> co_val);

    sf::FloatRect textRect = Value.getLocalBounds();
    Value.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top  + textRect.height / 2.0f);
    Value.setPosition(tmp -> cur_pos_x, tmp -> cur_pos_y);


    mWindow.draw(circle);
    mWindow.draw(Value);

}





void TrieScreen::draw_time_bar() {


    //rgb(116,255,188)
    float per = 1.0 * (cur_state + 1) / (int)trie_state.size();

    sf::RectangleShape Line;

    Line.setSize(sf::Vector2f(500.0f * per, 15));
    Line.setFillColor(sf::Color(116, 225, 188));

    // Đặt vị trí và góc cho hình chữ nhật
    Line.setPosition(468, 864);

    mWindow.draw(Line);

}








