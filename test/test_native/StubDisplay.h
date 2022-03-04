#include <display/Display.h>

namespace test {

class StubDisplay: public Display::Display {
public:
    static const int width = 40;
    static const int height = 8;
    static uint32_t leds[width * height];

    StubDisplay();

    int width() { return width; }

    int height() { return height; }

    uint32_t getPixel(int row, int col) {
        return leds[row * width + col];
    }

    void setPixel(int row, int col, uint32_t color) {
        leds[row * width + col] = color;
    }

    void clear() {
        for (int i = 0; i < width * height; i++) {
            leds[i] = 0;
        }
    }
};

}