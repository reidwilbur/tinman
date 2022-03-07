#include <Arduino.h>
#include <FastLED.h>
#include "DisplayRoutine.h"

namespace display_routine {

static const CRGB WHITE = CRGB(255,255,255);

Sparkle::Sparkle(display::Display& display): DisplayRoutine(display) {
}

void Sparkle::init() {
}

void Sparkle::step(const display_config_server::DisplayConfig& config) {
  for (int row = 0; row < display.height(); row++) {
    for (int col = 0; col < display.width(); col++) {
      uint8_t rand = random8(255);
      CRGB pixel = display.getPixel(row, col);
      if (pixel.getLuma() == 0 && rand > 250) {
        display.setPixel(row, col, WHITE);
      } else if (pixel == WHITE && rand > 200) {
        display.setPixel(row, col, CHSV(random8(255), random8(255), 255));
      } else {
        display.setPixel(row, col, pixel.nscale8(192));
      }
    }
  }
}

}