#ifndef __Ticker_h_INCLUDED__
#define __Ticker_h_INCLUDED__

#define FASTLED_ESP8266_RAW_PIN_ORDER

#include <FastLED.h>

namespace Ticker {

#define NUM_LEDS          36
#define NUM_STRIPS         6
#define FRAMES_PER_SECOND 15
#define MAX_CHAR_WIDTH     5

void setup();

int writeChar(int col, CRGB color, char c);

int writeString(int col, CRGB color, char* msg);

int writeString(int col, CRGB color, const String& msg);

void sanitize(String& msg);

void clear();

}

#endif