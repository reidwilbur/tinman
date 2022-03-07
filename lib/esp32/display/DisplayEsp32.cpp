#include "display/Display.h"
#include <FastLED.h>

namespace display {

#define LED_WIDTH          40
#define LED_HEIGHT          8

#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB

#define BRIGHTNESS 8

static const CRGB RAIN_DROP = CRGB(175,255,175);
static const CRGB RAIN_TAIL = CRGB(27,130,39);

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

void sanitize(String& msg) {
  unsigned int i = 0;
  while (i < msg.length()) {
    if (msg[i] >= ' ' && msg[i] <= '~') {
      i++;
    } else {
      msg.remove(i, 1);
    }
  }
}

void stepSparkle() {
  int maxled = LED_WIDTH * LED_HEIGHT;
  CRGB white = CRGB(255,255,255);
  for (int idx = 0; idx < maxled; idx++) {
    if (leds[idx].getLuma() == 0 && random8(255) > 250) {
      leds[idx] = white;
    } else if (leds[idx] == white && random8(255) > 200) {
      leds[idx] = CHSV(random8(255), random8(255), 255);
    } else {
      leds[idx].nscale8(192);
    }
  }
}

void stepDigitalRain() {
  for (int row=LED_HEIGHT - 1; row>=0; row--)
  {
    for (int col=0; col<LED_WIDTH; col++)
    {
      uint ledIdx = rcToDispIdx(row, col);
      if (leds[ledIdx] == RAIN_DROP)
      {
        leds[ledIdx] = RAIN_TAIL;
        if (row < LED_HEIGHT - 1) {
          uint nextHead = rcToDispIdx(row + 1, col);
          leds[nextHead] = RAIN_DROP; 
        }
      }
      if (leds[ledIdx].g != 255) {
        leds[ledIdx].nscale8(192);
      }
    }
  }

  for (int col=0; col<LED_WIDTH; col++) {
    bool noCode = true;
    for (int row=0; row<LED_HEIGHT; row++) {
      uint ledIdx = rcToDispIdx(row, col);
      if (leds[ledIdx] == RAIN_DROP) {
        noCode = false;
        break;
      }
    }
    if (noCode && random(24) == 0) {
      uint ledIdx = rcToDispIdx(0, col);
      leds[ledIdx] = RAIN_DROP;
    }
  }
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