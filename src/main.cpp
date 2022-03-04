//#include <ArduinoOTA.h>
#include <Arduino.h>
#include <FastLED.h>
#include <display/Display.h>
#include <configserver/DisplayConfigServer.h>
#include <display/DisplayRoutine.h>

#define ONBOARD_LED 2

void setup() {
  Serial.begin(115200);
  delay(3001); // 3 second delay for recovery

  pinMode(ONBOARD_LED, OUTPUT);
  digitalWrite(ONBOARD_LED, LOW);

  //Display::setup();
  DisplayConfigServer::setup();
}

//int col = Display::width() - 1;
//String msgStr;
DisplayConfigServer::Mode lastMode = DisplayConfigServer::TEXT_SCROLL;
Display::Display display = Display::Display();
DisplayRoutine::Ticker ticker = DisplayRoutine::Ticker(display);
DisplayRoutine::DoomFire doomfire = DisplayRoutine::DoomFire(display);

void loop() {
  //ArduinoOTA.handle();
  DisplayConfigServer::loop();
  DisplayConfigServer::DisplayConfig& config = DisplayConfigServer::getConfig();
  bool modeChanged = lastMode != config.mode;
  lastMode = config.mode;
  switch (config.mode) {
    case DisplayConfigServer::TEXT_SCROLL:
      // if (msgStr != config.message) {
      //   msgStr = config.message;
      //   col = Display::width() - 1;
      //   Display::clear();
      // }
      // Display::writeString(col, config.bkgColor, msgStr);
      // col = (col < -(((int)msgStr.length()) * Display::maxCharWidth())) ? Display::width() - 1 : col - 1;
      // Display::writeString(col, config.textColor, msgStr);
      if (modeChanged) {
        ticker.init();
      }
      ticker.step(config);
      break;
    case DisplayConfigServer::FIRE:
      if (modeChanged) {
        doomfire.init();
      }
      doomfire.step(config);
    default:
      break;
    // case DisplayConfigServer::DIGITAL_RAIN:
    //   Display::stepDigitalRain();
    //   break;
    // case DisplayConfigServer::SPARKLE:
    //   Display::stepSparkle();
    //   break;
    // case DisplayConfigServer::FIRE:
    //   if (modeChanged) {
    //     Display::initFire();
    //   }
    //   Display::stepFire();
    //   break;
  }
  FastLED.show();
  FastLED.delay(1000/config.speed);
  //FastLED.delay(85);
}