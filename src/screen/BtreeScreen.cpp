#include <BtreeScreen.hpp>
#include <MenuScreen.hpp>
#include <SFML/Graphics.hpp>
#include <math.h>
#include <random>
#include <tinyfiledialogs.h>
#include <fstream>

std::mt19937 rng_3(std::chrono::steady_clock::now().time_since_epoch().count());

int rand_3(int l, int r) {
    return l + rng_3() % (r - l + 1);
}

void BtreeScreen::reset_big_node(big_node *tmp) {
    if(tmp == NULL) return;
    for(int i = 0; i < tmp->ds_big_node.size(); i++) {
        reset_big_node(tmp->ds_big_node[i]);
    }
    tmp->ds_big_node.clear();

    while(tmp->ds_small_node.size()) {
        delete tmp->ds_small_node.back();
        tmp->ds_small_node.pop_back();
    }

    while(tmp->ds_edge.size()) {
        delete tmp->ds_edge.back();
        tmp->ds_edge.pop_back();
    }
    delete tmp;
    tmp = NULL;
    return;
}
void BtreeScreen::reset_root() {
    reset_big_node(root);
    root = NULL;
}

void BtreeScreen::reset_state() {
    while(btree_state.size()) {
        reset_big_node(btree_state.back());
        btree_state.pop_back();
    }
    cur_state = 0;
}

BtreeScreen::big_node *BtreeScreen::copy_root(big_node *tmp) {
    if(tmp == NULL) return tmp;
    big_node *new_big_node = new big_node;
    *new_big_node = *tmp;

    new_big_node->ds_big_node.clear();
    new_big_node->ds_edge.clear();
    new_big_node->ds_small_node.clear();

    for(int i = 0; i < tmp->ds_small_node.size(); i++) {
        small_node *new_small_node = new small_node;
        *new_small_node = *tmp->ds_small_node[i];
        new_big_node->ds_small_node.push_back(new_small_node);
    }

    for(int i = 0; i < tmp->ds_edge.size(); i++) {
        edge *new_edge = new edge;
        *new_edge = *tmp->ds_edge[i];
        new_big_node->ds_edge.push_back(new_edge);
    }

    for(int i = 0; i < tmp->ds_big_node.size(); i++) {
        new_big_node->ds_big_node.push_back(copy_root(tmp->ds_big_node[i]));
    }
    return new_big_node;
}

void BtreeScreen::dfs(big_node *tmp, small_node *block) {
    if(tmp == NULL) return;
    tmp->num_small_left = 0;
    tmp->num_big_node_left = 0;
    if(tmp->ds_big_node.size() == 0) {
        tmp->num_small_left = cnt_small_node;
        tmp->num_big_node_left = time_dfs;
        time_dfs++;

        for(int i = 0; i < tmp->ds_small_node.size(); i++) {
            if(tmp->ds_small_node[i] != block) cnt_small_node++;
        }
    }
    for(int i = 0; i < tmp->ds_big_node.size(); i++) {
        dfs(tmp->ds_big_node[i], block);
    }
}

void BtreeScreen::dfs_time(small_node *block) {
    time_dfs = 0;
    cnt_small_node = 0;

    dfs(root, block);
}

int BtreeScreen::dfs_minn(big_node *tmp) {
    if(tmp->ds_big_node.size() == 0) return tmp->ds_small_node[0]->nx_pos_x;
    else return dfs_minn(tmp->ds_big_node[0]);
}

int BtreeScreen::dfs_maxx(big_node *tmp) {
    if(tmp->ds_big_node.size() == 0) return tmp->ds_small_node.back()->nx_pos_x;
    else return dfs_maxx(tmp->ds_big_node.back());
}

void BtreeScreen::set_new_postition(big_node *tmp, small_node *block, int hi) {
    if(tmp == NULL) return;

    if(tmp->ds_big_node.size() == 0) {
        //nút lá

        int len = (time_dfs - 1) * 20 + cnt_small_node * 34;
        int begin = 720 - len / 2;

        int d = 0;

        for(int i = 0; i < tmp->ds_small_node.size(); i++) {
            if(tmp->ds_small_node[i] != block) {

                tmp->ds_small_node[i]->nx_pos_x = begin + tmp->num_big_node_left * 20 + (tmp->num_small_left + d) * 34 + 17;

                tmp->ds_small_node[i]->nx_pos_y = 120 + 70 * (hi - 1);
                d++;
            }
        }
    }
    else {

        for(int i = 0; i < tmp->ds_big_node.size(); i++) {
            set_new_postition(tmp->ds_big_node[i], block, hi + 1);
        }

        int len = 34 * tmp->ds_small_node.size();

        int mid = (dfs_minn(tmp) + dfs_maxx(tmp)) / 2;

        int begin = mid - len / 2;

        int d = 0;

        for(int i = 0; i < tmp->ds_small_node.size(); i++) {
            if(tmp->ds_small_node[i] != block) {

                tmp->ds_small_node[i]->nx_pos_x = begin + d * 34 + 17;

                tmp->ds_small_node[i]->nx_pos_y = 120 + 70 * (hi - 1);
                d++;
            }
        }

        //cạnh
        for(int i = 0; i < tmp->ds_edge.size(); i++) {
            tmp->ds_edge[i]->nx_st_x = begin + 34 * i;
            tmp->ds_edge[i]->nx_st_y = 120 + 70 * (hi - 1) + 15;

            big_node *cur = tmp->ds_big_node[i];
            int sum = 0;
            for(int j = 0; j < cur->ds_small_node.size(); j++) {
                sum += cur->ds_small_node[j]->nx_pos_x;
            }

            sum /= cur->ds_small_node.size();

            tmp->ds_edge[i]->nx_en_x = sum;
            tmp->ds_edge[i]->nx_en_y = 120 + 70 * (hi - 1) + 15 + 40;
        }

    }
    return;
}

