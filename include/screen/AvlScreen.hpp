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

        int hi_down = 0;//độ cao từ dưới
        int hi = 0;

        float per_edge = 0.0f;//lưu ý là nối từ nó lên cha
        float per_len_edge = 0.0f;
        float per_progress = 0.0f; //để tô màu cạnh

        
        //0: chữ đen + nền trắng + viền đen
        //1: chữ cam + nền trắng + viền cam
        //2: chữ trắng + nền cam + viền cam
        //3: chữ trắng + nền xanh + viên xanh

        sf::Color co_shape = sf::Color(255, 255, 255, 0);
        sf::Color co_outline = sf::Color(0, 0, 0, 0);
        sf::Color co_val = sf::Color(0, 0, 0, 0);

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
    void dfs_time(node *tmp, int hi, node *block);
    void insert_avl(int val);
    node* dfs_insert(node *tmp, node *par, int val);
    void set_new_postition(node *tmp);
    void set_last_postition(node *tmp);
    void shift_tree(node *need, float des, float des_len, float time);
    void dfs_shift_tree(node *tmp, float per);
    int get_high(node *tmp);
    int get_balance(node *tmp);
    void shift_tree_insert(float time);
    node *Right_rotation(node *tmp);
    node *Left_rotation(node *tmp);


//search
private:
    void search_avl(int val);
    void reset_color(node *tmp);
    void dfs_find(node *tmp, node *par, int val);

//delete 
private:
    void delete_avl(int val);
    node *dfs_delete(node *tmp, int val);
    void dfs_minn(node *tmp, node *boss);

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