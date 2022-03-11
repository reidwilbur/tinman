#include <Arduino.h>
#include <display/Display.h>
#include <configserver/DisplayConfigServer.h>
#include <display/DisplayRoutine.h>

#define ONBOARD_LED 2

using namespace display_config_server;
using namespace display_routine;
using namespace display;

display::Display disp;
DisplayRoutines routines(disp);
ConfigServer server(routines);
DisplayConfig lastCfg = routines.getRoutine(Mode::TICKER).getDefaultConfig();

void setup() {
  Serial.begin(115200);
  delay(1000);

  //pinMode(ONBOARD_LED, OUTPUT);
  //digitalWrite(ONBOARD_LED, LOW);

  auto res = server.start(disp);
  if (res != ESP_OK) {
    disp.clear();
    disp(0,0) = CRGB(255,0,0);
    disp.show();
    delay(3000);
    disp.clear();
    disp.show();
    esp_restart();
  }
}

void loop() {
  DisplayConfig& config = server.loop();
  if (config.sleep) {
    if (!lastCfg.sleep) {
      disp.clear();
      disp.show();
    }
    lastCfg = config;
    delay(1000);
  } else {
    bool modeChanged = lastCfg.mode != config.mode;
    lastCfg = config;
    DisplayRoutine& displayroutine = routines.getRoutine(config.mode);
    if (modeChanged) {
      displayroutine.init();
    }
    displayroutine.step(config);
    disp.delay(1000/config.speed);
  }
}