void BtreeScreen::set_last_postition(big_node *tmp, small_node *block, int hi) {
    if(tmp == NULL) return;

    if(tmp->ds_big_node.size() == 0) {
        //nút lá

        int len = (time_dfs - 1) * 20 + cnt_small_node * 34;
        int begin = 720 - len / 2;

        int d = 0;

        for(int i = 0; i < tmp->ds_small_node.size(); i++) {
            if(tmp->ds_small_node[i] != block) {

                tmp->ds_small_node[i]->cur_pos_x = tmp->ds_small_node[i]->pre_pos_x = tmp->ds_small_node[i]->nx_pos_x = begin + tmp->num_big_node_left * 20 + (tmp->num_small_left + d) * 34 + 17;

                tmp->ds_small_node[i]->cur_pos_y = tmp->ds_small_node[i]->pre_pos_y = tmp->ds_small_node[i]->nx_pos_y = 120 + 70 * (hi - 1);
                d++;
            }
        }
    }
    else {

        for(int i = 0; i < tmp->ds_big_node.size(); i++) {
            set_last_postition(tmp->ds_big_node[i], block, hi + 1);
        }

        int len = 34 * tmp->ds_small_node.size();

        int mid = (dfs_minn(tmp) + dfs_maxx(tmp)) / 2;

        int begin = mid - len / 2;

        int d = 0;

        for(int i = 0; i < tmp->ds_small_node.size(); i++) {
            if(tmp->ds_small_node[i] != block) {

                tmp->ds_small_node[i]->cur_pos_x = tmp->ds_small_node[i]->pre_pos_x = tmp->ds_small_node[i]->nx_pos_x = begin + d * 34 + 17;

                tmp->ds_small_node[i]->cur_pos_y = tmp->ds_small_node[i]->pre_pos_y = tmp->ds_small_node[i]->nx_pos_y = 120 + 70 * (hi - 1);
                d++;
            }
        }

        //cạnh
        for(int i = 0; i < tmp->ds_edge.size(); i++) {
            tmp->ds_edge[i]->cur_st_x = tmp->ds_edge[i]->pre_st_x = tmp->ds_edge[i]->nx_st_x = begin + 34 * i;
            tmp->ds_edge[i]->cur_st_y = tmp->ds_edge[i]->pre_st_y = tmp->ds_edge[i]->nx_st_y = 120 + 70 * (hi - 1) + 15;

            big_node *cur = tmp->ds_big_node[i];
            int sum = 0;
            for(int j = 0; j < cur->ds_small_node.size(); j++) {
                sum += cur->ds_small_node[j]->nx_pos_x;
            }

            sum /= cur->ds_small_node.size();

            tmp->ds_edge[i]->cur_en_x = tmp->ds_edge[i]->pre_en_x = tmp->ds_edge[i]->nx_en_x = sum;
            tmp->ds_edge[i]->cur_en_y = tmp->ds_edge[i]->pre_en_y = tmp->ds_edge[i]->nx_en_y = 120 + 70 * (hi - 1) + 15 + 40;
        }

    }
    return;
}

void BtreeScreen::dfs_shift_tree(big_node *tmp, float per) {
    if(tmp == NULL) return;
    for(int i = 0; i < tmp->ds_small_node.size(); i++) {
        small_node *cur = tmp->ds_small_node[i];

        cur->cur_pos_x = cur->pre_pos_x + 1.0 * per * (cur->nx_pos_x - cur->pre_pos_x);
        cur->cur_pos_y = cur->pre_pos_y + 1.0 * per * (cur->nx_pos_y - cur->pre_pos_y);
    }

    for(int i = 0; i < tmp->ds_edge.size(); i++) {
        edge *cur = tmp->ds_edge[i];

        cur->cur_st_x = cur->pre_st_x + 1.0 * per * (cur->nx_st_x - cur->pre_st_x);
        cur->cur_st_y = cur->pre_st_y + 1.0 * per * (cur->nx_st_y - cur->pre_st_y);

        cur->cur_en_x = cur->pre_en_x + 1.0 * per * (cur->nx_en_x - cur->pre_en_x);
        cur->cur_en_y = cur->pre_en_y + 1.0 * per * (cur->nx_en_y - cur->pre_en_y);
    }

    for(int i = 0; i < tmp->ds_big_node.size(); i++) {
        dfs_shift_tree(tmp->ds_big_node[i], per);
    }
}

