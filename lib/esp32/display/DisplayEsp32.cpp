#include "display/Display.h"
#include <FastLED.h>

namespace display {

#define LED_WIDTH          40
#define LED_HEIGHT          8

#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB

#define BRIGHTNESS 8

static CRGB leds[LED_WIDTH * LED_HEIGHT];

uint rcToDispIdx(const uint row, const uint col) {
  // this display is wired in zig zag pattern 
  // with led index 0 in bottom left
  //  7   8  23  24 ..
  //  6   9  22  25 ..
  //  .  ..  ..  .. ..
  //  .  ..  ..  .. ..
  //  1  14  17  30 ..
  //  0  15  16  31 ..
  //
  // (row,col) for api assumes (0,0) in top left of display
  // so need to map like below etc
  // (0,0) = 7
  // (1,0) = 6
  // (6,0) = 1
  // (7,0) = 0
  //
  // (0,1) = 8
  // (1,1) = 9
  // (6,1) = 14
  // (7,1) = 15
  return ((col & 1) == 0) 
    ? col * LED_HEIGHT + row
    : ((col + 1) * LED_HEIGHT) - 1 - row;
}

Display::Display() {
  FastLED.addLeds<LED_TYPE, GPIO_NUM_23, COLOR_ORDER>(leds, LED_WIDTH * LED_HEIGHT).setCorrection(TypicalLEDStrip).setDither(1);
  FastLED.setBrightness(BRIGHTNESS);
}

int Display::width() {
  return LED_WIDTH;
}

int Display::height() {
  return LED_HEIGHT;
}

CRGB Display::getPixel(int row, int col) {
  uint ledIdx = rcToDispIdx(row, col);
  return leds[ledIdx];
}

void Display::setPixel(int row, int col, CRGB color) {
  uint ledIdx = rcToDispIdx(row, col);
  leds[ledIdx] = color;
}

void Display::clear() {
  FastLED.clear();
}

}