#ifndef __DisplayConfigServer_h_INCLUDED__
#define __DisplayConfigServer_h_INCLUDED__

#include <stdint.h>
#include <display/Display.h>

namespace display_config_server {

enum Mode { DIGITAL_RAIN, TICKER, SPARKLE, FIRE, KITT };
static const String ModeStrings[] = {
  String("DIGITAL_RAIN"),
  String("TICKER"),
  String("SPARKLE"),
  String("FIRE"),
  String("KITT")
};

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
  void postSpeed();
  void postMode();
  void handleRoot();
  void handleNotFound();
  void getMode();
public:
  ConfigServer();
  int start(display::Display& disp);
  DisplayConfig& loop();
};

}

#endif
