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

void GraphScreen::clear_node(std::vector<node*> &tmp) {
    while(tmp.size()) {
        delete tmp.back();
        tmp.pop_back();
    }
    return;
}
void GraphScreen::clear_edge(std::vector<edge*> &tmp) {
    while(tmp.size()) {
        delete tmp.back();
        tmp.pop_back();
    }
    return;
}

void GraphScreen::clear_state() {
    cur_state = 0;
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

std::vector<GraphScreen::edge*> GraphScreen::copy_edge() {
    std::vector<edge*> tmp;
    tmp.clear();
    for(int i = 0; i < ds_edge.size(); i++) {
        edge *new_edge = new edge;
        *new_edge = *ds_edge[i];
        tmp.push_back(new_edge);
    }
    return tmp;
}

std::vector<GraphScreen::node*> GraphScreen::copy_node() {
    std::vector<node*> tmp;
    tmp.clear();
    for(int i = 0; i < ds_node.size(); i++) {
        node *new_node = new node;
        *new_node = *ds_node[i];
        tmp.push_back(new_node);
    }
    return tmp;
}

bool GraphScreen::cmp(const edge *a, const edge *b) {
    return a->val < b->val;
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
    if(ds_edge.size()) std::sort(ds_edge.begin(), ds_edge.end(), cmp);
    node_state.push_back(copy_node());
    edge_state.push_back(copy_edge());
}

sf::Vector2f GraphScreen::unit_vector(sf::Vector2f a, sf::Vector2f b) {
    sf::Vector2f direction = b - a;
    sf::Vector2f unitDirection = direction / length(a, b);
    return unitDirection;
}


void GraphScreen::set_position() {
    if(fixed) return;

    std::vector<sf::Vector2f> sum_F(num_node, sf::Vector2f(0, 0));

    float c_rep = 10000;
    float c_spring = 1;


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


        sf::Vector2f F_attr = c_spring * log(len / 100) * unit_vector(u, v);
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

            sf::Vector2f F_rep = 1000 / len / len * unit_vector(u, v);

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

                sf::Vector2f F_rep = 1000 / len / len * unit_vector(u, v);

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

void GraphScreen::change_color_node(node *tmp, sf::Color des, float time) {
    sf::Color cur_val = tmp->co_val;
    sf::Color cur_shape = tmp->co_shape;
    sf::Color cur_outline = tmp->co_outline;

    sf::Color des_val(255, 255, 255);

    for(int i = 1; i <= 60 * time; i++) {
        tmp->co_val.r = cur_val.r + (des_val.r - cur_val.r) * 1.0 * i / (60 * time);
        tmp->co_val.g = cur_val.g + (des_val.g - cur_val.g) * 1.0 * i / (60 * time);
        tmp->co_val.b = cur_val.b + (des_val.b - cur_val.b) * 1.0 * i / (60 * time);


        tmp->co_shape.r = cur_shape.r + (des.r - cur_shape.r) * 1.0 * i / (60 * time);
        tmp->co_shape.g = cur_shape.g + (des.g - cur_shape.g) * 1.0 * i / (60 * time);
        tmp->co_shape.b = cur_shape.b + (des.b - cur_shape.b) * 1.0 * i / (60 * time);


        tmp->co_outline.r = cur_outline.r + (des.r - cur_outline.r) * 1.0 * i / (60 * time);
        tmp->co_outline.g = cur_outline.g + (des.g - cur_outline.g) * 1.0 * i / (60 * time);
        tmp->co_outline.b = cur_outline.b + (des.b - cur_outline.b) * 1.0 * i / (60 * time);

        node_state.push_back(copy_node());
        edge_state.push_back(copy_edge());
    }
}

void GraphScreen::change_color_edge(edge *tmp, sf::Color des, float time) {
    sf::Color cur = tmp->co_edge;


    for(int i = 1; i <= 60 * time; i++) {
        tmp->co_edge.r = cur.r + (des.r - cur.r) * 1.0 * i / (60 * time);
        tmp->co_edge.g = cur.g + (des.g - cur.g) * 1.0 * i / (60 * time);
        tmp->co_edge.b = cur.b + (des.b - cur.b) * 1.0 * i / (60 * time);
        tmp->co_edge.a = cur.a + (des.a - cur.a) * 1.0 * i / (60 * time);

        node_state.push_back(copy_node());
        edge_state.push_back(copy_edge());
    }
} 

void GraphScreen::dfs_cc(int u, sf::Color color, bool *vi, bool *vi_edge) {

    change_color_node(ds_node[u], color, 0.3f);

    vi[u] = 1;
    for(int v = 0; v < num_node; v++) {
        for(int i = 0; i < num_edge; i++) {
            if(vi_edge[i] == 1) continue;
            if((ds_edge[i]->u->val == u + 1 && ds_edge[i]->v->val == v + 1) || (ds_edge[i]->u->val == v + 1 && ds_edge[i]->v->val == u + 1)) {
                change_color_edge(ds_edge[i], color, 0.3f);
                vi_edge[i] = 1;
                if(vi[v] == 0) dfs_cc(v, color, vi, vi_edge);
                break;
            }
        }
    }
}

void GraphScreen::find_cc() {

    reset_color();
    clear_state();
    cur_state = 0;
    node_state.push_back(copy_node());
    edge_state.push_back(copy_edge());

    bool *vi = new bool[100];
    bool *vi_edge = new bool[100];

    int cnt = 0;

    for(int i = 0; i < num_node; i++) {
        if(!vi[i]) {
            dfs_cc(i, ds_color[cnt % ds_color.size()], vi, vi_edge);
            cnt++;
        }
    }


    delete[] vi;
    delete[] vi_edge;
}

int GraphScreen::Find(int *par, int u) {
    if(par[u] < 0) return u;
    return par[u] = Find(par, par[u]);
}

bool GraphScreen::join(int *par, int u, int v) {
    u = Find(par, u);
    v = Find(par, v);

    if(u != v) {

        if(par[u] > par[v]) std::swap(u, v);
        par[u] += par[v];
        par[v] = u;

        return 1;
    }
    return 0;
}

void GraphScreen::cal_mst() {
    reset_color();
    clear_state();
    cur_state = 0;
    node_state.push_back(copy_node());
    edge_state.push_back(copy_edge());

    int *par = new int[num_node];

    for(int i = 0; i < num_node; i++) par[i] = -1;

    for(int i = 0; i < num_edge; i++) {
        int u = ds_edge[i]->u->val - 1;
        int v = ds_edge[i]->v->val - 1;

        change_color_edge(ds_edge[i], ds_color[0], 0.3f);
        change_color_edge(ds_edge[i], ds_color[0], 0.3f);

        if(join(par, u, v)) {
            change_color_edge(ds_edge[i], ds_color[1], 0.3f);
        }
        else {
            sf::Color co = ds_color[1];
            co.a = 0;
            change_color_edge(ds_edge[i], co, 0.3f);
        }
    }



    delete[]par;

}



void GraphScreen::run() {
    //Add_image
    ds.push_back(image(TextureID::app_bg, TextureID::app_bg, 0, 0, 0));//0
    ds.push_back(image(TextureID::back_menu_button, TextureID::back_menu_button, 10, 10, 0));//1
    ds.push_back(image(TextureID::fixed_on, TextureID::fixed_on_in, 0, 567, 1));//2
    ds.push_back(image(TextureID::fixed_off, TextureID::fixed_off_in, 0, 567, 0));//3
    ds.push_back(image(TextureID::matrix, TextureID::matrix_in, 0, 617, 0));//4
    ds.push_back(image(TextureID::create_button, TextureID::create_button_in, 0, 667, 0));//5
    ds.push_back(image(TextureID::cc, TextureID::cc_in, 0, 717, 0));//6
    ds.push_back(image(TextureID::mst, TextureID::mst_in, 0, 767, 0));//7
    ds.push_back(image(TextureID::tool_bar, TextureID::tool_bar, 0, 842, 0));//8
    ds.push_back(image(TextureID::time_bar, TextureID::time_bar, 468, 864, 0));//9
    ds.push_back(image(TextureID::speed_bar, TextureID::speed_bar, 1182, 864, 0));//10
    ds.push_back(image(TextureID::go_begin_disable, TextureID::go_begin_white, 190, 852, 0));//11
    ds.push_back(image(TextureID::step_backward_disable, TextureID::step_backward_white, 243, 852, 0));//12
    ds.push_back(image(TextureID::return_button, TextureID::return_button, 302, 854, 0));//13
    ds.push_back(image(TextureID::play_button, TextureID::play_button, 298, 852, 1));//14
    ds.push_back(image(TextureID::pause_button, TextureID::pause_button, 298, 852, 1));//15
    ds.push_back(image(TextureID::step_forward_disable, TextureID::step_forward_white, 353, 852, 0));//16
    ds.push_back(image(TextureID::go_end_disable, TextureID::go_end_white, 397, 852, 0));//17
    ds.push_back(image(TextureID::cuc, TextureID::cuc, 1182, 864, 0)); //cuc speed //18


    //create
    ds.push_back(image(TextureID::n_input, TextureID::n_input, 160, 667, 0));//19
    ds.push_back(image(TextureID::random_button, TextureID::random_button, 247, 674, 0));//20
    ds.push_back(image(TextureID::m_input, TextureID::m_input, 300, 667, 0));//21
    ds.push_back(image(TextureID::random_button, TextureID::random_button, 387, 674, 0));//22
    ds.push_back(image(TextureID::go_button, TextureID::go_button_in, 440, 667, 0));//23
    ds.push_back(image(TextureID::file_button, TextureID::file_button_in, 500, 667, 0));//24

    ds[19].input = 1;
    ds[19].box_pos_x = 212;
    ds[19].box_pos_y = 682;


    ds[21].input = 1;
    ds[21].box_pos_x = 352;
    ds[21].box_pos_y = 682;

    clear_all();

    ds_color.push_back(sf::Color(255, 165, 0));
    ds_color.push_back(sf::Color(106, 220, 127));
    ds_color.push_back(sf::Color(255, 127, 80));
    ds_color.push_back(sf::Color(255, 160, 122));
    ds_color.push_back(sf::Color(255, 99, 71));
    ds_color.push_back(sf::Color(255, 215, 0));
    ds_color.push_back(sf::Color(154, 205, 50));
    ds_color.push_back(sf::Color(127, 255, 0));
    ds_color.push_back(sf::Color(0, 255, 127));
    ds_color.push_back(sf::Color(32, 178, 170));
    ds_color.push_back(sf::Color(64, 224, 208));
    ds_color.push_back(sf::Color(135, 206, 235));
    ds_color.push_back(sf::Color(30, 144, 255));
    ds_color.push_back(sf::Color(0, 191, 255));
    ds_color.push_back(sf::Color(238, 130, 238));
    ds_color.push_back(sf::Color(255, 105, 180));
    ds_color.push_back(sf::Color(240, 128, 128));



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

                        if(i >= 2 && i <= 7) {
                            for(int j = 2; j <= 7; j++) ds[j].clicked = 0;
                        }
                        else if(i >= 19 && i <= 24) {
                            for(int j = 19; j <= 24; j++) ds[j].clicked = 0;
                        }

                        ds[i].clicked = 1;
                        if(i == 18) ds[i].pressed = 1;
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
                if(ds[18].pressed == 1) {
                    ds[18].pressed = 0;
                    ds[18].released = 1;
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
                ds[17].clicked = 1;
            }
            else if(event.key.code == sf::Keyboard::Down) {
                ds[11].clicked = 1;
            }
            else if(event.key.code == sf::Keyboard::Left) {
                ds[12].clicked = 1;
            }
            else if(event.key.code == sf::Keyboard::Right) {
                ds[16].clicked = 1;
            }
            else if(event.key.code == sf::Keyboard::Space) {
                if(ds[13].disable == 0) {
                    ds[13].clicked = 1;
                }
                else if(ds[14].disable == 0) {
                    ds[14].clicked = 1;
                }
                else if(ds[15].disable == 0) {
                    ds[15].clicked = 1;
                }
            }
            else if(event.key.code == sf::Keyboard::Enter) {
                if(ds[23].disable == 0) {
                    ds[23].clicked = 1;
                }
            }
        }
    }
}

