#include <iostream>
#include <SFML/Graphics.hpp>
#include <ctime>
#include <string>
#include <cstdlib>

using namespace sf;

const std::string kTileTextureFilePath = "tiles.png";
const std::string kBackgroundTextureFilePath = "background.png";
const std::string kFrameTextureFilePath = "frame.png";
const std::string kScoreFrameTextureFilePath = "score_frame.png";
const std::string kCalibriFontFilePath = "calibri.ttf";

const int kMaxTiles = 4;
const int kTotalTetriminos = 8;
const int kTilePixelSize = 18;
const int kFieldWidth = 10;
const int kFieldHeight = 20;

const int kWindowWidth = 320;
const int kWindowHeight = 480;
const int kScoreStartX = 307;
const int kScoreStartY = 455;
const int kScoreTextSize = 40;
const int kTileSpriteX_Offset = 28;
const int kTileSpriteY_Offset = 31;

const float kScoreFrameX_Scale = 0.2;
const float kScoreFrameY_Scale = 0.17;

// There are 7 Tetris blocks (called Tetriminos), each have 4 blocks and a name
// Each number represents a filled in block in a 2x4 array
// Each Tetrimino is in order that its color appears in tiles.png as to ensure
// each Tetrimino is using its official color
// A blank space at index 0 is necessary to ensure that a 0 Tetrimino index
// is not interpreted as a blank space. Thus the second blue tile from the
// texture is used at index 7
const int kAllTetriminos[kTotalTetriminos][kMaxTiles] = {
        {0, 0, 0, 0}, // Blank
        {2, 4, 5, 6}, // T
        {3, 4, 5, 6}, // Z
        {2, 4, 5, 7}, // S
        {2, 3, 4, 5}, // O
        {1, 3, 5, 7}, // I
        {2, 4, 6, 7}, // L
        {3, 5, 6, 7}, // J
};


struct Point {
    int x;
    int y;
} current_tetrimino_coords[kMaxTiles], prev_tetrimino_coords[kMaxTiles];


// Checks to make sure the Tetrimino is on the board and that no Tetrimino
// is already in the next position. Assuming both are true the function returns
// true and allows the Tetrimino to enter that position
// Input is just the playable_area which contains all positions that a Tetrimino
// can be in as well as the position of all previous Tetriminos
bool check(int playable_area[kFieldHeight][kFieldWidth]) {
  for (int i = 0; i < kMaxTiles; ++i) {
    if (current_tetrimino_coords[i].x < 0 ||
        current_tetrimino_coords[i].x >= kFieldWidth ||
        current_tetrimino_coords[i].y >= kFieldHeight) {
      return false;
    } else if (playable_area[current_tetrimino_coords[i].y]
               [current_tetrimino_coords[i].x] > 0) {
      return false;
    }
  }
  return true;
}

// Assigns the coordinates to the coordinate holder
// Input is the tetrimino_index to get the correct tetrimino
void AssignCoordinates(int tetrimino_index) {
  for (int i = 0; i < kMaxTiles; i++) {
    //Max of 2 Tiles on x-axis
    current_tetrimino_coords[i].x = kAllTetriminos[tetrimino_index][i] % 2;
    //Max of 4 Tiles on y-axis
    current_tetrimino_coords[i].y = kAllTetriminos[tetrimino_index][i] / 2;
  }
}

