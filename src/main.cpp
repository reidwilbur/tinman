#include "Ticker.h"

void setup() {
  delay(3001); // 3 second delay for recovery

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Ticker::setup();
}

static char* msg = "SHINY BUTTHOLE";
int msgLen = strlen(msg);
int col = NUM_LEDS - 1;

void loop() {
  Ticker::writeString(col, CRGB::Black, msg);
  col = (col < -(msgLen * MAX_CHAR_WIDTH)) ? NUM_LEDS - 1 : col - 1;
  Ticker::writeString(col, CRGB::White, msg);
  FastLED.show();
  FastLED.delay(1000/FRAMES_PER_SECOND);
}