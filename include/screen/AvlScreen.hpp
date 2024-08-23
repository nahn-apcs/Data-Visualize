#ifndef AVL_SCREEN_HPP
#define AVL_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <ResourceID.hpp>
#include <string.h>
#include <memory>
#include <vector>



class AVLScreen 
{
public:
    AVLScreen(sf::RenderWindow &parWindow, TextureHolder &parTexture, FontHolder &parFont): mWindow(parWindow), MyTexture(parTexture), MyFont(parFont), Return(0) {}
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



//node
private:
    struct node {
        int pre_pos_x;
        int pre_pos_y;
        int cur_pos_x;
        int cur_pos_y;
        int nx_pos_x;
        int nx_pos_y;

        int pre_par_pos_x = 0;
        int pre_par_pos_y = 0;
        int cur_par_pos_x = 0;
        int cur_par_pos_y = 0;
        int nx_par_pos_x = 0;
        int nx_par_pos_y = 0;
        int val;
        node *Left = NULL, *Right = NULL;
        node *par = NULL;
        int vt; //Thứ tự thăm dfs

        float per_edge = 255.0f;//lưu ý là nối từ nó lên cha
        float per_progress = 0; //để tô màu cạnh

        
        //0: chữ đen + nền trắng + viền đen
        //1: chữ cam + nền trắng + viền cam
        //2: chữ trắng + nền cam + viền cam
        //3: chữ trắng + nền xanh + viên xanh

        sf::Color co_shape = sf::Color::White;
        sf::Color co_outline = sf::Color::Black;
        sf::Color co_val = sf::Color::Black;

        node (int x = 0, int y = 0, int _val = 0): cur_pos_x(x), cur_pos_y(y), nx_pos_x(x), nx_pos_y(y), pre_pos_x(x), pre_pos_y(y) , val(_val) {}
    };
    node *root = NULL;

    void clear_root(node *tmp); //xoá dữ liệu
    node *copy_root(node *tmp); //copy dữ liệu
    void change_color_and_edge(node *tmp, float time, int t, float des); //đổi màu của nút
    void draw_avl(node *tmp);
    void draw_edge(node *tmp);

//insert

private:
    int time_dfs;
    void dfs_time(node *tmp);
    void insert_avl(int val);
    void dfs_insert(node *tmp, node *par, int val);

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
    int cur_state = 0;
    std::vector<node*>avl_state;
    void clear_state();
};



#endif //AVL_SCREEN_HPP