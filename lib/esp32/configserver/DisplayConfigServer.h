#ifndef __DisplayConfigServer_h_INCLUDED__
#define __DisplayConfigServer_h_INCLUDED__

#include <stdint.h>
#include <display/Display.h>
#include <display/DisplayConfig.h>

namespace display_config_server {

class ConfigServer {
private:
  display::DisplayConfig config;
  void postSpeed();
  void postMode();
  void handleRoot();
  void handleNotFound();
  void getMode();
  void postSleep();
  void getSleep();
public:
  ConfigServer();
  int start(display::Display& disp);
  display::DisplayConfig& loop();
};

}

#endif
