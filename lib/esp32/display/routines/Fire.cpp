#include <Arduino.h>
#include <FastLED.h>
#include "display/DisplayRoutine.h"

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

void Fire::debugPrint() {
  char dgbstr[3*display.width() + 1];
  for (auto row = 0; row < display.height(); row++) {
    auto len = 0;
    for (auto col = 0; col < display.width(); col++) {
      len += sprintf(dgbstr + len, "%2d ", firePixels[row * display.width() + col]);
    }
    Serial.println(dgbstr);
  }
  Serial.println();
}

void Fire::spreadFire(uint row, uint col) {
  int belowCol = (int)col - random8(0, 2) + 1;
  belowCol = min(max(0, belowCol), (int)display.width() - 1);
  auto belowColor = firePixels[(row + 1) * display.width() + col];
  int newColor = max(0, belowColor - 1);
  firePixels[row * display.width() + col] = newColor;
}

void Fire::setFlame(uint row) {
  auto start = row * display.width();
  auto end = start + display.width();
  for (auto idx = start; idx < end; idx++) {
    firePixels[idx] = random8(4, firePalette.size());
  }
}

Fire::Fire(display::Display& display): 
  DisplayRoutine(display), 
  firePixels(std::vector<uint8_t>(display.width() * display.height())) {
}

void Fire::init() {
  for (auto idx = 0; idx < firePixels.size(); idx++) {
    firePixels[idx] = 0;
  }
}

void Fire::step(const display::DisplayConfig& config) {
  //debugPrint();
  for (auto row=0; row < display.height() - 1; row++) {
    for (auto col=0; col < display.width(); col++) {
      spreadFire(row, col);
    }
  }
  setFlame(display.height() - 1);
  for (auto idx = 0; idx < firePixels.size(); idx++) {
    display[idx] = firePalette[firePixels[idx]];
  }
}

}