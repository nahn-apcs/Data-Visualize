#include <AvlScreen.hpp>
#include <MenuScreen.hpp>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <random>
#include <tinyfiledialogs.h>
#include <fstream>

std::mt19937 rng_2(std::chrono::steady_clock::now().time_since_epoch().count());

int rand_2(int l, int r) {
    return l + rng_2() % (r - l + 1);
}

void AVLScreen::clear_root(node *tmp) {
    if(tmp == NULL) return;
    clear_root(tmp->Left);
    clear_root(tmp->Right);
    delete tmp;
}

AVLScreen::node *AVLScreen::copy_root(node *tmp) {
    if(tmp == NULL) return NULL;
    node *new_node = new node;
    *new_node = *tmp;
    new_node->Left = copy_root(tmp->Left);
    new_node->Right = copy_root(tmp->Right);
    return new_node;
}

void AVLScreen::clear_state() {
    while(avl_state.size()) {
        clear_root(avl_state.back());
        avl_state.pop_back();
    }
}

void AVLScreen::dfs_time(node *tmp, int hi, node *block) {
    if(tmp == NULL) return;
    if(tmp == block) dfs_time(tmp->Left, hi, block);
    else dfs_time(tmp->Left, hi + 1, block);
    if(tmp != block) {
        tmp->vt = ++time_dfs;
        tmp->hi = hi;
    }
    if(tmp == block) dfs_time(tmp->Right, hi, block);
    else dfs_time(tmp->Right, hi + 1, block);

    int maxx = 0;
    if(tmp->Left != NULL) maxx = std::max(maxx, tmp->Left->hi_down);
    if(tmp->Right != NULL) maxx = std::max(maxx, tmp->Right->hi_down);

    if(tmp != block) tmp->hi_down = 1 + maxx;
    return;
}

void AVLScreen::set_new_postition(node *tmp) {
    if(tmp == NULL) return;
    if(tmp->par != NULL) {
        tmp->nx_par_pos_x = tmp->par->nx_pos_x;
        tmp->nx_par_pos_y = tmp->par->nx_pos_y;
    }
    int sz = time_dfs;
    if(sz % 2 == 0) {
        int mid = sz / 2;
        if(tmp->vt <= mid) {
            tmp->nx_pos_x = 720 - (mid - tmp->vt) * 34 - 17;
        }
        else {
            tmp->nx_pos_x = 720 + (tmp->vt - (mid + 1)) * 34 + 17;
        }
    }
    else {
        int mid = sz / 2 + 1;
        tmp->nx_pos_x = 720 + (tmp->vt - mid) * 34;
    }
    tmp->nx_pos_y = 120 + (tmp->hi - 1) * 60;

    if(tmp == root) {
        tmp->cur_par_pos_x = tmp->pre_par_pos_x = tmp->nx_par_pos_x = tmp->nx_pos_x;
        tmp->cur_par_pos_y = tmp->pre_par_pos_y = tmp->nx_par_pos_y = tmp->nx_pos_y;
    }

    set_new_postition(tmp->Left);
    set_new_postition(tmp->Right);
}
void AVLScreen::set_last_postition(node *tmp) {
    if(tmp == NULL) return;
    if(tmp->par != NULL) {
        tmp->nx_par_pos_x = tmp->cur_par_pos_x = tmp->pre_par_pos_x = tmp->par->nx_pos_x;
        tmp->nx_par_pos_x = tmp->cur_par_pos_y = tmp->pre_par_pos_y = tmp->par->nx_pos_y;
    }
    int sz = time_dfs;
    if(sz % 2 == 0) {
        int mid = sz / 2;
        if(tmp->vt <= mid) {
            tmp->nx_pos_x = tmp->cur_pos_x = tmp->pre_pos_x = 720 - (mid - tmp->vt) * 34 - 17;
        }
        else {
            tmp->nx_pos_x = tmp->cur_pos_x = tmp->pre_pos_x = 720 + (tmp->vt - (mid + 1)) * 34 + 17;
        }
    }
    else {
        int mid = sz / 2 + 1;
        tmp->nx_pos_x = tmp->cur_pos_x = tmp->pre_pos_x = 720 + (tmp->vt - mid) * 34;
    }
    tmp->nx_pos_y = tmp->cur_pos_y = tmp->pre_pos_y = 120 + (tmp->hi - 1) * 60;

    set_last_postition(tmp->Left);
    set_last_postition(tmp->Right);
}

