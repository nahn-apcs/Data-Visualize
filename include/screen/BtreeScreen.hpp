#ifndef BTREE_SCREEN_HPP
#define BTREE_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <ResourceID.hpp>
#include <string.h>
#include <memory>
#include <vector>



class BtreeScreen 
{
public:
    BtreeScreen(sf::RenderWindow &parWindow, TextureHolder &parTexture, FontHolder &parFont): mWindow(parWindow), MyTexture(parTexture), MyFont(parFont), Return(0) {}
    void            run();

private:
    sf::RenderWindow &mWindow;
    void            ProcessEvent();
    void            Update();
    void            Render();

    bool Return;
    bool is_play = 1;
    float speed = 1.0f;

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



//small_node
private:
    struct small_node {
        int pre_pos_x;
        int pre_pos_y;
        int cur_pos_x;
        int cur_pos_y;
        int nx_pos_x;
        int nx_pos_y;

        int val = 0;

        int hi = 0;

        
        //0: chữ đen + nền trắng + viền đen
        //1: chữ cam + nền trắng + viền cam
        //2: chữ trắng + nền cam + viền cam
        //3: chữ trắng + nền xanh + viên xanh

        sf::Color co_shape = sf::Color(255, 255, 255, 0);
        sf::Color co_outline = sf::Color(0, 0, 0, 0);
        sf::Color co_val = sf::Color(0, 0, 0, 0);

        small_node (int x = 0, int y = 0, int _val = 0): cur_pos_x(x), cur_pos_y(y), nx_pos_x(x), nx_pos_y(y), pre_pos_x(x), pre_pos_y(y) , val(_val) {}
    };

//edge
private:
    struct edge {
        int pre_st_x = 0;
        int pre_st_y = 0;
        int cur_st_x = 0;
        int cur_st_y = 0;
        int nx_st_x = 0;
        int nx_st_y = 0;

        int pre_en_x = 0;
        int pre_en_y = 0;
        int cur_en_x = 0;
        int cur_en_y = 0;
        int nx_en_x = 0;
        int nx_en_y = 0;

        sf::Color co_edge = sf::Color(0, 0, 0, 0);

        float per_process = 0; //tô màu cạnh trong quá trình đi xuống
    };

//big_node
private:
    struct big_node {
        std::vector<small_node*> ds_small_node;
        std::vector<edge*> ds_edge;  
        std::vector<big_node*> ds_big_node;

        int num_small_left = 0;
        int num_big_node_left = 0;
    };

    big_node *root = NULL;

    void reset_big_node(big_node *tmp);
    void reset_root();

    big_node *copy_root(big_node *tmp);

//insert
private:
    int cnt_small_node = 0;
    int time_dfs = 0;
    void dfs_time(small_node *block);
    void dfs(big_node *tmp, small_node *block);
    void set_new_postition();
    void set_last_postition();


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

//animation
private:
    //int cur_state = 0;
    //std::vector<node*>avl_state;
    //void clear_state();
};



#endif //BTREE_SCREEN_HPP