void BtreeScreen::shift_tree_insert(edge *edge_1, edge* edge_2, small_node *node_1, float time) {
    for(int i = 1; i <= 60 * time; i++) {
        if(edge_1 != NULL) {
            edge_1->co_edge.a = 1.0 * i / (60 * time) * 255;
        }
        if(edge_2 != NULL) {
            edge_2->co_edge.a = 1.0 * i / (60 * time) * 255;
        }
        if(node_1 != NULL) {
            node_1->co_outline.a = 1.0 * i / (60 * time) * 255;
            node_1->co_shape.a = 1.0 * i / (60 * time) * 255;
            node_1->co_val.a = 1.0 * i / (60 * time) * 255;
        }
        dfs_shift_tree(root, 1.0 * i / (60 * time));

        btree_state.push_back(copy_root(root));
    }
}

void BtreeScreen::reset_color(big_node *tmp) {
    if(tmp == NULL) return;
    for(int i = 0; i < tmp->ds_small_node.size(); i++) {
        tmp->ds_small_node[i]->co_outline = sf::Color::Black;
        tmp->ds_small_node[i]->co_val = sf::Color::Black;
        tmp->ds_small_node[i]->co_shape = sf::Color::White;
    }

    for(int i = 0; i < tmp->ds_edge.size(); i++) {
        tmp->ds_edge[i]->co_edge = sf::Color::Black;
        tmp->ds_edge[i]->per_process = 0;
    }

    for(int i = 0; i < tmp->ds_big_node.size(); i++) {
        reset_color(tmp->ds_big_node[i]);
    }
}

void BtreeScreen::change_color(std::vector<std::pair<small_node*, int>> &ds_node, std::vector<std::pair<edge*, int>> &ds_edge, float time) {
    std::vector<sf::Color> cur_outline;
    std::vector<sf::Color> cur_val;
    std::vector<sf::Color> cur_shape;

    std::vector<float> cur_edge;


    for(int i = 0; i < ds_node.size(); i++) {
        cur_outline.push_back(ds_node[i].first->co_outline);
        cur_val.push_back(ds_node[i].first->co_val);
        cur_shape.push_back(ds_node[i].first->co_shape);
    }

    for(int i = 0; i < ds_edge.size(); i++) {
        cur_edge.push_back(ds_edge[i].first->per_process);
    }

    for(int i = 1; i <= 60 * time; i++) {
        for(int j = 0; j < ds_edge.size(); j++) {
            ds_edge[j].first->per_process = cur_edge[j] + 1.0 * i / (60 * time) * (ds_edge[j].second - cur_edge[j]);
        }

        for(int j = 0; j < ds_node.size(); j++) {
            small_node *cur = ds_node[j].first;
            if(ds_node[j].second == 0) {

                cur->co_val.r = cur_val[j].r + 1.0 * i / (60 * time) * (0 - cur_val[j].r);
                cur->co_val.g = cur_val[j].g + 1.0 * i / (60 * time) * (0 - cur_val[j].g);
                cur->co_val.b = cur_val[j].b + 1.0 * i / (60 * time) * (0 - cur_val[j].b);

                cur->co_shape.r = cur_shape[j].r + 1.0 * i / (60 * time) * (255 - cur_shape[j].r);
                cur->co_shape.g = cur_shape[j].g + 1.0 * i / (60 * time) * (255 - cur_shape[j].g);
                cur->co_shape.b = cur_shape[j].b + 1.0 * i / (60 * time) * (255 - cur_shape[j].b);

                cur->co_outline.r = cur_outline[j].r + 1.0 * i / (60 * time) * (0 - cur_outline[j].r);
                cur->co_outline.g = cur_outline[j].g + 1.0 * i / (60 * time) * (0 - cur_outline[j].g);
                cur->co_outline.b = cur_outline[j].b + 1.0 * i / (60 * time) * (0 - cur_outline[j].b);
            }
            else if(ds_node[j].second == 1) {
                cur->co_val.r = cur_val[j].r + 1.0 * i / (60 * time) * (255 - cur_val[j].r);
                cur->co_val.g = cur_val[j].g + 1.0 * i / (60 * time) * (165 - cur_val[j].g);
                cur->co_val.b = cur_val[j].b + 1.0 * i / (60 * time) * (0 - cur_val[j].b);

                cur->co_shape.r = cur_shape[j].r + 1.0 * i / (60 * time) * (255 - cur_shape[j].r);
                cur->co_shape.g = cur_shape[j].g + 1.0 * i / (60 * time) * (255 - cur_shape[j].g);
                cur->co_shape.b = cur_shape[j].b + 1.0 * i / (60 * time) * (255 - cur_shape[j].b);

                cur->co_outline.r = cur_outline[j].r + 1.0 * i / (60 * time) * (255 - cur_outline[j].r);
                cur->co_outline.g = cur_outline[j].g + 1.0 * i / (60 * time) * (165 - cur_outline[j].g);
                cur->co_outline.b = cur_outline[j].b + 1.0 * i / (60 * time) * (0 - cur_outline[j].b);
            }
            else if(ds_node[j].second == 2) {
                cur->co_val.r = cur_val[j].r + 1.0 * i / (60 * time) * (255 - cur_val[j].r);
                cur->co_val.g = cur_val[j].g + 1.0 * i / (60 * time) * (255 - cur_val[j].g);
                cur->co_val.b = cur_val[j].b + 1.0 * i / (60 * time) * (255 - cur_val[j].b);

                cur->co_shape.r = cur_shape[j].r + 1.0 * i / (60 * time) * (255 - cur_shape[j].r);
                cur->co_shape.g = cur_shape[j].g + 1.0 * i / (60 * time) * (165 - cur_shape[j].g);
                cur->co_shape.b = cur_shape[j].b + 1.0 * i / (60 * time) * (0 - cur_shape[j].b);

                cur->co_outline.r = cur_outline[j].r + 1.0 * i / (60 * time) * (255 - cur_outline[j].r);
                cur->co_outline.g = cur_outline[j].g + 1.0 * i / (60 * time) * (165 - cur_outline[j].g);
                cur->co_outline.b = cur_outline[j].b + 1.0 * i / (60 * time) * (0 - cur_outline[j].b);
            }
            else if(ds_node[j].second == 3) {
                cur->co_val.r = cur_val[j].r + 1.0 * i / (60 * time) * (255 - cur_val[j].r);
                cur->co_val.g = cur_val[j].g + 1.0 * i / (60 * time) * (255 - cur_val[j].g);
                cur->co_val.b = cur_val[j].b + 1.0 * i / (60 * time) * (255 - cur_val[j].b);

                cur->co_shape.r = cur_shape[j].r + 1.0 * i / (60 * time) * (106 - cur_shape[j].r);
                cur->co_shape.g = cur_shape[j].g + 1.0 * i / (60 * time) * (220 - cur_shape[j].g);
                cur->co_shape.b = cur_shape[j].b + 1.0 * i / (60 * time) * (127 - cur_shape[j].b);

                cur->co_outline.r = cur_outline[j].r + 1.0 * i / (60 * time) * (106 - cur_outline[j].r);
                cur->co_outline.g = cur_outline[j].g + 1.0 * i / (60 * time) * (220 - cur_outline[j].g);
                cur->co_outline.b = cur_outline[j].b + 1.0 * i / (60 * time) * (127 - cur_outline[j].b);
            }
        }
        btree_state.push_back(copy_root(root));
    }
}

