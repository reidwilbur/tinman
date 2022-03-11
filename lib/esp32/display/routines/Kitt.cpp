#include <Arduino.h>
#include <FastLED.h>
#include "display/DisplayRoutine.h"

namespace display_routine {

static const int LIGHTS = 8;

Kitt::Kitt(display::Display& display): DisplayRoutine(display), lightIdx(0), dir(1) {
}

void Kitt::init() {
  lightIdx = 0;
  dir = 1;
}

void Kitt::drawLight() {
  auto lwidth = display.width() / LIGHTS;
  auto rowofs = 1;
  auto colofs = lightIdx * lwidth;

  for (auto row = 0; row < lwidth; row++) {
    for (auto col = 0; col < lwidth; col++) {
      if (row == 0 || row == lwidth - 1 || col == 0 || col == lwidth - 1) {
        display(row + rowofs, col + colofs) = CRGB(153,9,6);
      } else {
        display(row + rowofs, col + colofs) = CRGB(254,7,7);
      }
    }
  }
  if ((lwidth & 1) == 1) {
    auto center = (lwidth / 2);
    display(center + rowofs, center + colofs) = CRGB(249,53,41);
  }
}

void Kitt::step(const display::DisplayConfig& config) {
  for (auto row = 0; row < display.height(); row++) {
    for (auto col = 0; col < display.width(); col++) {
      display(row, col).nscale8(150);
    }
  }
  drawLight();
  lightIdx += dir;
  if (lightIdx == 0 && dir == -1) {
    dir = 1;
  } else if (lightIdx == LIGHTS - 1 && dir == 1) {
    dir = -1;
  }
}

}