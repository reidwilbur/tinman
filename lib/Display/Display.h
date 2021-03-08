#ifndef __Display_h_INCLUDED__
#define __Display_h_INCLUDED__

#ifdef PROJECT_ESP8266
  #define FASTLED_ESP8266_RAW_PIN_ORDER
  #define NUM_LEDS           36
  #define FRAMES_PER_SECOND  15
  #define MAX_CHAR_WIDTH      6
#endif

#ifdef PROJECT_ESP32
  #define NUM_LEDS           40
  #define FRAMES_PER_SECOND  15
  #define MAX_CHAR_WIDTH      6
#endif

#include <FastLED.h>

namespace Display {

void setup();

int writeChar(int col, CRGB color, char c);

int writeString(int col, CRGB color, char* msg);

int writeString(int col, CRGB color, const String& msg);

void stepDigitalRain();

void sanitize(String& msg);

void clear();

}

#endif