bool BtreeScreen::dfs_insert(big_node *tmp, big_node *par, edge *canh, int val) {

    if(tmp == NULL) return 0;

    std::vector<std::pair<small_node*, int>> ds_node;
    std::vector<std::pair<edge*, int>> ds_edge;

    if(par != NULL) {
        //đổi màu cha

        for(int i = 0; i < par->ds_small_node.size(); i++) {
            ds_node.push_back(std::make_pair(par->ds_small_node[i], 1));
        }

        //ds_edge.push_back(std::make_pair(canh, 1));

        change_color(ds_node, ds_edge, 0.3f);
        ds_edge.clear();
        ds_node.clear();
    }


    for(int i = 0; i < tmp->ds_small_node.size(); i++) {
        ds_node.push_back(std::make_pair(tmp->ds_small_node[i], 2));
    }

    if(canh != NULL) ds_edge.push_back(std::make_pair(canh, 1));

    change_color(ds_node, ds_edge, 0.3f);
    ds_edge.clear();
    ds_node.clear();



    if(tmp->ds_big_node.size() == 0) {

        //nút lá
        //=> insert vào đây

        small_node *new_node = new small_node(0, 0, val);

        int pos = 0;
        
        while(pos < tmp->ds_small_node.size() && tmp->ds_small_node[pos]->val < val) pos++;

        if(pos < tmp->ds_small_node.size() && tmp->ds_small_node[pos]->val == val) {

            for(int i = 0; i < tmp->ds_small_node.size(); i++) {
                ds_node.push_back(std::make_pair(tmp->ds_small_node[i], 0));
            }

            if(canh != NULL) ds_edge.push_back(std::make_pair(canh, 0));

            change_color(ds_node, ds_edge, 0.3f);
            ds_edge.clear();
            ds_node.clear();

            return 0;
        }

        tmp->ds_small_node.insert(tmp->ds_small_node.begin() + pos, new_node);

        dfs_time(NULL);

        set_new_postition(root, NULL, 1);

        new_node->pre_pos_x = new_node->cur_pos_x = new_node->nx_pos_x;
        new_node->pre_pos_y = new_node->cur_pos_y = new_node->nx_pos_y;
        new_node->co_outline = sf::Color(255, 165, 0);
        new_node->co_val= sf::Color(255, 255, 255);
        new_node->co_shape = sf::Color(255, 165, 0);


        shift_tree_insert(NULL, NULL, new_node, 0.3f);

        set_last_postition(root, NULL, 1);

        return 1;
    }
    else {

        int pos = 0;
        while(pos < tmp->ds_small_node.size() && tmp->ds_small_node[pos]->val < val) pos++;

        if(pos < tmp->ds_small_node.size() && tmp->ds_small_node[pos]->val == val) {
            for(int i = 0; i < tmp->ds_small_node.size(); i++) {
                ds_node.push_back(std::make_pair(tmp->ds_small_node[i], 0));
            }

            if(canh != NULL) ds_edge.push_back(std::make_pair(canh, 0));

            change_color(ds_node, ds_edge, 0.3f);
            ds_edge.clear();
            ds_node.clear();
            return 0;
        }

        big_node *nx = tmp->ds_big_node[pos];

        //TH nó có 3 giá trị ở nút tiếp theo nó thăm

        if(nx->ds_small_node.size() == 3) {

            small_node *cur = nx->ds_small_node[1];

            tmp->ds_small_node.insert(tmp->ds_small_node.begin() + pos, nx->ds_small_node[1]);

            big_node *B = new big_node;
            B->ds_small_node.push_back(nx->ds_small_node[2]);

            if(nx->ds_edge.size()) {
                B->ds_edge.push_back(nx->ds_edge[2]);
                B->ds_edge.push_back(nx->ds_edge[3]);

                nx->ds_edge.pop_back();
                nx->ds_edge.pop_back();
            }

            if(nx->ds_big_node.size()) {
                B->ds_big_node.push_back(nx->ds_big_node[2]);
                B->ds_big_node.push_back(nx->ds_big_node[3]);

                nx->ds_big_node.pop_back();
                nx->ds_big_node.pop_back();
            }

            nx->ds_small_node.pop_back();
            nx->ds_small_node.pop_back();

            edge *new_edge = new edge;

            tmp->ds_edge.insert(tmp->ds_edge.begin() + pos + 1, new_edge);
            tmp->ds_big_node.insert(tmp->ds_big_node.begin() + pos + 1, B);

            dfs_time(NULL);

            set_new_postition(root, NULL, 1);

            new_edge->cur_st_x = new_edge->pre_st_x = new_edge->nx_st_x;
            new_edge->cur_st_y = new_edge->pre_st_y = new_edge->nx_st_y;


            new_edge->cur_en_x = new_edge->pre_en_x = new_edge->nx_en_x;
            new_edge->cur_en_y = new_edge->pre_en_y = new_edge->nx_en_y;


            shift_tree_insert(new_edge, NULL, NULL, 0.3f);

            ds_node.push_back(std::make_pair(cur, 2));
            change_color(ds_node, ds_edge, 0.1f);
            ds_edge.clear();
            ds_node.clear();

            set_last_postition(root, NULL, 1);
        }


        pos = 0;
        while(pos < tmp->ds_small_node.size() && tmp->ds_small_node[pos]->val < val) pos++;

        if(pos < tmp->ds_small_node.size() && tmp->ds_small_node[pos]->val == val) {
            for(int i = 0; i < tmp->ds_small_node.size(); i++) {
                ds_node.push_back(std::make_pair(tmp->ds_small_node[i], 0));
            }

            if(canh != NULL) ds_edge.push_back(std::make_pair(canh, 0));

            change_color(ds_node, ds_edge, 0.3f);
            ds_edge.clear();
            ds_node.clear();
            return 0;
        }

        int ok = dfs_insert(tmp->ds_big_node[pos], tmp, tmp->ds_edge[pos], val);

        if(ok == 0) {
            for(int i = 0; i < tmp->ds_small_node.size(); i++) {
                ds_node.push_back(std::make_pair(tmp->ds_small_node[i], 0));
            }

            if(canh != NULL) ds_edge.push_back(std::make_pair(canh, 0));

            change_color(ds_node, ds_edge, 0.3f);
            ds_edge.clear();
            ds_node.clear();
            return 0;
        }

        return ok;

    }
}


