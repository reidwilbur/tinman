#include <Arduino.h>
#include <FastLED.h>
#include "DisplayRoutine.h"

namespace display_routine {

static const CRGB RAIN_DROP = CRGB(175,255,175);
static const CRGB RAIN_TAIL = CRGB(27,130,39);

DigitalRain::DigitalRain(display::Display& display): DisplayRoutine(display) {
}

void DigitalRain::init() {
}

void DigitalRain::step(const display_config_server::DisplayConfig& config) {
  for (int row=display.height() - 1; row>=0; row--)
  {
    for (int col=0; col<display.width(); col++)
    {
      CRGB pixel = display.getPixel(row, col);
      if (pixel == RAIN_DROP) {
        display.setPixel(row, col, RAIN_TAIL);
        if (row < display.height() - 1) {
          display.setPixel(row + 1, col, RAIN_DROP);
        }
      }
      if (pixel.g != 255) {
        display.setPixel(row, col, pixel.nscale8(192));
      }
    }
  }

  for (int col=0; col<display.width(); col++) {
    bool noCode = true;
    for (int row=0; row<display.height(); row++) {
      CRGB pixel = display.getPixel(row, col);
      if (pixel == RAIN_DROP) {
        noCode = false;
        break;
      }
    }
    if (noCode && random(24) == 0) {
      display.setPixel(0, col, RAIN_DROP);
    }
  }
}

}