
#include <SFML/Graphics.hpp>
#include <time.h>

using namespace sf;

int main() {

    RenderWindow window(VideoMode(320, 480), "Tetris");

    while (window.isOpen()){
        Event event{};
        while (window.pollEvent(event)){
            if (event.type == Event::Closed){
                window.close();
            }
        }
        window.clear(Color::Green);
        window.display();
    }

    return 0;
}
