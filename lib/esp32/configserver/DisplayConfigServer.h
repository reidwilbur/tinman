#ifndef __DisplayConfigServer_h_INCLUDED__
#define __DisplayConfigServer_h_INCLUDED__

#include <stdint.h>
#include <display/Display.h>
#include <display/DisplayConfig.h>
#include <display/DisplayRoutine.h>

namespace display_config_server {

class ConfigServer {
private:
  display::DisplayConfig config;
  display_routine::DisplayRoutines& routines;
  void handleRoot();
  void handleNotFound();
  void postMode();
  void getMode();
  void postSleep();
  void getSleep();
public:
  ConfigServer(display_routine::DisplayRoutines& routines);
  int start(display::Display& disp);
  display::DisplayConfig& loop();
};

}

#endif
