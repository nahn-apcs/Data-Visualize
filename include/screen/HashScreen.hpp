#ifndef HASH_SCREEN_HPP
#define HASH_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <ResourceID.hpp>
#include <string.h>
#include <memory>
#include <vector>



class HashScreen 
{
public:
    HashScreen(sf::RenderWindow &parWindow, TextureHolder &parTexture, FontHolder &parFont): mWindow(parWindow), MyTexture(parTexture), MyFont(parFont), Return(0) {}
    void            run();

private:
    sf::RenderWindow &mWindow;
    void            ProcessEvent();
    void            Update();
    void            Render();

    bool Return;
    bool is_play = 1;
    float speed = 1.0f;
    int mod = 0;

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
        int pos_x;
        int pos_y;
        int val;
        node *child;
        bool root;

        float per_edge = 255.0f;

        int state = 0;
        
        //0: chữ đen + nền trắng + viền đen
        //1: chữ cam + nền trắng + viền cam
        //2: chữ trắng + nền cam + viền cam
        //3: chữ trắng + nền xanh + viên xanh

        sf::Color co_shape = sf::Color::White;
        sf::Color co_outline = sf::Color::Black;
        sf::Color co_val = sf::Color::Black;

        node (int x = 0, int y = 0, int _val = 0, node* _child = NULL, bool _root = 0): pos_x(x), pos_y(y), child(_child), root(_root) {}

    };
    std::vector<node>ds_node;

    void reset_ds_node(std::vector<node>&tmp);
    void reset_node(node *tmp);
    void new_ds_node(int m);
    void draw_hash(std::vector<node>& ds_node);
    void copy_ds_node(std::vector<node>& new_ds_node);
    void draw_node(node *tmp);
    node* copy_node(node *tmp);
    void draw_edge(node *tmp);
    void change_color(node *tmp, float time, int t);

//insert function
private:
    void insert_hash(int val);
    void animation_shift_node_down(int block, float time);
    void set_ds_node(int block);
    void reset_color();
    void reset_color_node(node *tmp);

//search function
private:
    void search_hash(int val);
    void dfs_find(node *tmp, node *par, int need);

//delete function
private:
    void delete_hash(int val);
    void dfs_delete(node *tmp, node *par, int need);
    void animation_shift_node_up(node *tmp, float time);

//animation
private:
    std::vector<std::vector<node>>hash_state;
    int cur_stage = 0;

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
};



#endif //HASH_SCREEN_HPP