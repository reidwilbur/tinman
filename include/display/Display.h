#ifndef __Display_h_INCLUDED__
#define __Display_h_INCLUDED__

#include <FastLED.h>

namespace Display {

void setup();

int width();

int maxCharWidth();

int framesPerSecond();

int writeChar(int col, CRGB color, char c);

int writeString(int col, CRGB color, char* msg);

int writeString(int col, CRGB color, const String& msg);

void stepDigitalRain();

void stepSparkle();

void initFire();

void stepFire();

void sanitize(String& msg);

void clear();

}

#endif