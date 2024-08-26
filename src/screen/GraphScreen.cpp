#include <GraphScreen.hpp>
#include <MenuScreen.hpp>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <random>
#include <tinyfiledialogs.h>
#include <fstream>
#include <set>

std::mt19937 rng_6(std::chrono::steady_clock::now().time_since_epoch().count());

int rand_6(int l, int r) {
    return l + rng_6() % (r - l + 1);
}

void GraphScreen::clear_node(std::vector<node*> tmp) {
    while(tmp.size()) {
        delete tmp.back();
        tmp.pop_back();
    }
    return;
}
void GraphScreen::clear_edge(std::vector<edge*> tmp) {
    while(tmp.size()) {
        delete tmp.back();
        tmp.pop_back();
    }
    return;
}

void GraphScreen::clear_state() {
    while(node_state.size()) {
        clear_node(node_state.back());
        node_state.pop_back();
    }
    while(edge_state.size()) {
        clear_edge(edge_state.back());
        edge_state.pop_back();
    }
    return;
}

void GraphScreen::clear_all() {
    clear_node(ds_node);
    clear_edge(ds_edge);
    clear_state();
    num_node = 0;
    num_edge = 0;
    return;
}

void GraphScreen::copy_edge(std::vector<edge*>tmp) {
    for(int i = 0; i < ds_edge.size(); i++) {
        edge *new_edge = new edge;
        *new_edge = *ds_edge[i];
        tmp.push_back(new_edge);
    }
}

void GraphScreen::copy_node(std::vector<node*>tmp) {
    for(int i = 0; i < ds_node.size(); i++) {
        node *new_node = new node;
        *new_node = *ds_node[i];
        tmp.push_back(new_node);
    }
}

void GraphScreen::create_random(int n, int m) {
    if(m > n * (n - 1) / 2) return;
    clear_all();
    num_node = n;
    num_edge = m;
    for(int i = 1; i <= n; i++) {
        int pos_x = rand_6(0, 1440);
        int pos_y = rand_6(0, 900);

        node *new_node = new node(pos_x, pos_y, i);

        ds_node.push_back(new_node);
    }

    std::set<std::pair<int,int>> st;

    for(int i = 1; i <= m; i++) {
        while(1) {
            int u = rand_6(1, n);
            int v = rand_6(1, n);
            if(u != v && st.find(std::make_pair(std::min(u, v), std::max(u, v))) == st.end()) {
                st.insert(std::make_pair(std::min(u, v), std::max(u, v)));
                edge *new_edge = new edge(ds_node[u - 1], ds_node[v - 1], rand_6(1, 99));
                ds_edge.push_back(new_edge);
                break;
            }
        }
    }
}

sf::Vector2f GraphScreen::unit_vector(sf::Vector2f a, sf::Vector2f b) {
    sf::Vector2f direction = b - a;
    sf::Vector2f unitDirection = direction / length(a, b);
    return unitDirection;
}

