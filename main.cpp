#include <iostream>
#include <SFML/Graphics.hpp>
#include <time.h>
#include <string>
using namespace sf;


const std::string TILE_TEXTURE_PATH = "tiles.png";
const int TILE_PIXEL_SIZE = 18;
const int FIELD_WIDTH = 10;
const int FIELD_HEIGHT = 20;



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

    int dx = 0;
    bool rotate = false;
    int colorNum = 1;

    while (window.isOpen()){
        window.clear(Color::White);

        Event event{};
        while (window.pollEvent(event)){
            if (event.type == Event::Closed){
                window.close();
            }
            if (event.type == Event::KeyPressed){
                switch (event.key.code) {
                    case Keyboard::Up:
                        rotate = true;
                        break;
                    case Keyboard::Left:
                        dx = -1;
                        break;
                    case Keyboard::Right:
                        dx = 1;
                        break;
                    default:
                        break;
                }
            }

        }

        //Movement
        for (int i = 0; i < 4; i++){
            shapeTileCoordinates[i].x += dx;
        }

        //Rotation
        if (rotate){
            Point centerOfShape = shapeTileCoordinates[1];
            for (int i = 0; i < 4; i++){
                int yDistanceToCenter = shapeTileCoordinates[i].y - centerOfShape.y;
                int xDistanceToCenter = shapeTileCoordinates[i].x - centerOfShape.x;
                shapeTileCoordinates[i].x = centerOfShape.x - yDistanceToCenter;
                shapeTileCoordinates[i].y = centerOfShape.y + xDistanceToCenter;
            }
        }


        int n=2;
        if (shapeTileCoordinates[0].x == 0) {

            for (int i = 0; i < 4; i++) {
                //Max of 2 Tiles on x-axis
                shapeTileCoordinates[i].x = TERTRIMINOS[n][i] % 2;
                //Max of 4 Tiles on y-axis
                shapeTileCoordinates[i].y = TERTRIMINOS[n][i] / 2;
            }
        }

        dx = 0;
        rotate = false;

        for (int i = 0; i < 4; i++){
            sprite.setPosition(shapeTileCoordinates[i].x * TILE_PIXEL_SIZE,
                               shapeTileCoordinates[i].y * TILE_PIXEL_SIZE);
            window.draw(sprite);
        }
        window.display();
    }

    return 0;
}
