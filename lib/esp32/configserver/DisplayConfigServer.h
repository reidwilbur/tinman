#ifndef __DisplayConfigServer_h_INCLUDED__
#define __DisplayConfigServer_h_INCLUDED__

#include <stdint.h>

namespace display_config_server {

void setup();

void loop();

enum Mode { DIGITAL_RAIN, TEXT_SCROLL, SPARKLE, FIRE };

struct DisplayConfig {
  String message;
  uint32_t textColor;
  uint32_t bkgColor;
  uint32_t speed;
  Mode mode;
};

DisplayConfig& getConfig();

}

#endif