void BtreeScreen::insert_btree(int val) {
    reset_color(root);
    btree_state.push_back(copy_root(root));
    if(root == NULL) {


        root = new big_node;

        small_node *cur = new small_node(0, 0, val);

        root->ds_small_node.push_back(cur);

        dfs_time(NULL);

        set_new_postition(root, NULL, 1);

        cur->cur_pos_x = cur->pre_pos_x = cur->nx_pos_x;
        cur->cur_pos_y = cur->pre_pos_y = cur->nx_pos_y;

        shift_tree_insert(NULL, NULL, cur, 0.3f);

        set_last_postition(root, NULL, 1);
    }
    else {

        //xử lí TH mà đỉnh node nó có 3 key


        if(root->ds_small_node.size() == 3) {

            big_node *A = new big_node;

            big_node *B = new big_node;

            A->ds_small_node.push_back(root->ds_small_node[1]);

            B->ds_small_node.push_back(root->ds_small_node[2]);

            root->ds_small_node.pop_back();
            root->ds_small_node.pop_back();

            if(root->ds_edge.size()) {
                B->ds_edge.push_back(root->ds_edge[2]);
                B->ds_edge.push_back(root->ds_edge[3]);

                root->ds_edge.pop_back();
                root->ds_edge.pop_back();
            }

            edge *edge_1 = new edge;
            edge *edge_2 = new edge;

            A->ds_edge.push_back(edge_1);
            A->ds_edge.push_back(edge_2);

            if(root->ds_big_node.size()) {
                B->ds_big_node.push_back(root->ds_big_node[2]);
                B->ds_big_node.push_back(root->ds_big_node[3]);

                root->ds_big_node.pop_back();
                root->ds_big_node.pop_back();
            }

            A->ds_big_node.push_back(root);
            A->ds_big_node.push_back(B);

            root = A;


            dfs_time(NULL);



            set_new_postition(root, NULL, 1);

            edge_1->cur_st_x = edge_1->pre_st_x = edge_1->nx_st_x;
            edge_1->cur_st_y = edge_1->pre_st_y = edge_1->nx_st_y;


            edge_1->cur_en_x = edge_1->pre_en_x = edge_1->nx_en_x;
            edge_1->cur_en_y = edge_1->pre_en_y = edge_1->nx_en_y;

            edge_2->cur_st_x = edge_2->pre_st_x = edge_2->nx_st_x;
            edge_2->cur_st_y = edge_2->pre_st_y = edge_2->nx_st_y;


            edge_2->cur_en_x = edge_2->pre_en_x = edge_2->nx_en_x;
            edge_2->cur_en_y = edge_2->pre_en_y = edge_2->nx_en_y;


            shift_tree_insert(edge_1, edge_2, NULL, 0.3f);

            set_last_postition(root, NULL, 1);
        }


        //tiếp tục làm như bình thường

        dfs_insert(root, NULL, NULL, val);

    }
}