// Rotates the Tetrimino according to the key pressed
// The booleans passed in will be true when specific keys are pressed (Line 155)
// The playable_area is passed in to ensure that the Tetrimino can go there
void RotateTetrimino(bool clockwise_rotation, bool counter_clockwise_rotation,
                     int playable_area[kFieldHeight][kFieldWidth]) {
  if (clockwise_rotation) {
    Point centerOfShape = current_tetrimino_coords[1];
    for (int i = 0; i < kMaxTiles; i++) {
      int yDistanceToCenter = current_tetrimino_coords[i].y - centerOfShape.y;
      int xDistanceToCenter = current_tetrimino_coords[i].x - centerOfShape.x;
      current_tetrimino_coords[i].x = centerOfShape.x - yDistanceToCenter;
      current_tetrimino_coords[i].y = centerOfShape.y + xDistanceToCenter;
    }
    if (!check(playable_area)) {
      for (int i = 0; i < kMaxTiles; ++i) {
        current_tetrimino_coords[i] = prev_tetrimino_coords[i];
      }
    }
  }

  if (counter_clockwise_rotation) {
    Point centerOfShape = current_tetrimino_coords[1];
    for (int i = 0; i < kMaxTiles; i++) {
      int yDistanceToCenter = current_tetrimino_coords[i].y - centerOfShape.y;
      int xDistanceToCenter = current_tetrimino_coords[i].x - centerOfShape.x;
      current_tetrimino_coords[i].x = centerOfShape.x + yDistanceToCenter;
      current_tetrimino_coords[i].y = centerOfShape.y - xDistanceToCenter;
    }
    if (!check(playable_area)) {
      for (int i = 0; i < kMaxTiles; ++i) {
        current_tetrimino_coords[i] = prev_tetrimino_coords[i];
      }
    }
  }
}

int AlignPoints(const std::string& score) {
  int length = score.size();
  return kScoreStartX - (length-1) * kScoreTextSize / 2;
}