void AVLScreen::dfs_shift_tree(node *tmp, float per) {

    if(tmp == NULL) return;
    tmp->cur_pos_x = tmp->pre_pos_x + per * (tmp->nx_pos_x - tmp->pre_pos_x);
    tmp->cur_pos_y = tmp->pre_pos_y + per *(tmp->nx_pos_y - tmp->pre_pos_y);

    tmp->cur_par_pos_x = tmp->pre_par_pos_x + per * (tmp->nx_par_pos_x - tmp->pre_par_pos_x);
    tmp->cur_par_pos_y = tmp->pre_par_pos_y + per * (tmp->nx_par_pos_y - tmp->pre_par_pos_y);
    dfs_shift_tree(tmp->Left, per);
    dfs_shift_tree(tmp->Right, per);

}

void AVLScreen::shift_tree_insert(float time) {
    for(int i = 1; i <= 60 * time; i++) {

        dfs_shift_tree(root, 1.0 * i / (60 * time));
        avl_state.push_back(copy_root(root));
    }
}


void AVLScreen::shift_tree(node *need, float des, float des_len, float time) {
    float cur = need->per_edge;
    float cur_len = need->per_len_edge;
    for(int i = 1; i <= 60 * time; i++) {
        need->co_outline.a = cur + (des - cur) * 1.0 * i / (60 * time);
        need->per_edge = cur + (des - cur) * 1.0 * i / (60 * time);
        need->co_shape.a = cur + (des - cur) * 1.0 * i / (60 * time);
        need->co_val.a = cur + (des - cur) * 1.0 * i / (60 * time);
        need->per_len_edge = cur_len + (des_len - cur_len) * 1.0 * i / (60 * time);


        dfs_shift_tree(root, 1.0 * i / (60 * time));
        avl_state.push_back(copy_root(root));
    }
}

int AVLScreen::get_high(node *tmp) {
    if(tmp == NULL) return 0;
    return tmp->hi_down;
}
int AVLScreen::get_balance(node *tmp) {
    if(tmp == NULL) return 0;
    return get_high(tmp->Left) - get_high(tmp->Right);
}

AVLScreen::node* AVLScreen::Right_rotation(node *tmp) {
    node *x = tmp->Left;
    node *y = x->Right;

    std::swap(x->co_outline, tmp->co_outline);
    std::swap(x->co_val, tmp->co_val);
    std::swap(x->co_shape, tmp->co_shape);
    std::swap(x->per_edge, tmp->per_edge);
    std::swap(x->per_len_edge, tmp->per_len_edge);
    std::swap(x->per_progress, tmp->per_progress);
            

    x->par = tmp->par;
    tmp->par = x;
    x->Right = tmp;
    if(y != NULL) y->par = tmp;
    tmp->Left = y;

    if(x->par != NULL) {
        if(x->val <= x->par->val) {
            x->par->Left = x;
        }
        else {
            x->par->Right = x;
        }
    }
    else root = x;

    time_dfs = 0;
    dfs_time(root, 1, NULL);
    set_new_postition(root);
    shift_tree_insert(0.5f);
    set_last_postition(root);

    change_color_and_edge(x, 0.2f, 0, 0);
    return x;
}
AVLScreen::node* AVLScreen::Left_rotation(node *tmp) {
    node *x = tmp->Right;
    node *y = x->Left;

    std::swap(x->co_outline, tmp->co_outline);
    std::swap(x->co_val, tmp->co_val);
    std::swap(x->co_shape, tmp->co_shape);
    std::swap(x->per_edge, tmp->per_edge);
    std::swap(x->per_len_edge, tmp->per_len_edge);
    std::swap(x->per_progress, tmp->per_progress);

            

    x->par = tmp->par;
    tmp->par = x;
    x->Left = tmp;
    if(y != NULL) y->par = tmp;
    tmp->Right = y;

    if(x->par != NULL) {
        if(x->val <= x->par->val) {
            x->par->Left = x;
        }
        else {
            x->par->Right = x;
        }
    }
    else root = x;

    time_dfs = 0;
    dfs_time(root, 1, NULL);
    set_new_postition(root);
    shift_tree_insert(0.5f);
    set_last_postition(root);

    change_color_and_edge(x, 0.2f, 0, 0);
    return x;
}