bool BtreeScreen::dfs_find(big_node *tmp, big_node *par, edge *canh, int val) {
    if(tmp == NULL) return 0;

    std::vector<std::pair<small_node*, int>> ds_node;
    std::vector<std::pair<edge*, int>> ds_edge;

    if(par != NULL) {
        for(int i = 0; i < par->ds_small_node.size(); i++) {
            ds_node.push_back(std::make_pair(par->ds_small_node[i], 1));
        }

        change_color(ds_node, ds_edge, 0.3f);
        ds_node.clear();
        ds_edge.clear();
    }

    for(int i = 0; i < tmp->ds_small_node.size(); i++) {
        ds_node.push_back(std::make_pair(tmp->ds_small_node[i], 2));
    }

    if(canh != NULL) ds_edge.push_back(std::make_pair(canh, 1));

    change_color(ds_node, ds_edge, 0.3f);
    ds_node.clear();
    ds_edge.clear();

    int pos = 0;
    while(pos < tmp->ds_small_node.size() && tmp->ds_small_node[pos]->val < val) pos++;

    if(pos < tmp->ds_small_node.size() && tmp->ds_small_node[pos]->val == val) {

        ds_node.push_back(std::make_pair(tmp->ds_small_node[pos], 3));
        change_color(ds_node, ds_edge, 0.3f);
        ds_node.clear();

        return 1;
    }
    else {
        if(tmp->ds_big_node.size() == 0) {
            for(int i = 0; i < tmp->ds_small_node.size(); i++) {
                ds_node.push_back(std::make_pair(tmp->ds_small_node[i], 0));
            }

            if(canh != NULL) ds_edge.push_back(std::make_pair(canh, 0));

            change_color(ds_node, ds_edge, 0.3f);
            ds_node.clear();
            ds_edge.clear();

            return 0;
        }
        else {

            int ok = dfs_find(tmp->ds_big_node[pos], tmp, tmp->ds_edge[pos], val);

            if(ok == 0) {
                for(int i = 0; i < tmp->ds_small_node.size(); i++) {
                    ds_node.push_back(std::make_pair(tmp->ds_small_node[i], 0));
                }

                if(canh != NULL) ds_edge.push_back(std::make_pair(canh, 0));

                change_color(ds_node, ds_edge, 0.3f);
                ds_node.clear();
                ds_edge.clear();
            }
            return ok;
        }
    }
}


void BtreeScreen::search_btree(int val) {
    reset_color(root);
    btree_state.push_back(copy_root(root));

    if(root == NULL) return;

    dfs_find(root, NULL, NULL, val);


}



void BtreeScreen::run() {
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



    while(mWindow.isOpen()) {
        ProcessEvent();
        Update();
        if(Return  == 1) {
            reset_state();
            reset_root();
            root = NULL;
            cur_state = 0;
            break;
        }
        Render();
    }
}

