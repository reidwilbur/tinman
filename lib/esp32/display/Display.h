#ifndef __Display_h_INCLUDED__
#define __Display_h_INCLUDED__

#include <FastLED.h>

namespace display {

class Display {
public:
    Display();
    int width();
    int height();
    CRGB& operator()(uint row, uint col);
    CRGB& operator[](uint idx);
    void clear();
    void show();
};

}

#endif