void GraphScreen::set_position() {
    if(fixed) return;

    std::vector<sf::Vector2f> sum_F(num_node, sf::Vector2f(0, 0));

    float c_rep = 14400;
    float c_spring = 2.4;


    for(int i = 0; i < num_node; i++) {
        for(int j = i + 1; j < num_node; j++) {
            sf::Vector2f u(ds_node[i]->pos_x, ds_node[i]->pos_y);
            sf::Vector2f v(ds_node[j]->pos_x, ds_node[j]->pos_y);
            float len = length(u, v);

            sf::Vector2f F_rep =  unit_vector(u, v) * c_rep / (len * len);

            sum_F[i] -= F_rep;
            sum_F[j] += F_rep;
        }
    }


    for(int i = 0; i < num_edge; i++) {
        sf::Vector2f u(ds_node[ds_edge[i]->u->val - 1]->pos_x, ds_node[ds_edge[i]->u->val - 1]->pos_y);
        sf::Vector2f v(ds_node[ds_edge[i]->v->val - 1]->pos_x, ds_node[ds_edge[i]->v->val - 1]->pos_y);
        float len = length(u, v);

        sf::Vector2f F_rep =  unit_vector(u, v) * c_rep / (len * len);


        sf::Vector2f F_attr = c_spring * log(len / 120) * unit_vector(u, v);
        sum_F[ds_edge[i]->u->val - 1] += F_attr;
        sum_F[ds_edge[i]->v->val - 1] -= F_attr;
    }


    //lực kéo về trung tâm

    for(int i = 0; i < num_node; i++) {
        sf::Vector2f center(800, 400);

        float c_rep = 0.01;
        sf::Vector2f direct = center;
        direct.x -= ds_node[i]->pos_x;
        direct.y -= ds_node[i]->pos_y;

        direct *= c_rep;

        sum_F[i] += direct;

    }


    //giữa các cạnh

    for(int i = 0; i < num_edge; i++) {
        for(int j = i + 1; j < num_edge; j++) {
            sf::Vector2f u((ds_node[ds_edge[i]->u->val - 1]->pos_x + ds_node[ds_edge[i]->v->val - 1]->pos_x)/ 2, (ds_node[ds_edge[i]->u->val - 1]->pos_y + ds_node[ds_edge[i]->v->val - 1]->pos_y)/ 2);
            sf::Vector2f v((ds_node[ds_edge[j]->u->val - 1]->pos_x + ds_node[ds_edge[j]->v->val - 1]->pos_x)/ 2, (ds_node[ds_edge[j]->u->val - 1]->pos_y + ds_node[ds_edge[j]->v->val - 1]->pos_y)/ 2);

            float len = length(u, v);

            sf::Vector2f F_rep = 2000 / len / len * unit_vector(u, v);

            sum_F[ds_edge[i]->u->val - 1] -= F_rep;
            sum_F[ds_edge[i]->v->val - 1] -= F_rep;
            sum_F[ds_edge[j]->u->val - 1] += F_rep;
            sum_F[ds_edge[j]->v->val - 1] += F_rep;
        }
    }


    //giữa cạnh và node
    for(int i = 0; i < num_edge; i++) {
        for(int j = 0; j < num_node; j++) {
            if(ds_edge[i]->u->val - 1 != j && ds_edge[i]->v->val - 1 != j) {
                sf::Vector2f u((ds_node[ds_edge[i]->u->val - 1]->pos_x + ds_node[ds_edge[i]->v->val - 1]->pos_x)/ 2, (ds_node[ds_edge[i]->u->val - 1]->pos_y + ds_node[ds_edge[i]->v->val - 1]->pos_y)/ 2);
                sf::Vector2f v(ds_node[j]->pos_x, ds_node[j]->pos_y);

                float len = length(u, v);

                sf::Vector2f F_rep = 2000 / len / len * unit_vector(u, v);

                sum_F[ds_edge[i]->u->val - 1] -= F_rep / 2.0f;
                sum_F[ds_edge[i]->v->val - 1] -= F_rep / 2.0f;

                sum_F[j] += F_rep;
            }
        }
    }





    float maxx = 0.0f;


    for(int i = 0; i < num_node; i++) {
        maxx = std::max(maxx, std::sqrt(sum_F[i].x * sum_F[i].x + sum_F[i].y * sum_F[i].y));
    }

    if(maxx < 0.1f) return;

    for(int i = 0; i < num_node; i++) {
        if(ds_node[i] != hold_node) {
            ds_node[i]->pos_x += 0.99 * sum_F[i].x;
            ds_node[i]->pos_y += 0.99 * sum_F[i].y;
        }
    }

}



void GraphScreen::run() {
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


    create_random(10, 15);


    while(mWindow.isOpen()) {
        ProcessEvent();
        Update();
        if(Return  == 1) {
            clear_all();
            break;
        }
        Render();
    }
}

void GraphScreen::ProcessEvent() {
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
                for(int i = 0; i < num_node; i++) {
                    sf::CircleShape circle(15, 100);
                    circle.setOrigin(circle.getRadius(), circle.getRadius());
                    circle.setPosition(ds_node[i]->pos_x, ds_node[i]->pos_y);


                    sf::Vector2i mousePos = sf::Mouse::getPosition(mWindow);
                    if(circle.getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
                        hold_node = ds_node[i];
                    }
                }
            }
        }
        if(event.type == sf::Event::MouseButtonReleased) 
        {
            hold_node = NULL;
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


void GraphScreen::Update() {

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

    //change position of node by mouse
    if(hold_node != NULL) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(mWindow);
        hold_node->pos_x = mousePos.x;
        hold_node->pos_y = mousePos.y;
    }

    set_position();


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
        //modify_input(s, 1);
    }
    else if(ds[21].disable == 0 && ds[21].clicked == 1) {
        std::string &s = ds[21].text;
        if(back_space == 1) {
            if(s.size()) s.pop_back();
        }
        else if(so >= 0 && so < 10) {
            s += char(so + '0');
        }
        //modify_input(s, 2);
    }
    else if(ds[23].disable == 0 && ds[23].clicked == 1) {
        std::string &s = ds[23].text;
        if(back_space == 1) {
            if(s.size()) s.pop_back();
        }
        else if(so >= 0 && so < 10) {
            s += char(so + '0');
        }
        //modify_input(s, 2);
    }
    else if(ds[25].disable == 0 && ds[25].clicked == 1) {
        std::string &s = ds[25].text;
        if(back_space == 1) {
            if(s.size()) s.pop_back();
        }
        else if(so >= 0 && so < 10) {
            s += char(so + '0');
        }
        //modify_input(s, 2);
    }

    so = 10;
    back_space = 0;

    

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
        if(cur_state >= node_state.size()) cur_state = node_state.size() - 1;
        is_play = 0;
        ds[12].disable = 0;
        ds[13].disable = 1;
        ds[11].disable = 1;
    }

    //full_go
    if(ds[15].clicked) {
        ds[15].clicked = 0;
        cur_state = node_state.size() - 1;
        is_play = 0;
        ds[12].disable = 0;
        ds[13].disable = 1;
        ds[11].disable = 1;
    }



}