AVLScreen::node* AVLScreen::dfs_insert(node *tmp, node *par, int val) {
    if(par != NULL) change_color_and_edge(par, 0.2f, 1, 1);
    if(tmp == NULL) {
        node *new_node = new node;
        new_node->val = val;
        if(par != NULL) {
            new_node->par = par;
            if(val <= par->val) {
                par->Left = new_node;
            }
            else par->Right = new_node;
        }

        if(root == NULL) root = new_node;

        time_dfs = 0;
        dfs_time(root, 1, NULL);


        set_new_postition(root);

        new_node->cur_pos_x = new_node->pre_pos_x = new_node->nx_pos_x;
        new_node->cur_pos_y = new_node->pre_pos_y = new_node->nx_pos_y;

        new_node->cur_par_pos_x = new_node->pre_par_pos_x = new_node->nx_par_pos_x;
        new_node->cur_par_pos_y = new_node->pre_par_pos_y = new_node->nx_par_pos_y;



        shift_tree(new_node, 255, 1.0f, 0.5f);


        set_last_postition(root);

        return new_node;
    }
    else {
        change_color_and_edge(tmp, 0.2f, 2, 1);
        if(tmp->val == val);
        else if(tmp->val > val) tmp->Left = dfs_insert(tmp->Left, tmp, val);
        else tmp->Right = dfs_insert(tmp->Right, tmp, val);
        change_color_and_edge(tmp, 0.2f, 2, 1);

        //xoay cay
        int balance = get_balance(tmp);


        //4 case:

        
        if(balance > 1 && val <= tmp->Left->val) {
            return Right_rotation(tmp);
        }
        else if(balance < -1 && val > tmp->Right->val) {
            return Left_rotation(tmp);
        }
        else if(balance > 1 && val > tmp->Left->val) {
            tmp->Left = Left_rotation(tmp->Left); 
            return Right_rotation(tmp);
        }
        else if(balance < -1 && val <= tmp->Right->val) {
            tmp->Right = Right_rotation(tmp->Right);
            return Left_rotation(tmp);
        }
        


        change_color_and_edge(tmp, 0.2f, 0, 0);
        return tmp;
    }
}

void AVLScreen::insert_avl(int val) {
    root = dfs_insert(root, NULL, val);
}

void AVLScreen::reset_color(node *tmp) {
    if(tmp == NULL) return;
    tmp->co_outline = sf::Color::Black;
    tmp->co_shape = sf::Color::White;
    tmp->co_val = sf::Color::Black;
    tmp->per_progress = 0;

    reset_color(tmp->Left);
    reset_color(tmp->Right);
}

void AVLScreen::dfs_find(node *tmp, int val) {
    if(tmp == NULL) return;
    if(tmp->par != NULL) {
        change_color_and_edge(tmp->par, 0.2f, 1, 1);
    }
    change_color_and_edge(tmp, 0.2f, 2, 1);
    if(tmp->val == val) {
        change_color_and_edge(tmp, 0.2f, 3, 1);
        return;
    }
    else if(val < tmp->val) {
        dfs_find(tmp->Left, val);
    }
    else dfs_find(tmp->Right, val);
}

void AVLScreen::search_avl(int val) {
    reset_color(root);
    avl_state.push_back(copy_root(root));

    dfs_find(root, val);
}

void AVLScreen::dfs_minn(node *tmp, node *boss) {
    change_color_and_edge(tmp->par, 0.2f, 1, 1);
    change_color_and_edge(tmp, 0.2f, 2, 1);
    if(tmp->Left == NULL) {
        boss->val = tmp->val;
    }
    else dfs_minn(tmp->Left, boss);
    change_color_and_edge(tmp, 0.2f, 2, 1);
    change_color_and_edge(tmp, 0.2f, 0, 0);
    return;
}

