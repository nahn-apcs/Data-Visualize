#ifndef GRAPH_SCREEN_HPP
#define GRAPH_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <ResourceID.hpp>
#include <string.h>
#include <memory>
#include <vector>



class GraphScreen 
{
public:
    GraphScreen(sf::RenderWindow &parWindow, TextureHolder &parTexture, FontHolder &parFont): mWindow(parWindow), MyTexture(parTexture), MyFont(parFont), Return(0) {}
    void            run();

private:
    sf::RenderWindow &mWindow;
    void            ProcessEvent();
    void            Update();
    void            Render();

    bool Return;
    bool is_play = 1;
    float speed = 1.0f;
    int num_node = 0;
    int num_edge = 0;
    bool fixed = 0;

//image
private:
    TextureHolder &MyTexture;
    struct image {
        int pos_x;
        int pos_y;
        bool change;
        bool disable;
        bool clicked = 0;
        TextureID a;
        TextureID b;
        std::string text = "";
        int box_pos_x = 0;
        int box_pos_y = 0;
        bool input = 0;
        bool pressed = 0;
        bool released = 0;

        image(TextureID _a, TextureID _b, int _pos_x = 0, int _pos_y = 0, int _d = 0): a(_a), b(_b), pos_x(_pos_x), pos_y(_pos_y), change(0), disable(_d) {}
    };
    std::vector<image> ds;

    void render_image();


//node
private:
    struct node {
        float pos_x;
        float pos_y;

        int val;

        sf::Vector2f F;


        
        //0: chữ đen + nền trắng + viền đen
        //1: chữ cam + nền trắng + viền cam
        //2: chữ trắng + nền cam + viền cam
        //3: chữ trắng + nền xanh + viên xanh

        sf::Color co_shape = sf::Color(255, 255, 255, 255);
        sf::Color co_outline = sf::Color(0, 0, 0, 255);
        sf::Color co_val = sf::Color(0, 0, 0, 255);

        node (float x = 0, float y = 0, int _val = 0, int _vt = 0): pos_x(x), pos_y(y) , val(_val){}
    };
    std::vector<node*> ds_node;
    node *hold_node = NULL;

//edge
private:
    struct edge{
        int val;
        node *u, *v;

        sf::Color co_edge = sf::Color::Black;

        edge(node *_u = NULL, node *_v = NULL, int _val = 0): u(_u), v(_v), val(_val) {}
    };
    std::vector<edge*> ds_edge;

//Force-Directed Drawings
private:
    void set_position();


//create
private:
    void create_random(int n, int m);

//text
private:
    FontHolder &MyFont;

//timebar
private:
    void draw_time_bar();

//o input
private:
    int so;
    bool back_space;
    void modify_input(std::string &s, int t);

//math function
private:
    float length(const sf::Vector2f& start, const sf::Vector2f& end);
    float angle(const sf::Vector2f& start, const sf::Vector2f& end);
    sf::Vector2f unit_vector(sf::Vector2f a, sf::Vector2f b);

//animation
private:
    int cur_state = 0;
    std::vector<std::vector<node*>>node_state;
    std::vector<std::vector<edge*>>edge_state;
    //lưu lại trạng thái màu của các đỉnh và các cạnh
    void clear_state();
    void clear_node(std::vector<node*> tmp);
    void clear_edge(std::vector<edge*> tmp);
    void clear_all();
    void draw_edge(edge *tmp);
    void draw_node(node *tmp);
    void draw_graph(std::vector<node*>ds_node, std::vector<edge*>ds_edge);
    void copy_node(std::vector<node*>tmp);
    void copy_edge(std::vector<edge*>tmp);
};



#endif //GRAPH_SCREEN_HPP