void GraphScreen::render_image() {
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

void GraphScreen::Render() {
    mWindow.clear();
    render_image();

    draw_graph(ds_node, ds_edge);



    mWindow.display();
}






// Hàm để tính chiều dài của đường thẳng
float GraphScreen::length(const sf::Vector2f& start, const sf::Vector2f& end) {
    // Tính hiệu giữa tọa độ x và y của hai điểm
    float dx = end.x - start.x;
    float dy = end.y - start.y;
    
    // Tính chiều dài theo định lý Pythagoras
    return std::sqrt(dx * dx + dy * dy);
}

// Hàm để tính góc của đường thẳng (tính bằng radian)
float GraphScreen::angle(const sf::Vector2f& start, const sf::Vector2f& end) {
    // Tính hiệu giữa tọa độ x và y của hai điểm
    float dy = end.y - start.y;
    float dx = end.x - start.x;
    
    // Tính góc theo hàm atan2
    return std::atan2(dy, dx) * 180 / 3.14159265f;
}

void GraphScreen::draw_edge(edge *tmp) {



    //Vẽ cạnh

    sf::Vector2f center_1(tmp->u->pos_x, tmp->u->pos_y);
    sf::Vector2f center_2(tmp->v->pos_x, tmp->v->pos_y);

    if(center_1.y > center_2.y) std::swap(center_1, center_2);

    sf::Vector2f direction = center_2 - center_1;
    sf::Vector2f unitDirection = direction / length(center_1, center_2);

    sf::Vector2f start = center_1 + unitDirection * 15.0f;
    sf::Vector2f end = center_2 - unitDirection * 15.0f;



    //Cạnh là 1 rectangle
    sf::RectangleShape Line;
    //độ dày
    float thickness = 3.0f;
    //độ dài cạnh
    float len = length(start, end);

    // Thiết lập kích thước và góc cho hình chữ nhật
    Line.setSize(sf::Vector2f(len, thickness));
    Line.setFillColor(tmp->co_edge);
    Line.setOrigin(0, thickness / 2); // Đặt tâm hình chữ nhật để xoay

    // Đặt vị trí và góc cho hình chữ nhật
    Line.setPosition(start);
    Line.setRotation(angle(start, end)); // Chuyển đổi từ radian sang độ

    mWindow.draw(Line);

    sf::Vector2f mid = start + end;
    mid.x /= 2;
    mid.y /= 2;

    if(start.x > end.x) {
        sf::Vector2f pt(-unitDirection.y, unitDirection.x);
        mid = mid + pt * 15.0f;
    }
    else {
        sf::Vector2f pt(unitDirection.y, -unitDirection.x);
        mid = mid + pt * 15.0f;
    }

    //Vẽ giá trị
    sf::Text Value;
    Value.setFont(MyFont.get(FontID::ptsan));
    Value.setString(std::to_string(tmp -> val));
    Value.setCharacterSize(15);
    Value.setFillColor(sf::Color::Black);

    sf::FloatRect textRect = Value.getLocalBounds();
    Value.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top  + textRect.height / 2.0f);
    Value.setPosition(mid.x, mid.y);

    mWindow.draw(Value);


    //nhớ vẽ thêm giá trị trên đỉnh

}


void GraphScreen::draw_node(node *tmp) {

    sf::CircleShape circle(14, 100);
    circle.setOrigin(circle.getRadius(), circle.getRadius());
    circle.setOutlineThickness(3);
    circle.setPosition(tmp->pos_x, tmp->pos_y);
    circle.setOutlineColor(tmp -> co_outline);
    circle.setFillColor(tmp -> co_shape);


    //Vẽ giá trị
    sf::Text Value;
    Value.setFont(MyFont.get(FontID::ptsan));
    Value.setString(std::to_string(tmp -> val));
    Value.setCharacterSize(15);
    Value.setFillColor(tmp -> co_val);

    sf::FloatRect textRect = Value.getLocalBounds();
    Value.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top  + textRect.height / 2.0f);
    Value.setPosition(tmp->pos_x, tmp->pos_y);


    mWindow.draw(circle);
    mWindow.draw(Value);

}

void GraphScreen::draw_graph(std::vector<node*>ds_node, std::vector<edge*>ds_edge) {

    for(int i = 0; i < ds_edge.size(); i++) {
        draw_edge(ds_edge[i]);
    }

    for(int i = 0; i < ds_node.size(); i++) {
        draw_node(ds_node[i]);
    }
    return;
}


/*

void GraphScreen::draw_time_bar() {


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