void GraphScreen::modify_input(std::string &s, int maxx) {
    int tmp = 0;
    for(int i = 0; i < s.size(); i++) {
        tmp = tmp * 10 + s[i] - '0';
    }
    if(tmp > maxx) tmp = maxx;
    s = std::to_string(tmp);
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
    if(ds[18].pressed) {
        sf::Vector2i mousePos = sf::Mouse::getPosition(mWindow);
        ds[18].pos_x = mousePos.x;
        ds[18].pos_x = std::max(ds[18].pos_x, 1182);
        ds[18].pos_x = std::min(ds[18].pos_x, 1317);
    }
    if(ds[18].released) {
        ds[18].released = 0;
        speed = 1.0 * (ds[18].pos_x - 1182) / 135 * 4 + 1;
    }

    //fixed button
    if(ds[2].disable == 0 && ds[2].clicked == 1) {
        ds[2].disable = 1;
        ds[2].clicked = 0;
        ds[3].disable = 0;
        ds[3].clicked = 0;
        fixed = 0;
    }
    if(ds[3].disable == 0 && ds[3].clicked == 1) {
        ds[3].disable = 1;
        ds[3].clicked = 0;
        ds[2].disable = 0;
        ds[2].clicked = 0;
        fixed = 1;
    }

    //matrix button
    if(ds[4].clicked == 1) {

        ds[4].clicked = 0;

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

            int n = 0;

            std::vector<int>e[100];

            if(fin.is_open()) {
                std::string s;
                while(getline(fin, s)) {
                    int tmp = 0;
                    for(int i = 0; i < s.size(); i++) {
                        if(s[i] == ' ') {
                            e[n].push_back(tmp);
                            tmp = 0;
                        }
                        else {
                            tmp = tmp * 10 + s[i] - '0';
                        }
                    }
                    e[n].push_back(tmp);
                    n++;
                }
            }

            if(n <= 30) {
                clear_all();

                for(int i = 1; i <= n; i++) {
                    int pos_x = rand_6(0, 1440);
                    int pos_y = rand_6(0, 900);

                    node *new_node = new node(pos_x, pos_y, i);

                    ds_node.push_back(new_node);
                }

                for(int i = 0; i < n; i++) {
                    for(int j = i + 1; j < n; j++) {
                        if(e[i][j] != 0) {
                            edge *new_edge = new edge(ds_node[i], ds_node[j], e[i][j]);
                            ds_edge.push_back(new_edge);
                        }
                    }
                }
                if(ds_edge.size()) std::sort(ds_edge.begin(), ds_edge.end(), cmp);
                num_edge = ds_edge.size();
                num_node = n;
                node_state.push_back(copy_node());
                edge_state.push_back(copy_edge());
            }


            fin.close();
        }
    }

    //create button
    if(ds[5].clicked == 1) {
        ds[19].disable = ds[20].disable = ds[21].disable = ds[22].disable = ds[23].disable = ds[24].disable = 0;
    }
    else {
        ds[19].disable = ds[20].disable = ds[21].disable = ds[22].disable = ds[23].disable = ds[24].disable = 1;
        ds[19].text = ds[21].text = "";
    }




    ////xử lí theo tác nhập từ bàn phím
    if(ds[19].disable == 0 && ds[19].clicked == 1) {
        std::string &s = ds[19].text;
        if(back_space == 1) {
            if(s.size()) s.pop_back();
        }
        else if(so >= 0 && so < 10) {
            s += char(so + '0');
        }
        modify_input(s, 30);
    }
    else if(ds[21].disable == 0 && ds[21].clicked == 1) {
        std::string &s = ds[21].text;
        if(back_space == 1) {
            if(s.size()) s.pop_back();
        }
        else if(so >= 0 && so < 10) {
            s += char(so + '0');
        }

        int n = 0;
        for(int i = 0; i < ds[19].text.size(); i++) {
            n = n * 10 + ds[19].text[i] - '0';
        }

        int maxx = std::min(99, n * (n - 1) / 2);

        modify_input(s, maxx);
    }

    so = 10;
    back_space = 0;

    if(ds[20].clicked == 1) {
        ds[20].clicked = 0;

        int n = rand_6(1, 20);
        ds[19].text = std::to_string(n);
    }

    if(ds[22].clicked == 1) {
        ds[22].clicked = 0;

        int n = 0;
        for(int i = 0; i < ds[19].text.size(); i++) {
            n = n * 10 + ds[19].text[i] - '0';
        }

        int m = rand_6(0, std::min(99, n * (n - 1) / 2));

        ds[21].text = std::to_string(m);
    }

    if(ds[23].clicked == 1) {
        ds[23].clicked = 0;

        int n = 0, m = 0;
        for(int i = 0; i < ds[19].text.size(); i++) {
            n = n * 10 + ds[19].text[i] - '0';
        }
        for(int i = 0; i < ds[21].text.size(); i++) {
            m = m * 10 + ds[21].text[i] - '0';
        }

        create_random(n, m);
        ds[5].clicked = 0;
    }

    if(ds[24].clicked == 1) {
        ds[24].clicked = 0;
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

            int n = 0;

            std::vector<std::pair<std::pair<int,int>, int>>e;

            int d = 0;

            if(fin.is_open()) {
                std::string s;
                while(getline(fin, s)) {
                    d++;
                    if(d == 1) {
                        for(int i = 0; i < s.size(); i++) {
                            n = n * 10 + s[i] - '0';
                        }
                    }
                    else {
                        int tmp = 0;
                        std::vector<int>a;
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

                        e.push_back(std::make_pair(std::make_pair(a[0], a[1]), a[2]));
                    }
                }
            }

            if(n <= 30) {
                clear_all();

                for(int i = 1; i <= n; i++) {
                    int pos_x = rand_6(0, 1440);
                    int pos_y = rand_6(0, 900);

                    node *new_node = new node(pos_x, pos_y, i);

                    ds_node.push_back(new_node);
                }

                for(int i = 0; i < e.size(); i++) {
                    edge *new_edge = new edge(ds_node[e[i].first.first - 1], ds_node[e[i].first.second - 1], e[i].second);
                    ds_edge.push_back(new_edge);
                }
                if(ds_edge.size()) std::sort(ds_edge.begin(), ds_edge.end(), cmp);
                num_edge = ds_edge.size();
                num_node = n;
                node_state.push_back(copy_node());
                edge_state.push_back(copy_edge());
            }


            fin.close();
        }
        ds[5].clicked = 0;
    }

    if(ds[6].clicked == 1) {
        ds[6].clicked = 0;

        find_cc();

        is_play = 1;
        ds[15].disable = 0;
        ds[14].disable = 1;
        ds[13].disable = 1;
    }

    if(ds[7].clicked == 1) {
        ds[7].clicked = 0;
        cal_mst();

        is_play = 1;
        ds[15].disable = 0;
        ds[14].disable = 1;
        ds[13].disable = 1;
    }

    /*
    is_play = 1;
    ds[15].disable = 0;
    ds[14].disable = 1;
    ds[13].disable = 1;
    */

    

    //full_back
    if(ds[11].clicked) {
        cur_state = 0;
        ds[11].clicked = 0;
        is_play = 0;
        ds[14].disable = 0;
        ds[15].disable = 1;
        ds[13].disable = 1;
    }


    //back_step

    if(ds[12].clicked) {
        cur_state -= 30;
        if(cur_state < 0) cur_state = 0;
        ds[12].clicked = 0;
        is_play = 0;
        ds[14].disable = 0;
        ds[15].disable = 1;
        ds[13].disable = 1;
    }

    //return
    if(ds[13].clicked) {
        cur_state = 0;
        ds[13].clicked = 0;
        ds[13].disable = 1;
        ds[15].disable = 0;
        is_play = 1;
    }

    //play
    if(ds[14].clicked) {
        ds[14].clicked = 0;
        ds[14].disable = 1;
        ds[15].disable = 0;
        is_play = 1;
        ds[13].disable = 1;
    }


    //pause
    if(ds[15].clicked) {
        ds[15].clicked = 0;
        is_play = 0;
        ds[14].disable = 0;
        ds[15].disable = 1;
        ds[13].disable = 1;
    }


    //go_step
    if(ds[16].clicked) {
        ds[16].clicked = 0;
        cur_state += 30;
        if(cur_state >= node_state.size()) cur_state = node_state.size() - 1;
        is_play = 0;
        ds[14].disable = 0;
        ds[15].disable = 1;
        ds[13].disable = 1;
    }

    //full_go
    if(ds[17].clicked) {
        ds[17].clicked = 0;
        cur_state = node_state.size() - 1;
        is_play = 0;
        ds[14].disable = 0;
        ds[15].disable = 1;
        ds[13].disable = 1;
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


 
    cur_state = std::min(cur_state, (int)node_state.size() - 1);
    if(cur_state >= 0 && cur_state < node_state.size()) {
        draw_graph(node_state[cur_state], edge_state[cur_state]);
        draw_time_bar();
    }



    cur_state = cur_state + is_play * (int)speed;
    cur_state = std::min(cur_state, (int)node_state.size() - 1);
    cur_state = std::max(cur_state, 0);
    if(cur_state == node_state.size() - 1) {
        is_play = 0;
        ds[14].disable = ds[15].disable = 1;
        ds[13].disable = 0;
    }


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

void GraphScreen::draw_edge(edge *tmp, edge *color) {



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
    Line.setFillColor(color->co_edge);
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
    Value.setFillColor(color->co_edge);

    sf::FloatRect textRect = Value.getLocalBounds();
    Value.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top  + textRect.height / 2.0f);
    Value.setPosition(mid.x, mid.y);

    mWindow.draw(Value);


    //nhớ vẽ thêm giá trị trên đỉnh

}


