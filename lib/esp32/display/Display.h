#ifndef __Display_h_INCLUDED__
#define __Display_h_INCLUDED__

#include <FastLED.h>

namespace display {

class Display {
public:
    Display();
    size_t width();
    size_t height();
    size_t size();
    CRGB& operator()(size_t row, size_t col);
    CRGB& operator[](size_t idx);
    void clear();
    void show();
    void delay(uint32_t ms);
};

}

#endif