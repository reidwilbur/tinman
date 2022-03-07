#include <Arduino.h>
#include <FastLED.h>
#include <display/Display.h>
#include <configserver/DisplayConfigServer.h>
#include <display/DisplayRoutine.h>

#define ONBOARD_LED 2

using namespace display_config_server;
using namespace display_routine;

Mode lastMode = Mode::TEXT_SCROLL;
display::Display disp = display::Display();

Ticker ticker = Ticker(disp);
DoomFire doomfire = DoomFire(disp);
DigitalRain digitalrain = DigitalRain(disp);
Sparkle sparkle = Sparkle(disp);

void setup() {
  Serial.begin(115200);
  delay(3001); // 3 second delay for recovery

  pinMode(ONBOARD_LED, OUTPUT);
  digitalWrite(ONBOARD_LED, LOW);

  display_config_server::setup();
}

DisplayRoutine& getRoutine(const DisplayConfig& config) {
  switch(config.mode) {
    case Mode::TEXT_SCROLL: return ticker;
    case Mode::DIGITAL_RAIN: return digitalrain;
    case Mode::SPARKLE: return sparkle;
    case Mode::FIRE: return doomfire;
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
  FastLED.show();
  FastLED.delay(1000/config.speed);
}