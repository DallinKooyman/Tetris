#include <iostream>
#include <SFML/Graphics.hpp>
#include <ctime>
#include <string>
#include <cstdlib>

using namespace sf;


const std::string kTileTextureFilePath = "tiles.png";
const std::string kBackgroujndTextureFilePath = "background.png";
const std::string kFrameTextureFilePath = "frame.png";

const int kMaxTiles = 4;
const int kTotalPieces = 7;
const int kTilePixelSize = 18;
const int kFieldWidth = 10;
const int kFieldHeight = 20;
const int kNumColors = 7;

//There are 7 Tetris blocks (called Tetriminos), each have 4 blocks and a name
//Each number represents a filled in block in a 2x4 array
const int kTertriminos[kTotalPieces][kMaxTiles] = {
        {1, 3, 5, 7}, //I
        {3, 5, 6, 7}, //J
        {2, 4, 6, 7}, //L
        {2, 3, 4, 5}, //O
        {2, 4, 5, 7}, //S
        {2, 4, 5, 6}, //T
        {3, 4, 5, 6}  //Z
};


struct Point {
    int x;
    int y;
} shape_tile_coordinates[kMaxTiles], overflow_shape_tile_coordinates[kMaxTiles];


// Checks to make sure the piece is on the board and that no piece is already
// in the position. Assuming both are true the function returns true and allows
// the piece to enter that position
// Input is just the playable_area which contains all positions that a piece
// can be in as well as the position of all previous pieces
bool check(int playable_area[kFieldHeight][kFieldWidth]) {
  for (int i = 0; i < kMaxTiles; ++i) {
    if (shape_tile_coordinates[i].x < 0 ||
        shape_tile_coordinates[i].x >= kFieldWidth ||
        shape_tile_coordinates[i].y >= kFieldHeight) {
      return false;
    } else if (playable_area[shape_tile_coordinates[i].y]
                            [shape_tile_coordinates[i].x]) {
      return false;
    }
  }
  return true;
}

int main() {

  srand(time(0));

  RenderWindow window(VideoMode(320, 480), "Tetris");

  Texture tile_texture;
  tile_texture.loadFromFile(kTileTextureFilePath);
  tile_texture.setSmooth(true);

  Texture background_texture;
  background_texture.loadFromFile(kBackgroujndTextureFilePath);
  background_texture.setSmooth(true);

  Texture frame_texture;
  frame_texture.loadFromFile(kFrameTextureFilePath);
  frame_texture.setSmooth(true);

  Sprite tile_sprite(tile_texture);
  Sprite background_sprite(background_texture);
  Sprite frame_sprite(frame_texture);

  Clock clock;

  int dx = 0;
  bool rotate = false;
  int color_num = 1 + rand() % kNumColors;
  int next_shape_index = rand() % kTotalPieces;
  float timer = 0;
  float fall_delay = 0.3;

  int playable_area[kFieldHeight][kFieldWidth] = {0};

  for (int i = 0; i < kMaxTiles; i++) {
    //Max of 2 Tiles on x-axis
    shape_tile_coordinates[i].x = kTertriminos[next_shape_index][i] % 2;
    //Max of 4 Tiles on y-axis
    shape_tile_coordinates[i].y = kTertriminos[next_shape_index][i] / 2;
  }

  while (window.isOpen()) {
    float time = clock.getElapsedTime().asSeconds();
    clock.restart();
    timer += time;

    Event event{};
    while (window.pollEvent(event)) {
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

    if (Keyboard::isKeyPressed(Keyboard::Down)) {
      fall_delay = 0.05;
    }

    //Movement
    for (int i = 0; i < kMaxTiles; i++) {
      overflow_shape_tile_coordinates[i] = shape_tile_coordinates[i];
      shape_tile_coordinates[i].x += dx;
    }
    if (!check(playable_area)){
      for (int i = 0; i < kMaxTiles; ++i) {
        shape_tile_coordinates[i] = overflow_shape_tile_coordinates[i];
      }
    }

    //Rotation
    if (rotate) {
      Point centerOfShape = shape_tile_coordinates[1];
      for (int i = 0; i < kMaxTiles; i++) {
        //TODO
        // Rename the next 2 variables to fit style guide
        int yDistanceToCenter = shape_tile_coordinates[i].y - centerOfShape.y;
        int xDistanceToCenter = shape_tile_coordinates[i].x - centerOfShape.x;
        shape_tile_coordinates[i].x = centerOfShape.x - yDistanceToCenter;
        shape_tile_coordinates[i].y = centerOfShape.y + xDistanceToCenter;
      }
      if (!check(playable_area)){
        for (int i = 0; i < kMaxTiles; ++i) {
          shape_tile_coordinates[i] = overflow_shape_tile_coordinates[i];
        }
      }
    }

    //Falling animation
    if (timer > fall_delay) {
      for (int i = 0; i < kMaxTiles; i++) {
        overflow_shape_tile_coordinates[i] = shape_tile_coordinates[i];
        shape_tile_coordinates[i].y += 1;
      }
      if (!check(playable_area)){
        for (int i = 0; i < kMaxTiles; ++i) {
          //TODO fix next line, maybe rename stuff
          playable_area[overflow_shape_tile_coordinates[i].y][overflow_shape_tile_coordinates[i].x]=color_num;
        }
        color_num = 1 + rand() % kNumColors;
        next_shape_index = rand() % kTotalPieces;
        for (int i = 0; i < kMaxTiles; i++) {
          //Max of 2 Tiles on x-axis
          shape_tile_coordinates[i].x = kTertriminos[next_shape_index][i] % 2;
          //Max of 4 Tiles on y-axis
          shape_tile_coordinates[i].y = kTertriminos[next_shape_index][i] / 2;
        }
      }
      timer = 0;
    }

    //Check for full line
    int k = kFieldHeight - 1;
    for (int i = kFieldHeight - 1; i > 0; --i) {
      int count = 0;
      for (int j = 0; j < kFieldWidth; ++j) {
        if (playable_area[i][j]) {
          count++;
        }
        playable_area[k][j] = playable_area[i][j];
      }
      if (count < kFieldWidth) {
        k--;
      }
    }




    dx = 0;
    rotate = false;
    fall_delay = 0.3;

    //Create board
    window.clear(Color::White);
    window.draw(background_sprite);

    for (int i = 0; i < kFieldHeight; ++i) {
      for (int j = 0; j < kFieldWidth; ++j) {
        if (playable_area[i][j] == 0){
          continue;
        }
        tile_sprite.setTextureRect(IntRect(playable_area[i][j] * kTilePixelSize, 0,
                                           kTilePixelSize, kTilePixelSize));
        tile_sprite.setPosition(j * kTilePixelSize, i * kTilePixelSize);
        tile_sprite.move(28,31);
        window.draw(tile_sprite);
      }
    }

    for (int i = 0; i < kMaxTiles; i++) {
      tile_sprite.setTextureRect(IntRect(color_num * kTilePixelSize, 0,
                                         kTilePixelSize, kTilePixelSize));
      tile_sprite.setPosition(shape_tile_coordinates[i].x * kTilePixelSize,
                         shape_tile_coordinates[i].y * kTilePixelSize);
      tile_sprite.move(28,31);
      window.draw(tile_sprite);
    }
    //Frame goes after so pieces do appear above it
    window.draw(frame_sprite);
    window.display();
  }

  return 0;
}
