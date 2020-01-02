#include "Display.h"

namespace Display {

#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB

#define BRIGHTNESS           8 

static const uint8 char_data[6][108] = {
{ 0, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1 },
{ 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 1, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1 },
{ 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0 },
{ 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0 },
{ 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0 },
{ 1, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 1, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 1, 0, 1, 1, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1 }
};

static const uint8 ofs[27] = {0, 4, 8, 12, 16, 20, 24, 28, 32, 35, 39, 43, 47, 52, 57, 61, 65, 69, 73, 77, 80, 84, 89, 94, 99, 104, 108};

static CRGB leds[NUM_STRIPS][NUM_LEDS];

void setup() {
  FastLED.addLeds<LED_TYPE, D2, COLOR_ORDER>(leds[5], NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, D3, COLOR_ORDER>(leds[4], NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, D4, COLOR_ORDER>(leds[3], NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, D5, COLOR_ORDER>(leds[2], NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, D6, COLOR_ORDER>(leds[1], NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, D7, COLOR_ORDER>(leds[0], NUM_LEDS).setCorrection(TypicalLEDStrip);

  FastLED.setBrightness(BRIGHTNESS);
}

int writeChar(int col, CRGB color, char c) {
  if (c == ' ') {
    return 2;
  }
  uint8 charDefOfs = ofs[c - 'A'];
  uint8 charDefSize = ofs[c - 'A' + 1] - charDefOfs;
  for (uint8 row = 0; row < NUM_STRIPS; row++) {
    for (uint8 charCol = 0; charCol < charDefSize; charCol++) {
      int ledCol = charCol + col;
      if (ledCol >= 0 && ledCol < NUM_LEDS) {
        if (char_data[row][charCol + charDefOfs]) {
          leds[row][ledCol] = color;
        }
      }
    }
  }
  return charDefSize;
}

int writeString(int col, CRGB color, char* msg) {
  char* msgChar = msg;
  int charStart = col;
  while(*msgChar) {
    int charSize = writeChar(charStart, color, *msgChar);
    charStart += charSize + 1;
    msgChar++;
  }
  return charStart;
}

int writeString(int col, CRGB color, const String& msg) {
  int charStart = col;
  for (const char* c = msg.begin(); c != msg.end(); c++) {
    int charSize = writeChar(charStart, color, *c);
    charStart += charSize + 1;
  }
  return charStart;
}

void sanitize(String& msg) {
  msg.toUpperCase();
  unsigned int i = 0;
  while (i < msg.length()) {
    if (msg[i] == ' ' || (msg[i] - 'A' >= 0 && msg[i] - 'A' <= 'Z')) {
      i++;
    } else if (msg[i] == '+') {
      msg[i] = ' ';
      i++;
    } else {
      msg.remove(i, 1);
    }
  }
}

void clear() {
  FastLED.clear();
}

}
