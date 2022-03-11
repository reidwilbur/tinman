#ifndef __DisplayRoutine_h_INCLUDED__
#define __DisplayRoutine_h_INCLUDED__

#include "Display.h"
#include "DisplayConfig.h"
#include <vector>
#include <option.h>

namespace display_routine {

class DisplayRoutine {
protected:
    display::Display& display;
public:
    DisplayRoutine(display::Display& display): display(display) {};
    virtual void init() = 0;
    virtual void step(const display::DisplayConfig& config) = 0;
    //virtual const display::DisplayConfig& getDefaultConfig() = 0;

    //static const DisplayRoutine* getRoutine(const String& mode);
    //static const DisplayRoutine& getRoutine(display::Mode mode);
};

class Ticker: public DisplayRoutine {
private:
    String msgStr;
    int col;
public:
    Ticker(display::Display& display);
    void init();
    void step(const display::DisplayConfig& config);
    //const display::DisplayConfig& getDefaultConfig();
};

class Fire: public DisplayRoutine {
private:
    std::vector<uint8_t> firePixels;
    void setFlame(uint row);
    void spreadFire(uint row, uint col);
    void debugPrint();
public:
    Fire(display::Display& display);
    void init();
    void step(const display::DisplayConfig& config);
    //const display::DisplayConfig& getDefaultConfig();
};

class DigitalRain: public DisplayRoutine {
public:
    DigitalRain(display::Display& display);
    void init();
    void step(const display::DisplayConfig& config);
    //const display::DisplayConfig& getDefaultConfig();
};

class Sparkle: public DisplayRoutine {
public:
    Sparkle(display::Display& display);
    void init();
    void step(const display::DisplayConfig& config);
    //const display::DisplayConfig& getDefaultConfig();
};

class Kitt: public DisplayRoutine {
private:
    int lightIdx;
    int dir;
    void drawLight();
public:
    Kitt(display::Display& display);
    void init();
    void step(const display::DisplayConfig& config);
    //const display::DisplayConfig& getDefaultConfig();
};

class Static: public DisplayRoutine {
public:
    Static(display::Display& display);
    void init();
    void step(const display::DisplayConfig& config);
    //const display::DisplayConfig& getDefaultConfig();
};

class NyanCat: public DisplayRoutine {
private:
    int catFrame;
    int catPosR;
    int catPosC;
public:
    NyanCat(display::Display& display);
    void init();
    void step(const display::DisplayConfig& config);
    //const display::DisplayConfig& getDefaultConfig();
};

}

#endif