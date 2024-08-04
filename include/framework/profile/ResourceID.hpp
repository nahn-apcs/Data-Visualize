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
    //App Screen
};

enum class ImageID 
{
    //enumerator
};

enum class FontID
{
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