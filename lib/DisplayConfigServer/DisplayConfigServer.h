#ifndef __DisplayConfigServer_h_INCLUDED__
#define __DisplayConfigServer_h_INCLUDED__

namespace DisplayConfigServer {

void setup();

void loop();

enum Mode { DIGITAL_RAIN, TEXT_SCROLL };

struct DisplayConfig {
  String message;
  uint32 textColor;
  uint32 bkgColor;
  uint32 speed;
  Mode mode;
};

DisplayConfig& getConfig();

}

#endif
