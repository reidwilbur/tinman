#include <Arduino.h>
#include <FastLED.h>
#include "DisplayRoutine.h"

namespace DisplayRoutine {

static const CRGB firePalette[] = {
  CRGB(0,0,0),
  CRGB(35,10,7),
  CRGB(103,20,7),
  CRGB(175,63,7),
  CRGB(223,87,7),
  CRGB(207,127,15),
  CRGB(191,167,39),
  CRGB(0xEF,0xEF,0xC7)
};

static const int firePaletteLen = sizeof(firePalette)/sizeof(firePalette[0]);

//TODO: fix this hardcoded stuff
static uint8_t firePixels[8][40];

void fireDebugPrint(int width, int height) {
  char dgbstr[3*width + 1];
  for (uint row = 0; row < height; row++) {
    uint len = 0;
    for (uint col = 0; col < width; col++) {
      len += sprintf(dgbstr + len, "%2d ", firePixels[row][col]);
    }
    Serial.println(dgbstr);
  }
  Serial.println();
}

void spreadFire(int width, uint row, uint col) {
  int rand = random8(0, 4);
  int belowCol = col - rand + 1;
  belowCol = (width + belowCol) & width;
  int belowColor = firePixels[row + 1][col];
  rand = random8(0, 3);
  int newColor = belowColor - 1;
  firePixels[row][col] = (newColor < 0) ? 0 : newColor;
}

void setFlame(uint row, uint col) {
  int rand = random8(0, 6);
  firePixels[row][col] = 7 - rand;
}

DoomFire::DoomFire(Display::Display& display): DisplayRoutine(display) {
}

void DoomFire::init() {
  for (uint row=0; row < display.height()- 1; row++) {
    for (uint col=0; col < display.width(); col++) {
      firePixels[row][col] = 0;
    }
  }
}

void DoomFire::step(const DisplayConfigServer::DisplayConfig& config) {
  fireDebugPrint(display.width(), display.height());
  for (uint row=0; row < display.height() - 1; row++) {
    for (uint col=0; col < display.width(); col++) {
      spreadFire(display.width(), row, col);
    }
  }
  for (uint col=0; col < display.width(); col++) {
    setFlame(display.height() - 1, col);
  }
  for (uint col=0; col < display.width(); col++) {
    for (uint row=0; row < display.height(); row++) {
      display.setPixel(row, col, firePalette[firePixels[row][col]]);
    }
  }
}

}