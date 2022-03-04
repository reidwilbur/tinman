#ifndef __DisplayRoutine_h_INCLUDED__
#define __DisplayRoutine_h_INCLUDED__

#include "Display.h"
#include "configserver/DisplayConfigServer.h"

namespace DisplayRoutine {

class DisplayRoutine {
protected:
    Display::Display& display;

public:
    DisplayRoutine(Display::Display& display): display(display) {};

    virtual void init() = 0;

    virtual void step(const DisplayConfigServer::DisplayConfig& config) = 0;
};

class Ticker: public DisplayRoutine {
private:
    String msgStr;
    int col;
public:
    Ticker(Display::Display& display);

    void init();

    void step(const DisplayConfigServer::DisplayConfig& config);
};

}

#endif