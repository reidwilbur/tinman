#ifndef __DisplayRoutine_h_INCLUDED__
#define __DisplayRoutine_h_INCLUDED__

#include "Display.h"
#include "DisplayConfig.h"
#include <vector>

namespace display_routine {

class DisplayRoutine {
protected:
    display::Display& display;
    display::DisplayConfig defaultCfg;
public:
    DisplayRoutine(display::Display& display, display::DisplayConfig cfg): display(display), defaultCfg(cfg) {};
    virtual void init() = 0;
    virtual void step(const display::DisplayConfig& config) = 0;
    const display::DisplayConfig& getDefaultConfig() const {
        return defaultCfg;
    };
};

class Ticker: public DisplayRoutine {
private:
    String msgStr;
    int col;
public:
    Ticker(display::Display& display);
    void init();
    void step(const display::DisplayConfig& config);
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
};

class DigitalRain: public DisplayRoutine {
public:
    DigitalRain(display::Display& display);
    void init();
    void step(const display::DisplayConfig& config);
};

class Sparkle: public DisplayRoutine {
public:
    Sparkle(display::Display& display);
    void init();
    void step(const display::DisplayConfig& config);
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
};

class Static: public DisplayRoutine {
public:
    Static(display::Display& display);
    void init();
    void step(const display::DisplayConfig& config);
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
};

class DisplayRoutines {
private:
    DigitalRain rain;
    Fire fire;
    Kitt kitt;
    NyanCat nyanCat;
    Sparkle sparkle;
    Static tvStatic;
    Ticker ticker;
public:
    DisplayRoutines(display::Display& disp):
        rain(DigitalRain(disp)),
        fire(Fire(disp)),
        kitt(Kitt(disp)),
        nyanCat(NyanCat(disp)),
        sparkle(Sparkle(disp)),
        tvStatic(Static(disp)),
        ticker(Ticker(disp)) {
    }

    const DisplayRoutine* getRoutine(const String& mode) {
        for (auto idx = 0; idx < display::MODE_STRINGS.size(); idx++) {
            if (mode == display::MODE_STRINGS[idx]) {
              return &getRoutine((display::Mode)idx);
            }
        }
        return NULL;
    }

    DisplayRoutine& getRoutine(display::Mode mode) {
        switch(mode) {
            case display::Mode::DIGITAL_RAIN: return rain;
            case display::Mode::FIRE: return fire;
            case display::Mode::KITT: return kitt;
            case display::Mode::NYANCAT: return nyanCat;
            case display::Mode::SPARKLE: return sparkle;
            case display::Mode::STATIC: return tvStatic;
            case display::Mode::TICKER: return ticker;
        }
    }
};

}

#endif