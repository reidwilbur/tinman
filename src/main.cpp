#include <Arduino.h>
#include <display/Display.h>
#include <configserver/DisplayConfigServer.h>
#include <display/DisplayRoutine.h>

#define ONBOARD_LED 2

using namespace display_config_server;
using namespace display_routine;
using namespace display;

Mode lastMode = Mode::TICKER;
display::Display disp = display::Display();
DisplayRoutines routines = DisplayRoutines(disp);
ConfigServer server = ConfigServer();

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
  bool modeChanged = lastMode != config.mode;
  lastMode = config.mode;
  DisplayRoutine& displayroutine = routines.getRoutine(config.mode);
  if (modeChanged) {
    displayroutine.init();
  }
  displayroutine.step(config);
  disp.show();
  delay(1000/config.speed);
}