#include "Ticker.h"

namespace Ticker {

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

int writeChar(int loc, CRGB color, char c) {
  if (c == ' ') {
    return 2;
  }
  uint8 charDefOfs = ofs[c - 'A'];
  uint8 charDefSize = ofs[c - 'A' + 1] - charDefOfs;
  for (uint8 r = 0; r < NUM_STRIPS; r++) {
    for (uint8 c = 0; c < charDefSize; c++) {
      int pixelLoc = c + loc;
      if (pixelLoc >= 0 && pixelLoc < NUM_LEDS) {
        if (char_data[r][c + charDefOfs]) {
          leds[r][pixelLoc] = color;
        }
      }
    }
  }
  return charDefSize;
}

int writeString(int loc, CRGB color, char* msg) {
  char* msgChar = msg;
  int charStart = loc;
  while(*msgChar) {
    int charSize = writeChar(charStart, color, *msgChar);
    charStart += charSize + 1;
    msgChar++;
  }
  return charStart;
}
}
