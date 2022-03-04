#ifndef __Display_h_INCLUDED__
#define __Display_h_INCLUDED__

//#include <FastLED.h>
//#include <stdint.h>
//#include <types.h>
//#include <string>
#include <WString.h>

namespace Display {

//void setup();
//
//int width();
//
//int maxCharWidth();
//
//int framesPerSecond();
//
//int writeChar(int col, CRGB color, char c);
//
//int writeString(int col, CRGB color, char* msg);
//
//int writeString(int col, CRGB color, const String& msg);
//
//void stepDigitalRain();
//
//void stepSparkle();
//
//void initFire();
//
//void stepFire();

void sanitize(String& msg);

//void clear();

class Display {
public:
    Display();

    virtual int width();

    virtual int height();

    virtual uint32_t getPixel(int row, int col);

    virtual void setPixel(int row, int col, uint32_t color);

    virtual void clear();
};

}

#endif