void GraphScreen::draw_node(node *tmp, node *color) {

    sf::CircleShape circle(14, 100);
    circle.setOrigin(circle.getRadius(), circle.getRadius());
    circle.setOutlineThickness(3);
    circle.setPosition(tmp->pos_x, tmp->pos_y);
    circle.setOutlineColor(color -> co_outline);
    circle.setFillColor(color -> co_shape);


    //Vẽ giá trị
    sf::Text Value;
    Value.setFont(MyFont.get(FontID::ptsan));
    Value.setString(std::to_string(tmp -> val));
    Value.setCharacterSize(15);
    Value.setFillColor(color -> co_val);

    sf::FloatRect textRect = Value.getLocalBounds();
    Value.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top  + textRect.height / 2.0f);
    Value.setPosition(tmp->pos_x, tmp->pos_y);



    mWindow.draw(circle);
    mWindow.draw(Value);

}

void GraphScreen::draw_graph(std::vector<node*> &node_state, std::vector<edge*> &edge_state) {

    for(int i = 0; i < ds_edge.size(); i++) {
        draw_edge(ds_edge[i], edge_state[i]);
    }

    for(int i = 0; i < ds_node.size(); i++) {
        draw_node(ds_node[i], node_state[i]);
    }
    return;
}

void GraphScreen::reset_color() {
    for(int i = 0; i < ds_node.size(); i++) {
        ds_node[i]->co_shape = sf::Color(255, 255, 255, 255);
        ds_node[i]->co_outline = sf::Color(0, 0, 0, 255);
        ds_node[i]->co_val = sf::Color(0, 0, 0, 255);
    }

    for(int i = 0; i < ds_edge.size(); i++) {
        ds_edge[i]->co_edge = sf::Color(0, 0, 0, 255);
    }
}




void GraphScreen::draw_time_bar() {


    //rgb(116,255,188)
    float per = 1.0 * (cur_state + 1) / (int)node_state.size();

    sf::RectangleShape Line;

    Line.setSize(sf::Vector2f(500.0f * per, 15));
    Line.setFillColor(sf::Color(116, 225, 188));

    // Đặt vị trí và góc cho hình chữ nhật
    Line.setPosition(468, 864);

    mWindow.draw(Line);

}