AVLScreen::node *AVLScreen::dfs_delete(node *tmp, int val) {
    if(tmp == NULL) return NULL;
    if(tmp->par != NULL) change_color_and_edge(tmp->par, 0.2f, 1, 1);
    change_color_and_edge(tmp, 0.2f, 2, 1);

    if(val < tmp->val) {
        tmp->Left = dfs_delete(tmp->Left, val);
        change_color_and_edge(tmp, 0.2f, 2, 1);
    }
    else if(val > tmp->val) {
        tmp->Right = dfs_delete(tmp->Right, val);
        change_color_and_edge(tmp, 0.2f, 2, 1);
    }
    else {
        if(tmp->Left == NULL && tmp->Right == NULL) {
            time_dfs = 0;
            dfs_time(root, 1, tmp);
            set_new_postition(root);
            shift_tree(tmp, 0, 0, 0.5f);
            if(tmp->par != NULL) {
                if(tmp->val < tmp->par->val) {
                    tmp->par->Left = NULL;
                }
                else tmp->par->Right = NULL;
            }
            else root = NULL;
            delete tmp;
            tmp = NULL;
            set_last_postition(root);
        }
        else if(tmp->Left == NULL) {
            node *cur = tmp->Right;
            time_dfs = 0;
            dfs_time(root, 1, tmp);
            set_new_postition(root);
            shift_tree(tmp, 0, 0, 0.5f);
            cur->par = tmp->par;
            if(tmp->par != NULL) {
                if(tmp->val < tmp->par->val) {
                    tmp->par->Left = cur;
                }
                else tmp->par->Right = cur;
            }
            else root = cur;
            std::swap(tmp, cur);
            delete cur;
            cur = NULL;
            set_last_postition(root);
        }
        else if(tmp->Right == NULL) {
            node *cur = tmp->Left;
            time_dfs = 0;
            dfs_time(root, 1, tmp);
            set_new_postition(root);
            shift_tree(tmp, 0, 0, 0.5f);
            cur->par = tmp->par;
            if(tmp->par != NULL) {
                if(tmp->val < tmp->par->val) {
                    tmp->par->Left = cur;
                }
                else tmp->par->Right = cur;
            }
            else root = cur;

            std::swap(cur, tmp);

            delete cur;
            cur = NULL;
            set_last_postition(root);
        }
        else {
            dfs_minn(tmp->Right, tmp);
            change_color_and_edge(tmp, 0.2f, 2, 1);
            node *cur = dfs_delete(tmp->Right, tmp->val);
            if(cur != NULL) cur->par = tmp;
            tmp->Right = cur;
        }
    }

    if(tmp == NULL) return tmp;

    //xoay cay
    int balance = get_balance(tmp);


    //4 case:

        
    if(balance > 1 && get_balance(tmp->Left) >= 0) {
        return Right_rotation(tmp);
    }
    else if(balance < -1 && get_balance(tmp->Right) <= 0) {
        return Left_rotation(tmp);
    }
    else if(balance > 1 && get_balance(tmp->Left) < 0) {
        tmp->Left = Left_rotation(tmp->Left); 
        return Right_rotation(tmp);
    }
    else if(balance < -1 && get_balance(tmp->Right) > 0) {
        tmp->Right = Right_rotation(tmp->Right);
        return Left_rotation(tmp);
    }




    change_color_and_edge(tmp, 0.2f, 0, 0);
    return tmp;
}

void AVLScreen::delete_avl(int val) {
    reset_color(root);
    root = dfs_delete(root, val);
}



void AVLScreen::run() {
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
            clear_state();
            clear_root(root);
            root = NULL;
            break;
        }
        Render();
    }
}

void AVLScreen::ProcessEvent() {
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
        }
    }
}


