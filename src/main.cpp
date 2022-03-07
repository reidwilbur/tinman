#include <Arduino.h>
#include <display/Display.h>
#include <configserver/DisplayConfigServer.h>
#include <display/DisplayRoutine.h>

#define ONBOARD_LED 2

using namespace display_config_server;
using namespace display_routine;

Mode lastMode = Mode::TICKER;
display::Display disp = display::Display();

Ticker ticker = Ticker(disp);
Fire fire = Fire(disp);
DigitalRain digitalrain = DigitalRain(disp);
Sparkle sparkle = Sparkle(disp);
Kitt kitt = Kitt(disp);

void setup() {
  Serial.begin(115200);
  delay(3001); // 3 second delay for recovery

  pinMode(ONBOARD_LED, OUTPUT);
  digitalWrite(ONBOARD_LED, LOW);

  display_config_server::setup();
}

DisplayRoutine& getRoutine(const DisplayConfig& config) {
  switch(config.mode) {
    case Mode::TICKER: return ticker;
    case Mode::DIGITAL_RAIN: return digitalrain;
    case Mode::SPARKLE: return sparkle;
    case Mode::FIRE: return fire;
    case Mode::KITT: return kitt;
  }
}

void loop() {
  display_config_server::loop();
  DisplayConfig& config = display_config_server::getConfig();
  bool modeChanged = lastMode != config.mode;
  lastMode = config.mode;
  DisplayRoutine& displayroutine = getRoutine(config);
  if (modeChanged) {
    displayroutine.init();
  }
  displayroutine.step(config);
  disp.show();
  delay(1000/config.speed);
}