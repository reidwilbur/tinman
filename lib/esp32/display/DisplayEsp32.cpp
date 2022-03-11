#include "display/Display.h"
#include <FastLED.h>

namespace display {

static const size_t LED_WIDTH = 40;
static const size_t LED_HEIGHT = 8;
static const size_t LED_SIZE = LED_WIDTH * LED_HEIGHT;
static const size_t BRIGHTNESS = 255;

static CRGB leds[LED_WIDTH * LED_HEIGHT];

uint rcToDispIdx(uint row, uint col) {
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
  FastLED.addLeds<WS2812B, GPIO_NUM_23, GRB>(leds, LED_WIDTH * LED_HEIGHT).setCorrection(TypicalPixelString).setDither(1);
}

size_t Display::width() {
  return LED_WIDTH;
}

size_t Display::height() {
  return LED_HEIGHT;
}

size_t Display::size() {
  return LED_SIZE;
}

CRGB& Display::operator()(size_t row, size_t col) {
  uint idx = rcToDispIdx(row, col);
  return leds[idx];
}

CRGB& Display::operator[](size_t idx) {
  auto cidx = idx % size();
  return leds[rcToDispIdx(cidx / LED_WIDTH, cidx % LED_WIDTH)];
}

void Display::clear() {
  FastLED.clear();
}

void Display::show() {
  FastLED.show(BRIGHTNESS);
}

void Display::delay(uint32_t ms) {
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.delay(ms);
}

}