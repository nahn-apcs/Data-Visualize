#ifndef HEAP_SCREEN_HPP
#define HEAP_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <ResourceID.hpp>
#include <string.h>
#include <memory>
#include <vector>


class HeapScreen 
{
public:
    HeapScreen(sf::RenderWindow &parWindow, TextureHolder &parTexture, FontHolder &parFont): mWindow(parWindow), MyTexture(parTexture), MyFont(parFont), Return(0) {}
    void            run();

private:
    sf::RenderWindow &mWindow;
    void            ProcessEvent();
    void            Update();
    void            Render();

    bool Return;
    bool is_play = 1;

//image
private:
    TextureHolder &MyTexture;
    struct image {
        int pos_x;
        int pos_y;
        bool change;
        bool disable;//danh cho nut start, play, stop
        TextureID a;
        TextureID b;

        image(TextureID _a, TextureID _b, int _pos_x = 0, int _pos_y = 0, int _d = 0): a(_a), b(_b), pos_x(_pos_x), pos_y(_pos_y), change(0), disable(_d) {}
    };
    std::vector<image> ds;

    void render_image();

//text
private:
    FontHolder &MyFont;


//insert function
private:

    struct node {
        int pos_x, pos_y, id, val;
        int vt;
        float per_node, per_edge_l, per_edge_r;
        float per_next;//độ đậm nếu đó là nút next
        bool now, next; //nut hien tai, nut tiep theo

        node (int _pos_x = 0, int _pos_y = 0, int _val = 0): pos_x(_pos_x), pos_y(_pos_y), val(_val), per_node(0), per_edge_l(0), per_edge_r(0), per_next(0), now(0), next(0) {}
    };
    std::vector<node>ds_node;
    int time_vi;


    void reset_heap();
    void insert_heap(int val);
    void dfs_inoder(int u, int block);
    void set_ds_node();
    void draw_node(int u, std::vector<node>&, std::vector<node>&);
    void up_heapify(int u);
    void animation_shift_tree_new_node(float time);
    void draw_heap(std::vector<node>& tree_stage, std::vector<node>& postition_stage);
    void animation_swap_node(int cha, int u, float time);


//animation
private:
    std::vector<std::vector<node>>tree_state;
    std::vector<std::vector<node>>postition_state;
    int cur_stage = 0;


//timebar
private:
    void draw_time_bar();


//extract_max_function
private:
    void extract_max();
    void delete_node(int u);
    void animation_shift_tree_delete_node(float time);
    void set_ds_node_delete();
    void down_heapify(int u);


//update_function
private:
    void update_pos(int vt, int val);

//delete_function
private:
    void delete_pos(int vt);
};



#endif //HEAP_SCREEN_HPP