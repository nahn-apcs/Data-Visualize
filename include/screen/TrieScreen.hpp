#ifndef TRIE_SCREEN_HPP
#define TRIE_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <ResourceID.hpp>
#include <string.h>
#include <memory>
#include <vector>



class TrieScreen 
{
public:
    TrieScreen(sf::RenderWindow &parWindow, TextureHolder &parTexture, FontHolder &parFont): mWindow(parWindow), MyTexture(parTexture), MyFont(parFont), Return(0) {}
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


        int cnt = 0;//đếm số xâu đi qua nó, nếu = 0 thì nghĩa là xoá nó
        std::string ch = "";
        int vt = 0;
        node *c[26];
        node *par = NULL;
        int en = 0;

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

        node (int x = 0, int y = 0, int _val = 0): cur_pos_x(x), cur_pos_y(y), nx_pos_x(x), nx_pos_y(y), pre_pos_x(x), pre_pos_y(y) {
            for(int i = 0; i < 26; i++) c[i] = NULL;
        }
    };
    node *root = NULL;

    void clear_root(node *tmp); //xoá dữ liệu
    node *copy_root(node *tmp); //copy dữ liệu
    void change_color_and_edge(node *tmp, float time, int t, float des); //đổi màu của nút
    void draw_trie(node *tmp);
    void draw_edge(node *tmp);
    int time_dfs = 0;
    void dfs_time(node *tmp, int hi, node *block);
    void set_new_postition(node *tmp, node *block);
    void set_last_postition(node *tmp, node *block);
    int dfs_minn(node *tmp, node *block);
    int dfs_maxx(node *tmp, node *block);


//insert
private:
    void insert_trie(const std::string &s);
    void dfs_insert(node *tmp, node *par, const std::string &s, int cur);
    void shift_tree_insert(node *need, float des, float des_len, float time);
    void dfs_shift_tree(node *tmp, float per);
    void reset_color(node *tmp);

//search
private:
    void search_trie(const std::string &s);
    bool dfs_search(node *tmp, node *par, const std::string &s, int cur);

//delete
private:
    void delete_trie(const std::string &s);
    bool dfs_delete(node *tmp, node *par, const std::string &s, int cur);

//text
private:
    FontHolder &MyFont;

//timebar
private:
    void draw_time_bar();

//o input
private:
    int chu;
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
    std::vector<node*>trie_state;
    void clear_state();
};



#endif //TRIE_SCREEN_HPP