void AVLScreen::Update() {

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
        int tmp = rand_2(1, 30);
        ds[17].text = std::to_string(tmp);
        ds[18].clicked = 0;
    }

    //go create
    if(ds[19].disable == 0 && ds[19].clicked == 1) {
        clear_state();
        clear_root(root);
        avl_state.push_back(copy_root(root));
        cur_state = 0;
        root = NULL;
        int n = 0;
        for(int i = 0; i < ds[17].text.size(); i++) {
            n = n * 10 + ds[17].text[i] - '0';
        }

        for(int i = 1; i <= n; i++) {
            int val = rand_2(1, 99);
            insert_avl(val);
        }
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

            std::vector<int>a;
            int m = 0;

            if(fin.is_open()) {
                std::string s;
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

            clear_state();
            clear_root(root);
            avl_state.push_back(copy_root(root));
            cur_state = 0;
            root = NULL;

            for(int i = 0; i < a.size(); i++) {
                insert_avl(a[i]);
            }


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
        clear_state();
        avl_state.push_back(copy_root(root));
        cur_state = 0;
        int tmp = 0;
        for(int i = 0; i < ds[21].text.size(); i++) {
            tmp = tmp * 10 + ds[21].text[i] - '0';
        }
        insert_avl(tmp);
        ds[22].clicked = 0;
        is_play = 1;
        ds[13].disable = 0;
        ds[12].disable = 1;
        ds[11].disable = 1;
    }


    //go search 
    if(ds[24].clicked && ds[24].disable == 0) {
        clear_state();
        avl_state.push_back(copy_root(root));
        cur_state = 0;
        int tmp = 0;
        for(int i = 0; i < ds[23].text.size(); i++) {
            tmp = tmp * 10 + ds[23].text[i] - '0';
        }
        search_avl(tmp);
        ds[24].clicked = 0;
        is_play = 1;
        ds[13].disable = 0;
        ds[12].disable = 1;
        ds[11].disable = 1;
    }


    //go delete
    if(ds[26].clicked && ds[26].disable == 0) {
        clear_state();
        avl_state.push_back(copy_root(root));
        cur_state = 0;
        int tmp = 0;
        for(int i = 0; i < ds[25].text.size(); i++) {
            tmp = tmp * 10 + ds[25].text[i] - '0';
        }
        delete_avl(tmp);
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
        if(cur_state >= avl_state.size()) cur_state = avl_state.size() - 1;
        is_play = 0;
        ds[12].disable = 0;
        ds[13].disable = 1;
        ds[11].disable = 1;
    }

    //full_go
    if(ds[15].clicked) {
        ds[15].clicked = 0;
        cur_state = avl_state.size() - 1;
        is_play = 0;
        ds[12].disable = 0;
        ds[13].disable = 1;
        ds[11].disable = 1;
    }



}


void AVLScreen::modify_input(std::string &s, int t) {
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


void AVLScreen::render_image() {
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

void AVLScreen::Render() {
    mWindow.clear();
    render_image();
    cur_state = std::min(cur_state, (int)avl_state.size() - 1);
    if(cur_state >= 0 && cur_state < avl_state.size()) {
        draw_avl(avl_state[cur_state]);
        draw_time_bar();
    }



    mWindow.display();
    cur_state = cur_state + is_play * (int)speed;
    cur_state = std::min(cur_state, (int)avl_state.size() - 1);
    if(cur_state == avl_state.size() - 1) {
        is_play = 0;
        ds[12].disable = ds[13].disable = 1;
        ds[11].disable = 0;
    }
}



void AVLScreen::change_color_and_edge(node *tmp, float time, int t, float des) {
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

            node *new_ds = copy_root(root);
            avl_state.push_back(new_ds);
        }
        tmp->per_progress = des;
        tmp->co_val = sf::Color(0, 0, 0);
        tmp->co_shape = sf::Color(255, 255, 255);
        tmp->co_outline = sf::Color(0, 0, 0);
        node *new_ds = copy_root(root);
        avl_state.push_back(new_ds);
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

            node *new_ds = copy_root(root);
            avl_state.push_back(new_ds);
        }  
        tmp->per_progress = des;
        tmp->co_val = sf::Color(255, 165, 0);
        tmp->co_shape = sf::Color(255, 255, 255);
        tmp->co_outline = sf::Color(255, 165, 0);
        node *new_ds = copy_root(root);
        avl_state.push_back(new_ds);    
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

            node *new_ds = copy_root(root);
            avl_state.push_back(new_ds);
        }
        tmp->per_progress = des;
        tmp->co_val = sf::Color(255, 255, 255);
        tmp->co_shape = sf::Color(255, 165, 0);
        tmp->co_outline = sf::Color(255, 165, 0);
        node *new_ds = copy_root(root);
        avl_state.push_back(new_ds);
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
            tmp->co_shape.r = cur_nen.r + 1.0 * (34 - cur_nen.r) * i / (60 * time);
            tmp->co_shape.g = cur_nen.g + 1.0 * (139 - cur_nen.g) * i / (60 * time);
            tmp->co_shape.b = cur_nen.b + 1.0 * (34 - cur_nen.b) * i / (60 * time);


            //vien
            tmp->co_outline.r = cur_vien.r + 1.0 * (34 - cur_vien.r) * i / (60 * time);
            tmp->co_outline.g = cur_vien.g + 1.0 * (139 - cur_vien.g) * i / (60 * time);
            tmp->co_outline.b = cur_vien.b + 1.0 * (34 - cur_vien.b) * i / (60 * time);

            node *new_ds = copy_root(root);
            avl_state.push_back(new_ds);
        }
        tmp->per_progress = des;
        tmp->co_val = sf::Color(255, 255, 255);
        tmp->co_shape = sf::Color(34, 139, 34);
        tmp->co_outline = sf::Color(34, 139, 34);
        node *new_ds = copy_root(root);
        avl_state.push_back(new_ds);
    }
}





