#ifndef __Display_h_INCLUDED__
#define __Display_h_INCLUDED__

#include <stdint.h>
#include <FastLED.h>

namespace Display {

class Display {
public:
    Display();

    virtual int width();

    virtual int height();

    virtual CRGB getPixel(int row, int col);

    virtual void setPixel(int row, int col, CRGB color);

    virtual void clear();
};

}

#endif