void BtreeScreen::ProcessEvent() {
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
            }
        }
        if(event.type == sf::Event::MouseButtonReleased) 
        {
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


void BtreeScreen::Update() {

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
        modify_input(s, 1);
    }
    else if(ds[21].disable == 0 && ds[21].clicked == 1) {
        std::string &s = ds[21].text;
        if(back_space == 1) {
            if(s.size()) s.pop_back();
        }
        else if(so >= 0 && so < 10) {
            s += char(so + '0');
        }
        modify_input(s, 2);
    }
    else if(ds[23].disable == 0 && ds[23].clicked == 1) {
        std::string &s = ds[23].text;
        if(back_space == 1) {
            if(s.size()) s.pop_back();
        }
        else if(so >= 0 && so < 10) {
            s += char(so + '0');
        }
        modify_input(s, 2);
    }
    else if(ds[25].disable == 0 && ds[25].clicked == 1) {
        std::string &s = ds[25].text;
        if(back_space == 1) {
            if(s.size()) s.pop_back();
        }
        else if(so >= 0 && so < 10) {
            s += char(so + '0');
        }
        modify_input(s, 2);
    }

    so = 10;
    back_space = 0;

    //random n create
    if(ds[18].disable == 0 && ds[18].clicked == 1) {
        int tmp = rand_3(1, 30);
        ds[17].text = std::to_string(tmp);
        ds[18].clicked = 0;
    }

    //go create
    if(ds[19].disable == 0 && ds[19].clicked == 1) {
        reset_state();
        reset_root();
        btree_state.push_back(copy_root(root));
        cur_state = 0;
        root = NULL;
        int n = 0;
        for(int i = 0; i < ds[17].text.size(); i++) {
            n = n * 10 + ds[17].text[i] - '0';
        }

        bool *ok = new bool[100];
        for(int i = 0; i < 100; i++) ok[i] = 0;

        while(n) {
            int tmp = rand_3(1, 99);
            if(ok[tmp] == 0) {
                n--;
                ok[tmp] = 1;
                insert_btree(tmp);
            }
        }

        delete[]ok;
        ds[19].clicked = 0;
        is_play = 1;
        ds[13].disable = 0;
        ds[12].disable = 1;
        ds[11].disable = 1;
    }

    //file create
    if(ds[20].disable == 0 && ds[20].clicked == 1) {
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

            std::vector<int>a;
            int m = 0;

            if(fin.is_open()) {
                std::string s;
                while(getline(fin, s)) {
                    int tmp = 0;
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
                }
            }

            reset_state();
            reset_root();
            btree_state.push_back(copy_root(root));
            cur_state = 0;
            root = NULL;

            for(int i = 0; i < a.size(); i++) {
                insert_btree(a[i]);
            }


            fin.close();
        }

        ds[20].clicked = 0;
        is_play = 1;
        ds[13].disable = 0;
        ds[12].disable = 1;
        ds[11].disable = 1;
    }

    //go insert
    if(ds[22].clicked && ds[22].disable == 0) {
        reset_state();
        cur_state = 0;
        int tmp = 0;
        for(int i = 0; i < ds[21].text.size(); i++) {
            tmp = tmp * 10 + ds[21].text[i] - '0';
        }
        insert_btree(tmp);
        ds[22].clicked = 0;
        is_play = 1;
        ds[13].disable = 0;
        ds[12].disable = 1;
        ds[11].disable = 1;
    }

    //go search 
    if(ds[24].clicked && ds[24].disable == 0) {
        reset_state();
        cur_state = 0;
        int tmp = 0;
        for(int i = 0; i < ds[23].text.size(); i++) {
            tmp = tmp * 10 + ds[23].text[i] - '0';
        }
        search_btree(tmp);
        ds[24].clicked = 0;
        is_play = 1;
        ds[13].disable = 0;
        ds[12].disable = 1;
        ds[11].disable = 1;
    }


    /*
    //go delete
    if(ds[26].clicked && ds[26].disable == 0) {
        reset_color(root);
        clear_state();
        avl_state.push_back(copy_root(root));
        cur_state = 0;
        int tmp = 0;
        for(int i = 0; i < ds[25].text.size(); i++) {
            tmp = tmp * 10 + ds[25].text[i] - '0';
        }
        delete_avl(tmp);
        ds[26].clicked = 0;
        is_play = 1;
        ds[13].disable = 0;
        ds[12].disable = 1;
        ds[11].disable = 1;
    }
    */


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
        if(cur_state >= btree_state.size()) cur_state = btree_state.size() - 1;
        is_play = 0;
        ds[12].disable = 0;
        ds[13].disable = 1;
        ds[11].disable = 1;
    }

    //full_go
    if(ds[15].clicked) {
        ds[15].clicked = 0;
        cur_state = btree_state.size() - 1;
        is_play = 0;
        ds[12].disable = 0;
        ds[13].disable = 1;
        ds[11].disable = 1;
    }



}


