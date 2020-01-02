#ifndef __TickerServer_h_INCLUDED__
#define __TickerServer_h_INCLUDED__

namespace TickerServer {

void setup();

void loop();

struct TickerConfig {
  String message;
  uint32 textColor;
  uint32 bkgColor;
  uint32 speed;
};

TickerConfig& getConfig();

}

#endif
