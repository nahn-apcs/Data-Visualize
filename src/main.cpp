#include <SFML/Graphics.hpp>
#include <begin_button.hpp>


int main() {
    // Tạo một cửa sổ
    sf::RenderWindow window(sf::VideoMode(1440, 900), "Data Structure Visualization");

    Begin_Button nen("res/background/start_background.png", 0, 0);
    Begin_Button hash_button("res/background/hash_button.png", 122, 216);
    Begin_Button avl_button("res/background/avl_button.png", 570, 216);
    Begin_Button btree_button("res/background/234_tree_button.png", 1018, 216);
    Begin_Button graph_button("res/background/graph_button.png", 1018, 560);
    Begin_Button heap_button("res/background/heap_button.png", 122, 560);
    Begin_Button trie_button("res/background/trie_button.png", 570, 560);


    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Xóa cửa sổ với màu đen
        window.clear(sf::Color::Black);

        // Vẽ đối tượng myObject lên cửa sổ
        nen.draw(window);
        hash_button.draw(window);
        avl_button.draw(window);
        btree_button.draw(window);
        heap_button.draw(window);
        trie_button.draw(window);
        graph_button.draw(window);


        // Hiển thị nội dung cửa sổ
        window.display();
    }

    return 0;



}