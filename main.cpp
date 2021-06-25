#include <iostream>
#include <SFML/Graphics.hpp>
#include <time.h>
#include <string>
using namespace sf;


const std::string kTileTextureFilePath = "tiles.png";
const int kTilePixelSize = 18;
const int kFieldWidth = 10;
const int kFieldHeight= 20;



struct Point {
    int x;
    int y;
} shape_tile_coordinates[4];

//There are 7 Tetris blocks (called Tetriminos) and each have 4 blocks and a name
//Each number represents a filled in block in a 2x4 array
const int kTertriminos[7][4] = {
        {1,3,5,7}, //I
        {3,5,6,7}, //J
        {2,4,6,7}, //L
        {2,3,4,5}, //O
        {2,4,5,7}, //S
        {2,4,5,6}, //T
        {3,4,5,6}  //Z
};

int main() {

    RenderWindow window(VideoMode(320, 480), "Tetris");

    Texture texture;
    texture.loadFromFile(kTileTextureFilePath);
    texture.setSmooth(true);

    Sprite sprite(texture);
    //Left X, Top Y, Num X pixels to show, Num Y pixels to show
    sprite.setTextureRect(IntRect(0, 0, kTilePixelSize, kTilePixelSize));

    Clock clock;

    int dx = 0;
    bool rotate = false;
    int color_num = 1;
    float timer = 0;
    float fall_delay = 0.3;

    while (window.isOpen()) {
        window.clear(Color::White);

        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        Event event{};
        while (window.pollEvent(event)){
            if (event.type == Event::Closed) {
                window.close();
            }
            if (event.type == Event::KeyPressed) {
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
        for (int i = 0; i < 4; i++) {
            shape_tile_coordinates[i].x += dx;
        }

        //Rotation
        if (rotate) {
            Point centerOfShape = shape_tile_coordinates[1];
            for (int i = 0; i < 4; i++) {
                //TODO
                // Rename the next 2 variables to fit style guide
                int yDistanceToCenter = shape_tile_coordinates[i].y - centerOfShape.y;
                int xDistanceToCenter = shape_tile_coordinates[i].x - centerOfShape.x;
                shape_tile_coordinates[i].x = centerOfShape.x - yDistanceToCenter;
                shape_tile_coordinates[i].y = centerOfShape.y + xDistanceToCenter;
            }
        }

        //Falling animation
        if (timer > fall_delay) {
            for (int i = 0; i < 4; i++) {
                shape_tile_coordinates[i].y += 1;
            }
            timer = 0;
        }

        int n=2;
        if (shape_tile_coordinates[0].y == 0) {
            for (int i = 0; i < 4; i++) {
                //Max of 2 Tiles on x-axis
                shape_tile_coordinates[i].x = kTertriminos[n][i] % 2;
                //Max of 4 Tiles on y-axis
                shape_tile_coordinates[i].y = kTertriminos[n][i] / 2;
            }
        }

        dx = 0;
        rotate = false;

        for (int i = 0; i < 4; i++) {
            sprite.setPosition(shape_tile_coordinates[i].x * kTilePixelSize,
                               shape_tile_coordinates[i].y * kTilePixelSize);
            window.draw(sprite);
        }
        window.display();
    }

    return 0;
}
