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

void AVLScreen::dfs_time(node *tmp) {
    if(tmp == NULL) return;
    dfs_time(tmp->Left);
    tmp->vt = ++time_dfs;
    dfs_time(tmp->Right);
    return;
}

void AVLScreen::dfs_insert(node *tmp, node *par, int val) {
    if(par != NULL) change_color_and_edge(par, 0.2f, 1, 1);
    if(tmp == NULL) {
        node *new_node = new node;
        new_node->val = val;
        if(par != NULL) {
            new_node->par = par;
            if(par->val >= val) {
                par->Left = new_node;
            }
            else par->Right = new_node;
        }
    }
    else {
        change_color_and_edge(tmp, 0.2f, 2, 1);
        if(tmp->val >= val) dfs_insert(tmp->Left, tmp, val);
        else dfs_insert(tmp->Right, tmp, val);
        change_color_and_edge(tmp, 0.2f, 0, 0);
    }
}

void AVLScreen::insert_avl(int val) {
    dfs_insert(root, NULL, val);
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






    while(mWindow.isOpen()) {
        ProcessEvent();
        Update();
        if(Return  == 1) {
            //new_ds_node(0);
            //nhớ xoá dữ liệu trước khi break
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



}


void AVLScreen::modify_input(std::string &s, int t) {
    if(t == 1) {
        //m
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




    mWindow.display();
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
    Line.setSize(sf::Vector2f(len, thickness));
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


/*
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
*/