// Hàm để tính chiều dài của đường thẳng
float AVLScreen::length(const sf::Vector2f& start, const sf::Vector2f& end) {
    // Tính hiệu giữa tọa độ x và y của hai điểm
    float dx = end.x - start.x;
    float dy = end.y - start.y;
    
    // Tính chiều dài theo định lý Pythagoras
    return std::sqrt(dx * dx + dy * dy);
}

// Hàm để tính góc của đường thẳng (tính bằng radian)
float AVLScreen::angle(const sf::Vector2f& start, const sf::Vector2f& end) {
    // Tính hiệu giữa tọa độ x và y của hai điểm
    float dy = end.y - start.y;
    float dx = end.x - start.x;
    
    // Tính góc theo hàm atan2
    return std::atan2(dy, dx) * 180 / 3.14159265f;
}

void AVLScreen::draw_edge(node *tmp) {


    //Vẽ cạnh

    sf::Vector2f center_1(tmp -> cur_par_pos_x, tmp -> cur_par_pos_y);
    sf::Vector2f center_2(tmp -> cur_pos_x, tmp -> cur_pos_y);

    sf::Vector2f direction = center_2 - center_1;
    sf::Vector2f unitDirection = direction / length(center_1, center_2);

    sf::Vector2f start = center_1 + unitDirection * 15.0f;
    sf::Vector2f end = center_2 - unitDirection * 15.0f;



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

void AVLScreen::draw_avl(node *tmp) {
    if(tmp == NULL) return;
    if(tmp->par != NULL) {
        draw_edge(tmp);
    }
    if(tmp->Left != NULL) {
        draw_avl(tmp->Left);
    }
    if(tmp->Right != NULL) {
        draw_avl(tmp->Right);
    }

    // Vẽ node hiện tại
    sf::CircleShape circle(15, 100);
    circle.setOutlineThickness(3);
    circle.setPosition(tmp -> cur_pos_x - 15, tmp -> cur_pos_y - 15);
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
    Value.setPosition(tmp -> cur_pos_x, tmp -> cur_pos_y);


    mWindow.draw(circle);
    mWindow.draw(Value);

}



void AVLScreen::draw_time_bar() {


    //rgb(116,255,188)
    float per = 1.0 * (cur_state + 1) / (int)avl_state.size();

    sf::RectangleShape Line;

    Line.setSize(sf::Vector2f(500.0f * per, 15));
    Line.setFillColor(sf::Color(116, 225, 188));

    // Đặt vị trí và góc cho hình chữ nhật
    Line.setPosition(468, 864);

    mWindow.draw(Line);

}








