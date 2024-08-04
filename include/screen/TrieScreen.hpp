#ifndef TRIE_SCREEN_HPP
#define TRIE_SCREEN_HPP

#include <SFML/Graphics.hpp>
#include <ResourceID.hpp>

class TrieScreen 
{
public:
    TrieScreen(sf::RenderWindow &parWindow, TextureHolder &parTexture): mWindow(parWindow), MyTexture(parTexture) {}
    void            run();

private:
    sf::RenderWindow &mWindow;
    TextureHolder &MyTexture;
    void            ProcessEvent();
    void            Update();
    void            Render();
    struct nhan {
        int pos_x;
        int pos_y;
        TextureID a;

        nhan(TextureID _a, int _pos_x = 0, int _pos_y = 0): a(_a), pos_x(_pos_x), pos_y(_pos_y) {}
    };
    std::vector<nhan> ds;
    
};



#endif //TRIE_SCREEN_HPP