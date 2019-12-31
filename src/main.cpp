#include "Ticker.h"

void setup() {
  delay(3001); // 3 second delay for recovery

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Ticker::setup();
}

static char* msg = "SHINY BUTTHOLE";
int txtLen = strlen(msg);
int loc = NUM_LEDS - 1;

void loop() {
  // char* msgChar = msg;
  // int txtStart = loc;
  // while(*msgChar) {
  //   int charSize = Ticker::writeChar(txtStart, CRGB::Black, *msgChar);
  //   txtStart += charSize + 1;
  //   msgChar++;
  // }

  // msgChar = msg;
  // //loc = (loc - 1) % (33 * 5);
  // loc = (loc < -(txtLen * 5)) ? NUM_LEDS - 1 : loc - 1;
  // txtStart = loc;
  // while(*msgChar) {
  //   int charSize = Ticker::writeChar(txtStart, CRGB::White, *msgChar);
  //   txtStart += charSize + 1;
  //   msgChar++;
  // }

  Ticker::writeString(loc, CRGB::Black, msg);
  loc = (loc < -(txtLen * 5)) ? NUM_LEDS - 1 : loc - 1;
  Ticker::writeString(loc, CRGB::White, msg);
  FastLED.show();
  FastLED.delay(1000/15);
}