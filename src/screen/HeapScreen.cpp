#include <HeapScreen.hpp>
#include <MenuScreen.hpp>
#include <SFML/Graphics.hpp>
#include <math.h>

void HeapScreen::run() {
    //Add_image
    ds.push_back(image(TextureID::app_bg, TextureID::app_bg, 0, 0, 0));
    ds.push_back(image(TextureID::back_menu_button, TextureID::back_menu_button, 10, 10, 0));
    ds.push_back(image(TextureID::create_button, TextureID::create_button_in, 0, 573, 0));
    ds.push_back(image(TextureID::insert_button, TextureID::insert_button_in, 0, 623, 0));
    ds.push_back(image(TextureID::extractmax_button, TextureID::extractmax_button_in, 0, 673, 0));
    ds.push_back(image(TextureID::update_button, TextureID::update_button_in, 0, 723, 0));
    ds.push_back(image(TextureID::delete_button, TextureID::delete_button_in, 0, 773, 0));
    ds.push_back(image(TextureID::tool_bar, TextureID::tool_bar, 0, 842, 0));
    ds.push_back(image(TextureID::time_bar, TextureID::time_bar, 468, 864, 0));
    ds.push_back(image(TextureID::speed_bar, TextureID::speed_bar, 1182, 864, 0));
    ds.push_back(image(TextureID::go_begin_disable, TextureID::go_begin_white, 190, 852, 0));
    ds.push_back(image(TextureID::step_backward_disable, TextureID::step_backward_white, 243, 852, 0));
    ds.push_back(image(TextureID::return_button, TextureID::return_button, 302, 854, 0));
    ds.push_back(image(TextureID::play_button, TextureID::play_button, 298, 852, 1));
    ds.push_back(image(TextureID::pause_button, TextureID::pause_button, 298, 852, 1));
    ds.push_back(image(TextureID::step_forward_disable, TextureID::step_forward_white, 353, 852, 0));
    ds.push_back(image(TextureID::go_end_disable, TextureID::go_end_white, 397, 852, 0));
    ds.push_back(image(TextureID::cuc, TextureID::cuc, 1213, 864, 0)); //cuc speed


    reset_heap();

    insert_heap(20);
    insert_heap(30);
    insert_heap(40);

    insert_heap(40);
    insert_heap(40);
    insert_heap(99);
    insert_heap(99);
    insert_heap(40);
    insert_heap(40);
    insert_heap(99);
    insert_heap(99);

    insert_heap(40);
    insert_heap(40);
    insert_heap(99);
    insert_heap(99);
    insert_heap(40);
    insert_heap(40);
    insert_heap(99);
    insert_heap(99);
    insert_heap(40);
    insert_heap(40);
    insert_heap(99);
    insert_heap(99);
    insert_heap(40);
    insert_heap(40);
    insert_heap(99);
    insert_heap(99);    

    cur_stage = 0;

    while(mWindow.isOpen()) {
        ProcessEvent();
        if(Return  == 1) break;
        Update();
        Render();
    }
}

void HeapScreen::ProcessEvent() {
    sf::Event event;
    while(mWindow.pollEvent(event)) {
        if (event.type == sf::Event::Closed) mWindow.close();
        else if(event.type == sf::Event::MouseButtonPressed) 
        {
            if(event.mouseButton.button == sf::Mouse::Left) 
            {
                sf::Vector2i mousePos = sf::Mouse::getPosition(mWindow);
                sf::Sprite sprite;
                sprite.setPosition(ds[1].pos_x, ds[1].pos_y);
                sprite.setTexture(MyTexture.get(ds[1].a));
                if(sprite.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                    Return = 1;
                }
            }
        }
    }
}

void HeapScreen::Update() {
    for(int i = 2; i < ds.size(); i++) {
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
    }
}

void HeapScreen::Render() {
    mWindow.clear();
    render_image();
    cur_stage = std::min(cur_stage, (int)tree_state.size() - 1);
    draw_heap(tree_state[cur_stage]);



    mWindow.display();
    cur_stage = cur_stage + is_play;
}

void HeapScreen::reset_heap() {
    ds_node.clear();
    tree_state.clear();
}

void HeapScreen::dfs_inoder(int u) {
    int v = 2 * u;
    if(v <= ds_node.size()) {
        dfs_inoder(v);
    }
    ds_node[u - 1].vt = ++time_vi;
    v = 2 * u + 1;
    if(v <= ds_node.size()) {
        dfs_inoder(v);
    }
}

void HeapScreen::insert_heap(int val) {
    node tmp(0, 0, val);
    ds_node.push_back(tmp);


    time_vi = 0;

    if(ds_node.size()) {
        dfs_inoder(1);


        animation_shift_tree_new_node(0.5f);

        up_heapify(ds_node.size());
    }
}

void HeapScreen::animation_shift_tree_new_node(float time) {

    for(int i = 0; i < ds.size(); i++) ds_node[i].now = ds_node[i].next = 0;
    if(ds_node.size()) ds_node.back().now = 1;

    //tree_state.clear();
    tree_state.push_back(ds_node);

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
    }


    set_position();
}

