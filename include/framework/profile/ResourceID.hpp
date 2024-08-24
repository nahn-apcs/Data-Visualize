#ifndef RESOURCE_ID_HPP
#define RESOURCE_ID_HPP

#include <ResourceHolder.hpp>

enum class TextureID 
{
    menu_bg = 0,
    hash = 1,
    avl = 2,
    btree = 3,
    heap = 4,
    trie = 5,
    graph = 6,
    border_hash = 7,
    border_avl = 8,
    border_btree = 9,
    border_heap = 10,
    border_trie = 11,
    border_graph = 12,
    //Menu Screen

    app_bg = 13,
    back_menu_button = 14,
    cuc = 15,
    go_begin_disable = 16,
    go_begin_white = 17,
    go_end_disable = 18,
    go_end_white = 19,
    pause_button = 20,
    play_button = 21,
    return_button = 22,
    speed_bar = 23,
    step_backward_disable = 24,
    step_backward_white = 25,
    step_forward_disable = 26,
    step_forward_white = 27,
    time_bar = 28,
    tool_bar = 29,
    create_button = 30,
    create_button_in = 31,
    delete_button = 32,
    delete_button_in = 33,
    extractmax_button = 34,
    extractmax_button_in = 35,
    insert_button = 36,
    insert_button_in = 37,
    update_button = 38,
    update_button_in = 39,
    file_button_in = 40,
    n_input = 41,
    random_button = 42,
    file_button = 43,
    go_button = 44,
    go_button_in = 45,
    val_input = 46,
    i_input = 47,
    search_button = 48,
    search_button_in = 49,
    m_input = 50,
    s_input = 51,
    //App Screen
};

enum class ImageID 
{
    //enumerator
};

enum class FontID
{
    arial_bold,
    arial,
    consolas,
    ptsan,
    //enumerator
};

enum class SoundBufferID
{
    //enumerator
};

enum class MusicID
{
    //enumerator
};

typedef ResourceHolder<sf::Texture, TextureID> TextureHolder;
typedef ResourceHolder<sf::Image, ImageID> ImageHolder;
typedef ResourceHolder<sf::Font, FontID> FontHolder;
typedef ResourceHolder<sf::SoundBuffer, SoundBufferID> SoundBufferHolder;



#endif //RESOURCE_ID_HPP