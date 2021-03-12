#include <ArduinoOTA.h>

#include "display/Display.h"
#include "configserver/DisplayConfigServer.h"

#define ONBOARD_LED 2

void setup() {
  Serial.begin(115200);
  delay(3001); // 3 second delay for recovery

  pinMode(ONBOARD_LED, OUTPUT);
  digitalWrite(ONBOARD_LED, LOW);

  Display::setup();
  DisplayConfigServer::setup();

  ArduinoOTA.setPort(3232);
  ArduinoOTA.setPasswordHash("0f29f2c45f1e0ce1726ba3d1e24cd6f7");
  ArduinoOTA.setHostname("tinman");

  ArduinoOTA.onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });

  ArduinoOTA.begin();
}

int col = Display::width() - 1;
String msgStr;
DisplayConfigServer::Mode lastMode = DisplayConfigServer::TEXT_SCROLL;

void loop() {
  ArduinoOTA.handle();
  DisplayConfigServer::loop();
  DisplayConfigServer::DisplayConfig& config = DisplayConfigServer::getConfig();
  bool modeChanged = lastMode != config.mode;
  lastMode = config.mode;
  switch (config.mode) {
    case DisplayConfigServer::TEXT_SCROLL:
      if (msgStr != config.message) {
        msgStr = config.message;
        col = Display::width() - 1;
        Display::clear();
      }
      Display::writeString(col, config.bkgColor, msgStr);
      col = (col < -(((int)msgStr.length()) * Display::maxCharWidth())) ? Display::width() - 1 : col - 1;
      Display::writeString(col, config.textColor, msgStr);
      break;
    case DisplayConfigServer::DIGITAL_RAIN:
      Display::stepDigitalRain();
      break;
    case DisplayConfigServer::SPARKLE:
      Display::stepSparkle();
      break;
    case DisplayConfigServer::FIRE:
      if (modeChanged) {
        Display::initFire();
      }
      Display::stepFire();
      break;
  }
  FastLED.show();
  FastLED.delay(1000/config.speed);
  //FastLED.delay(85);
}