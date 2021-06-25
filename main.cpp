#include <iostream>
#include <SFML/Graphics.hpp>
#include <time.h>
#include <string>
using namespace sf;


const std::string TILE_TEXTURE_PATH = "tiles.png";
const int TILE_PIXEL_SIZE = 18;

struct Point {
    int x;
    int y;
};

//There are 7 Tetris blocks (called Tetriminos) and each have 4 blocks and a name
//Each number represents a filled in block in a 2x4 array
const int TERTRIMINOS[7][4] = {
        {0,2,4,6}, //I
        {1,3,4,5}, //J
        {0,2,4,5}, //L
        {0,1,2,3}, //O
        {0,2,3,5}, //S
        {0,2,3,4}, //T
        {1,2,3,4}  //Z
};

int main() {

    RenderWindow window(VideoMode(320, 480), "Tetris");

    Texture texture;
    texture.loadFromFile(TILE_TEXTURE_PATH);
    texture.setSmooth(true);
    Point shapeTileCoordinates[4];
    Sprite sprite(texture);
    //Left X, Top Y, Num X pixels to show, Num Y pixels to show
    sprite.setTextureRect(IntRect(0,0,TILE_PIXEL_SIZE,TILE_PIXEL_SIZE));

    while (window.isOpen()){
        Event event{};
        while (window.pollEvent(event)){
            if (event.type == Event::Closed){
                window.close();
            }
            int n=0;
            for (int i=0;i<4;i++){
                //Max of 2 Tiles on x-axis
                shapeTileCoordinates[i].x = TERTRIMINOS[n][i] % 2;
                //Max of 4 Tiles on y-axis
                shapeTileCoordinates[i].y = TERTRIMINOS[n][i] / 2;
            }
        }

        window.clear(Color::White);
        for (int i = 0; i < 4; i++){
            sprite.setPosition(shapeTileCoordinates[i].x * TILE_PIXEL_SIZE,
                               shapeTileCoordinates[i].y * TILE_PIXEL_SIZE);
            window.draw(sprite);
        }
        window.display();
    }

    return 0;
}
