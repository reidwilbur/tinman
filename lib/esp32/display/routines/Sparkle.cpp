#include <Arduino.h>
#include <FastLED.h>
#include "display/DisplayRoutine.h"

namespace display_routine {

static const CRGB WHITE = CRGB(255,255,255);

Sparkle::Sparkle(display::Display& display): 
  DisplayRoutine(display, {"", 0x0, 0x0, 25, display::Mode::SPARKLE, false, 64}) {
}

void Sparkle::init() {
}

void Sparkle::step(const display::DisplayConfig& config) {
  for (int row = 0; row < display.height(); row++) {
    for (int col = 0; col < display.width(); col++) {
      uint8_t rand = random8(255);
      CRGB pixel = display(row, col);
      if (pixel.getLuma() == 0 && rand > 250) {
        display(row, col) = WHITE;
      } else if (pixel == WHITE && rand > 200) {
        display(row, col) = CHSV(random8(255), random8(255), 255);
      } else {
        display(row, col) = pixel.nscale8(192);
      }
    }
  }
}

}