void HeapScreen::up_heapify(int u) {
    int cha = u / 2;
    if(cha < 1) return;
    if(ds_node[cha - 1].val < ds_node[u - 1].val) {


        animation_swap_node(cha, u, 0.5f);
        std::swap(ds_node[cha - 1].val, ds_node[u - 1].val);

        tree_state.push_back(ds_node);
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

    for(int t = 1; t <= 60 * time; t++) {
        std::vector<node>tmp;

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
}

void HeapScreen::set_position() {
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

// Hàm để tính chiều dài của đường thẳng
float length(const sf::Vector2f& start, const sf::Vector2f& end) {
    // Tính hiệu giữa tọa độ x và y của hai điểm
    float dx = end.x - start.x;
    float dy = end.y - start.y;
    
    // Tính chiều dài theo định lý Pythagoras
    return std::sqrt(dx * dx + dy * dy);
}

// Hàm để tính góc của đường thẳng (tính bằng radian)
float angle(const sf::Vector2f& start, const sf::Vector2f& end) {
    // Tính hiệu giữa tọa độ x và y của hai điểm
    float dy = end.y - start.y;
    float dx = end.x - start.x;
    
    // Tính góc theo hàm atan2
    return std::atan2(dy, dx);
}

void HeapScreen::draw_heap(std::vector<node>& tmp) {
    draw_node(1, tmp);
}


void HeapScreen::draw_node(int u, std::vector<node> &ds_node) {
    if(u > ds_node.size()) return;

    // Vẽ cạnh nối đến con trái
    int v = 2 * u;
    if(v <= ds_node.size()) {

        sf::Vector2f start(ds_node[u - 1].pos_x, ds_node[u - 1].pos_y);
        sf::Vector2f end(ds_node[v - 1].pos_x, ds_node[v - 1].pos_y);
    
        // Tạo hình chữ nhật để làm đường thẳng dày hơn
        sf::RectangleShape Line;
        float thickness = 3.5f; // Độ dày của đường thẳng
        float lineLength = length(start, end);

        // Thiết lập kích thước và góc cho hình chữ nhật
        Line.setSize(sf::Vector2f(lineLength, thickness));
        Line.setFillColor(sf::Color(0, 0, 0, ds_node[u - 1].per_edge_l));
        Line.setOrigin(thickness / 2, 0); // Đặt tâm hình chữ nhật để xoay

        // Đặt vị trí và góc cho hình chữ nhật
        Line.setPosition(start);
        Line.setRotation(angle(start, end) * 180 / 3.14159265f); // Chuyển đổi từ radian sang độ

        mWindow.draw(Line);

        draw_node(v, ds_node);
    }

    // Vẽ cạnh nối đến con phải
    v = 2 * u + 1;
    if(v <= ds_node.size()) {

        sf::Vector2f start(ds_node[u - 1].pos_x, ds_node[u - 1].pos_y);
        sf::Vector2f end(ds_node[v - 1].pos_x, ds_node[v - 1].pos_y);
    
        // Tạo hình chữ nhật để làm đường thẳng dày hơn
        sf::RectangleShape Line;
        float thickness = 3.5f; // Độ dày của đường thẳng
        float lineLength = length(start, end);

        // Thiết lập kích thước và góc cho hình chữ nhật
        Line.setSize(sf::Vector2f(lineLength, thickness));
        Line.setFillColor(sf::Color(0, 0, 0, ds_node[u - 1].per_edge_r));
        Line.setOrigin(thickness / 2, 0); // Đặt tâm hình chữ nhật để xoay

        // Đặt vị trí và góc cho hình chữ nhật
        Line.setPosition(start);
        Line.setRotation(angle(start, end) * 180 / 3.14159265f); // Chuyển đổi từ radian sang độ

        mWindow.draw(Line);

        draw_node(v, ds_node);
    }



    // Vẽ node hiện tại
    sf::CircleShape circle(15, 100);
    if(ds_node[u - 1].now == 0) {
        circle.setFillColor(sf::Color(255, 255, 255, ds_node[u - 1].per_node));
        circle.setOutlineColor(sf::Color(0, 0, 0, ds_node[u - 1].per_node));
    }
    else {
        circle.setFillColor(sf::Color(255, 165, 0, ds_node[u - 1].per_node));
        circle.setOutlineColor(sf::Color(255, 165, 0, ds_node[u - 1].per_node));
    }
    circle.setOutlineThickness(3);
    circle.setPosition(ds_node[u - 1].pos_x - 15, ds_node[u - 1].pos_y - 15);
    mWindow.draw(circle);




    //Vẽ giá trị
    sf::Text Value;
    Value.setFont(MyFont.get(FontID::ptsan));
    Value.setString(std::to_string(ds_node[u - 1].val));
    Value.setCharacterSize(16);
    if(ds_node[u - 1].now == 0) Value.setFillColor(sf::Color(0, 0, 0, ds_node[u - 1].per_node));
    else Value.setFillColor(sf::Color(255, 255, 255, ds_node[u - 1].per_node));

    sf::FloatRect textRect = Value.getLocalBounds();
    Value.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top  + textRect.height / 2.0f);
    Value.setPosition(ds_node[u - 1].pos_x, ds_node[u - 1].pos_y);

    mWindow.draw(Value);

    //Vẽ ID
    sf::Text ID;
    ID.setFont(MyFont.get(FontID::ptsan));
    ID.setString(std::to_string(u));
    ID.setCharacterSize(16);
    ID.setFillColor(sf::Color(255, 0, 0, ds_node[u - 1].per_node));

    textRect = ID.getLocalBounds();
    ID.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top  + textRect.height / 2.0f);
    ID.setPosition(ds_node[u - 1].pos_x, ds_node[u - 1].pos_y + 30);

    mWindow.draw(ID);

    if(ds_node[u - 1].next) {    
        circle.setFillColor(sf::Color(255, 165, 0, 100));
        circle.setOutlineColor(sf::Color(255, 165, 0, 100));
        circle.setOutlineThickness(3);
        circle.setPosition(ds_node[u - 1].pos_x - 15, ds_node[u - 1].pos_y - 15);
        mWindow.draw(circle);
    }

    return;

}