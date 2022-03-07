#include <Arduino.h>
#include <FastLED.h>
#include "DisplayRoutine.h"

namespace display_routine {

static const std::vector<CRGB> firePalette = {
  CRGB(0,0,0),
  CRGB(35,10,7),
  CRGB(103,20,7),
  CRGB(175,63,7),
  CRGB(223,87,7),
  CRGB(207,127,15),
  CRGB(191,167,39),
  CRGB(0xEF,0xEF,0xC7)
};

void DoomFire::debugPrint() {
  char dgbstr[3*display.width() + 1];
  for (uint row = 0; row < display.height(); row++) {
    uint len = 0;
    for (uint col = 0; col < display.width(); col++) {
      len += sprintf(dgbstr + len, "%2d ", firePixels[row * display.width() + col]);
    }
    Serial.println(dgbstr);
  }
  Serial.println();
}

void DoomFire::spreadFire(uint row, uint col) {
  int rand = random8(0, 4);
  int belowCol = col - rand + 1;
  belowCol = (display.width() + belowCol) & display.width();
  int belowColor = firePixels[(row + 1) * display.width() + col];
  rand = random8(0, 3);
  int newColor = belowColor - 1;
  firePixels[row * display.width() + col] = (newColor < 0) ? 0 : newColor;
}

void DoomFire::setFlame(uint row, uint col) {
  int rand = random8(0, 6);
  firePixels[row * display.width() + col] = (firePalette.size() - 1) - rand;
}

DoomFire::DoomFire(display::Display& display): 
  DisplayRoutine(display), 
  firePixels(std::vector<uint8_t>(display.width() * display.height())) {
}

void DoomFire::init() {
  for (uint row=0; row < display.height()- 1; row++) {
    for (uint col=0; col < display.width(); col++) {
      firePixels[row * display.width() + col] = 0;
    }
  }
}

void DoomFire::step(const display_config_server::DisplayConfig& config) {
  //debugPrint();
  for (uint row=0; row < display.height() - 1; row++) {
    for (uint col=0; col < display.width(); col++) {
      spreadFire(row, col);
    }
  }
  for (uint col=0; col < display.width(); col++) {
    setFlame(display.height() - 1, col);
  }
  for (uint col=0; col < display.width(); col++) {
    for (uint row=0; row < display.height(); row++) {
      display(row, col) = firePalette[firePixels[row * display.width() + col]];
    }
  }
}

}