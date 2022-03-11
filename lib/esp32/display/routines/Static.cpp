#include <Arduino.h>
#include <FastLED.h>
#include "display/DisplayRoutine.h"

namespace display_routine {

static const CRGB WHITE = CRGB(255,255,255);

Static::Static(display::Display& display): 
  DisplayRoutine(display, {"", 0x0, 0x0, 30, display::Mode::STATIC, false, 64}) {
}

void Static::init() {
}

void Static::step(const display::DisplayConfig& config) {
  for (auto idx = 0; idx < display.size(); idx++) {
    if (random8() > 128) {
      auto lum = random8();
      display[idx] = CRGB(lum,lum,lum);
    } else {
      display[idx].nscale8(32);
    }
  }
}

}