#ifndef __DisplayConfig_h_INCLUDED__
#define __DisplayConfig_h_INCLUDED__

#include <Arduino.h>

namespace display {

enum Mode { 
  DIGITAL_RAIN, 
  TICKER, 
  SPARKLE, 
  FIRE, 
  KITT, 
  STATIC, 
  NYANCAT 
};

static const std::vector<String> MODE_STRINGS {
  String("DIGITAL_RAIN"),
  String("TICKER"),
  String("SPARKLE"),
  String("FIRE"),
  String("KITT"),
  String("STATIC"),
  String("NYANCAT")
};

struct DisplayConfig {
  String message;
  uint32_t textColor;
  uint32_t bkgColor;
  uint32_t speed;
  Mode mode;
  bool sleep;
  uint8_t brightness;
};

}

#endif