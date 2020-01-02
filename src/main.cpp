#include "Ticker.h"
#include "TickerServer.h"

void setup() {
  Serial.begin(115200);
  delay(3001); // 3 second delay for recovery

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Ticker::setup();
  TickerServer::setup();
}

int col = NUM_LEDS - 1;
String msgStr;

void loop() {
  TickerServer::loop();
  TickerServer::TickerConfig& config = TickerServer::getConfig();
  if (msgStr != config.message) {
    msgStr = config.message;
    col = NUM_LEDS - 1;
    Ticker::clear();
  }
  Ticker::writeString(col, config.bkgColor, msgStr);
  col = (col < -(((int)msgStr.length()) * MAX_CHAR_WIDTH)) ? NUM_LEDS - 1 : col - 1;
  Ticker::writeString(col, config.textColor, msgStr);
  FastLED.show();
  FastLED.delay(1000/config.speed);
}