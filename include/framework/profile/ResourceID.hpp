#ifndef RESOURCE_ID_HPP
#define RESOURCE_ID_HPP

#include <ResourceHolder.hpp>

enum class TextureID 
{
    nen
    //enumerator
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