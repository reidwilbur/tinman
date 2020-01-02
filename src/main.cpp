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
  String newMsgStr = TickerServer::loop();
  if (msgStr != newMsgStr) {
    msgStr = newMsgStr;
    col = NUM_LEDS - 1;
    Ticker::clear();
  }
  Ticker::writeString(col, TickerServer::backgroundColor(), msgStr);
  col = (col < -(((int)msgStr.length()) * MAX_CHAR_WIDTH)) ? NUM_LEDS - 1 : col - 1;
  Ticker::writeString(col, TickerServer::textColor(), msgStr);
  FastLED.show();
  FastLED.delay(1000/FRAMES_PER_SECOND);
}