#ifndef __DisplayConfigServer_h_INCLUDED__
#define __DisplayConfigServer_h_INCLUDED__

#include <stdint.h>

namespace display_config_server {

enum Mode { DIGITAL_RAIN, TICKER, SPARKLE, FIRE, KITT };

struct DisplayConfig {
  String message;
  uint32_t textColor;
  uint32_t bkgColor;
  uint32_t speed;
  Mode mode;
};

class ConfigServer {
private:
  DisplayConfig config;
  void handleMsg();
  void handleFgColor();
  void handleSpeed();
  void handleMode();
  void handleRoot();
  void handleNotFound();
public:
  ConfigServer();
  int start();
  DisplayConfig& loop();
};

}

#endif
