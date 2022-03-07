#ifndef __DisplayRoutine_h_INCLUDED__
#define __DisplayRoutine_h_INCLUDED__

#include "Display.h"
#include "configserver/DisplayConfigServer.h"
#include <vector>

namespace display_routine {

class DisplayRoutine {
protected:
    display::Display& display;
public:
    DisplayRoutine(display::Display& display): display(display) {};
    virtual void init() = 0;
    virtual void step(const display_config_server::DisplayConfig& config) = 0;
};

class Ticker: public DisplayRoutine {
private:
    String msgStr;
    int col;
public:
    Ticker(display::Display& display);
    void init();
    void step(const display_config_server::DisplayConfig& config);
};

class DoomFire: public DisplayRoutine {
private:
    std::vector<uint8_t> firePixels;
    void setFlame(uint row, uint col);
    void spreadFire(uint row, uint col);
    void debugPrint();
public:
    DoomFire(display::Display& display);
    void init();
    void step(const display_config_server::DisplayConfig& config);
};

class DigitalRain: public DisplayRoutine {
public:
    DigitalRain(display::Display& display);
    void init();
    void step(const display_config_server::DisplayConfig& config);
};

class Sparkle: public DisplayRoutine {
public:
    Sparkle(display::Display& display);
    void init();
    void step(const display_config_server::DisplayConfig& config);
};

}

#endif