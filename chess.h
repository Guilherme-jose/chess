#include<string>
#include<map>
#include"FastLED.h"
#define NUM_LEDS 256
#define DATA_PIN "G36"

CRGB leds[NUM_LEDS];

String activePlayer = "none";

int selectedX = 0;
int selectedY = 0;

int boardX = 16;
int boardY = 16;

int size = 16;

char board[64];
char startPos[64] = {
  'r','n','b','q','k','b','n','r',
  'p','p','p','p','p','p','p','p',
  'o','o','o','o','o','o','o','o',
  'o','o','o','o','o','o','o','o',
  'o','o','o','o','o','o','o','o',
  'o','o','o','o','o','o','o','o',
  'P','P','P','P','P','P','P','P',
  'R','N','B','Q','K','B','N','R'
};

std::map<char, CRGB> colorMap = {{'r',CRGB::Red}, {'n', CRGB::Yellow}, {'b', CRGB::Green}, {'q', CRGB::Blue}, {'k', CRGB::White}, {'p', CRGB::Orange},
                               {'R',CRGB::Red}, {'N', CRGB::Yellow}, {'B', CRGB::Green}, {'Q', CRGB::Blue}, {'K', CRGB::White}, {'P', CRGB::Orange},
};

void initBoard() {
  for(int i = 0; i < 64; i++)
    board[i] = startPos[i];
}

void chessSetup() {
  initBoard();
  FastLED.addLeds<NEOPIXEL, 26>(leds, NUM_LEDS);
  activePlayer = "white";
}

void play(String move) {

}

void moveSelected(String direction) {
  if(direction == "UP")
    selectedY++;
  if(direction == "DOWN")
    selectedY--;
  if(direction == "RIGHT")
    selectedX++;
  if(direction == "LEFT")
    selectedX--;

  selectedX = selectedX % size;
  selectedY = selectedY % size;
}

void drawBoard() {
  for(int i = 0; i < 256; i++) {
    if(i % 4 < 2)
      leds[i] = CRGB::Black;
    else
      leds[i] = CRGB::Gray;
  }
  for (int j = 0; j < 8; j++)
    for(int i = 0; i < 8; i++) {
      leds[2*i + 2*j] = colorMap[board[i + 8 * j]];
      leds[2*i + 2*j + 1] = colorMap[board[i + 8 * j]];
      leds[2*i + 2*j + 16] = colorMap[board[i + 8 * j]];
      leds[2*i + 2*j + 17] = colorMap[board[i + 8 * j]];
    }

  leds[selectedX +  size * selectedY] = CRGB::Cyan;
  FastLED.show();
}

void clearDisplay(){
  for(int i = 0; i < 64; i++) {
    leds[i] = CRGB::Black;     
  }
}

void chessLoop() {
  drawBoard();
  clearDisplay();
}