void BtreeScreen::modify_input(std::string &s, int t) {
    if(t == 1) {
        //n
        int tmp = 0;
        for(int i = 0; i < s.size(); i++) {
            tmp = tmp * 10 + s[i] - '0';
        }
        if(tmp > 30) tmp = 30;
        s = std::to_string(tmp);
    }
    else {
        //val
        int tmp = 0;
        for(int i = 0; i < s.size(); i++) {
            tmp = tmp * 10 + s[i] - '0';
        }
        if(tmp > 99) tmp = 99;
        s = std::to_string(tmp);
    }
}


void BtreeScreen::render_image() {
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

void BtreeScreen::Render() {
    mWindow.clear();
    render_image();

    
    cur_state = std::min(cur_state, (int)btree_state.size() - 1);
    if(cur_state >= 0 && cur_state < btree_state.size()) {
        draw_btree(btree_state[cur_state]);
        draw_time_bar();
    }



    mWindow.display();
    
    cur_state = cur_state + is_play * (int)speed;
    cur_state = std::min(cur_state, (int)btree_state.size() - 1);
    if(cur_state == btree_state.size() - 1) {
        is_play = 0;
        ds[12].disable = ds[13].disable = 1;
        ds[11].disable = 0;
    }
}







// Hàm để tính chiều dài của đường thẳng
float BtreeScreen::length(const sf::Vector2f& start, const sf::Vector2f& end) {
    // Tính hiệu giữa tọa độ x và y của hai điểm
    float dx = end.x - start.x;
    float dy = end.y - start.y;
    
    // Tính chiều dài theo định lý Pythagoras
    return std::sqrt(dx * dx + dy * dy);
}

// Hàm để tính góc của đường thẳng (tính bằng radian)
float BtreeScreen::angle(const sf::Vector2f& start, const sf::Vector2f& end) {
    // Tính hiệu giữa tọa độ x và y của hai điểm
    float dy = end.y - start.y;
    float dx = end.x - start.x;
    
    // Tính góc theo hàm atan2
    return std::atan2(dy, dx) * 180 / 3.14159265f;
}

void BtreeScreen::draw_btree(big_node *tmp) {
    if(tmp == NULL) return;

    for(int i = 0; i < tmp->ds_edge.size(); i++) {
        draw_edge(tmp->ds_edge[i]);
    }

    for(int i = 0; i < tmp->ds_big_node.size(); i++) {
        draw_btree(tmp->ds_big_node[i]);
    }

    for(int i = 0; i < tmp->ds_small_node.size(); i++) {
        draw_node(tmp->ds_small_node[i]);
    }
}

void BtreeScreen::draw_edge(edge *tmp) {


    //Vẽ cạnh

    sf::Vector2f start(tmp->cur_st_x, tmp->cur_st_y);
    sf::Vector2f end(tmp->cur_en_x, tmp->cur_en_y);



    //Cạnh là 1 rectangle
    sf::RectangleShape Line;
    //độ dày
    float thickness = 3.5f;
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

    len = len * tmp->per_process;
    if(len > 0) {
        // Thiết lập kích thước và góc cho hình chữ nhật
        Line.setSize(sf::Vector2f(len, thickness));
        Line.setFillColor(sf::Color(255, 165, 0));
        Line.setOrigin(0, thickness / 2); // Đặt tâm hình chữ nhật để xoay

        // Đặt vị trí và góc cho hình chữ nhật
        Line.setPosition(start);
        Line.setRotation(angle(start, end)); // Chuyển đổi từ radian sang độ

        mWindow.draw(Line);
    } 
}

void BtreeScreen::draw_node(small_node *tmp) {
    if(tmp == NULL) return;

    // Vẽ node hiện tại
    sf::RectangleShape square(sf::Vector2f(30.0f, 30.0f));
    square.setOutlineThickness(3);
    square.setOrigin(15, 15);
    square.setPosition(tmp->cur_pos_x, tmp->cur_pos_y);
    square.setOutlineColor(tmp->co_outline);
    square.setFillColor(tmp->co_shape);


    //Vẽ giá trị
    sf::Text Value;
    Value.setFont(MyFont.get(FontID::ptsan));
    Value.setString(std::to_string(tmp -> val));
    Value.setCharacterSize(16);
    Value.setFillColor(tmp -> co_val);

    sf::FloatRect textRect = Value.getLocalBounds();
    Value.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top  + textRect.height / 2.0f);
    Value.setPosition(tmp -> cur_pos_x, tmp -> cur_pos_y);


    mWindow.draw(square);
    mWindow.draw(Value);

}






void BtreeScreen::draw_time_bar() {


    //rgb(116,255,188)
    float per = 1.0 * (cur_state + 1) / (int)btree_state.size();

    sf::RectangleShape Line;

    Line.setSize(sf::Vector2f(500.0f * per, 15));
    Line.setFillColor(sf::Color(116, 225, 188));

    // Đặt vị trí và góc cho hình chữ nhật
    Line.setPosition(468, 864);

    mWindow.draw(Line);

}