int main() {

  srand(time(0));

  RenderWindow window(VideoMode(kWindowWidth, kWindowHeight), "Tetris");

  Texture tile_texture;
  tile_texture.loadFromFile(kTileTextureFilePath);
  tile_texture.setSmooth(true);

  Texture background_texture;
  background_texture.loadFromFile(kBackgroundTextureFilePath);
  background_texture.setSmooth(true);

  Texture frame_texture;
  frame_texture.loadFromFile(kFrameTextureFilePath);
  frame_texture.setSmooth(true);

  Texture score_frame_texture;
  score_frame_texture.loadFromFile(kScoreFrameTextureFilePath);
  score_frame_texture.setSmooth(true);

  Sprite tile_sprite(tile_texture);
  Sprite background_sprite(background_texture);
  Sprite frame_sprite(frame_texture);
  Sprite score_frame_sprite(score_frame_texture);
  score_frame_sprite.scale(kScoreFrameX_Scale, kScoreFrameY_Scale);

  Font font;
  font.loadFromFile(kCalibriFontFilePath);

  int score = 0;
  Text score_as_text(std::to_string(score), font, kScoreTextSize);
  score_as_text.setPosition(kScoreStartX, kScoreStartY);
  score_as_text.setFillColor(Color::Black);

  FloatRect floatRect = score_as_text.getLocalBounds();
  score_as_text.setOrigin(floatRect.left + floatRect.width,
                          floatRect.top + floatRect.height);

  int x_movement = 0;
  bool clockwise_rotation = false;
  bool counter_clockwise_rotation = false;
  int tetrimino_index = rand() % (kTotalTetriminos - 1) + 1;
  float timer = 0;
  float fall_delay = 0.3;

  int playable_area[kFieldHeight][kFieldWidth] = {0};

  AssignCoordinates(tetrimino_index);

  Clock clock;

  while (window.isOpen()) {
    if (!check(playable_area)){
      break;
    }
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
            clockwise_rotation = true;
            break;
          case Keyboard::X:
            clockwise_rotation = true;
            break;
          case Keyboard::LControl:
            counter_clockwise_rotation = true;
            break;
          case Keyboard::RControl:
            counter_clockwise_rotation = true;
            break;
          case Keyboard::Z:
            counter_clockwise_rotation = true;
            break;
          case Keyboard::Left:
            x_movement = -1;
            break;
          case Keyboard::Right:
            x_movement = 1;
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
      prev_tetrimino_coords[i] = current_tetrimino_coords[i];
      current_tetrimino_coords[i].x += x_movement;
    }
    if (!check(playable_area)) {
      for (int i = 0; i < kMaxTiles; ++i) {
        current_tetrimino_coords[i] = prev_tetrimino_coords[i];
      }
    }

    //Rotation
    //If the tetrimino is not O then it can rotate
    if (tetrimino_index != 4) {
      RotateTetrimino(clockwise_rotation, counter_clockwise_rotation,
                      playable_area);
    }

    //Falling animation
    if (timer > fall_delay) {
      for (int i = 0; i < kMaxTiles; i++) {
        prev_tetrimino_coords[i] = current_tetrimino_coords[i];
        current_tetrimino_coords[i].y += 1;
      }
      if (!check(playable_area)) {
        for (int i = 0; i < kMaxTiles; ++i) {
          playable_area[prev_tetrimino_coords[i].y]
          [prev_tetrimino_coords[i].x] = tetrimino_index % 8;
        }
        tetrimino_index = rand() % (kTotalTetriminos - 1) + 1;
        AssignCoordinates(tetrimino_index);
      }
      timer = 0;
    }

    //Check for full line
    int line_to_check = kFieldHeight - 1;
    int num_completed_lines = 0;
    for (int i = kFieldHeight - 1; i > 0; --i) {
      int total_filled_tiles = 0;
      for (int j = 0; j < kFieldWidth; ++j) {
        if (playable_area[i][j]) {
          total_filled_tiles++;
        }
        playable_area[line_to_check][j] = playable_area[i][j];
      }
      if (total_filled_tiles < kFieldWidth) {
        line_to_check--;
      }
      else {
        num_completed_lines++;
      }
    }

    switch (num_completed_lines) {
      case 1:
        score += 100;
        break;
      case 2:
        score += 300;
        break;
      case 3:
        score += 500;
        break;
      case 4: // A Tetris
        score += 800;
        break;
      default:
        break;
    }

    //Reset everything for next button press
    x_movement = 0;
    clockwise_rotation = false;
    counter_clockwise_rotation = false;
    fall_delay = 0.3;

    //Create board
    window.clear(Color::White);
    window.draw(background_sprite);

    //This code holds the placement of the previous Tetriminos
    for (int i = 0; i < kFieldHeight; ++i) {
      for (int j = 0; j < kFieldWidth; ++j) {
        if (playable_area[i][j] == 0) {
          continue;
        }
        tile_sprite.setTextureRect(
                IntRect(playable_area[i][j] * kTilePixelSize, 0,
                        kTilePixelSize, kTilePixelSize));
        tile_sprite.setPosition(j * kTilePixelSize, i * kTilePixelSize);
        tile_sprite.move(kTileSpriteX_Offset, kTileSpriteY_Offset);
        window.draw(tile_sprite);
      }
    }

    //This code shows the Tetrimino dropping
    for (int i = 0; i < kMaxTiles; i++) {
      tile_sprite.setTextureRect(IntRect(tetrimino_index * kTilePixelSize, 0,
                                         kTilePixelSize, kTilePixelSize));
      tile_sprite.setPosition(current_tetrimino_coords[i].x * kTilePixelSize,
                              current_tetrimino_coords[i].y * kTilePixelSize);
      tile_sprite.move(kTileSpriteX_Offset, kTileSpriteY_Offset);
      window.draw(tile_sprite);
    }

    //Frame goes after so pieces do appear above it
    window.draw(frame_sprite);
    score_frame_sprite.setPosition(195, 400);
    score_as_text.setString(std::to_string(score));
    std::cout << AlignPoints(std::to_string(score)) << std::endl;
    score_as_text.setPosition(AlignPoints(std::to_string(score)), kScoreStartY);
    window.draw(score_frame_sprite);
    window.draw(score_as_text);
    window.display();


  }

  std::string kGameOverString = "Game Over!\nQ = Quit";
  Text game_over(kGameOverString, font, 30);
  game_over.setPosition(10, 400);
  game_over.setFillColor(Color::Black);
  window.draw(game_over);
  window.display();
  while (window.isOpen()){
    Event event;
    while (window.pollEvent(event)) {
      if (event.type == Event::Closed) {
        window.close();
      }
      if (event.type == Event::KeyPressed) {
        if (event.key.code == Keyboard::Q){
          window.close();
        }
      }
    }
  }

  return 0;
}
