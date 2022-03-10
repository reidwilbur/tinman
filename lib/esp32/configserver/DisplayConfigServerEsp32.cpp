#include <Arduino.h>

#include <WiFi.h>
#include <WebServer.h>
#include <mdns.h>
#include <map>

#include "DisplayConfigServer.h"
#include "NetworkConfig.h"

namespace display_config_server {

WebServer server(80);

static const String ARG_MSG = String("msg");
static const String ARG_SPEED = String("speed");
static const String ARG_CLR = String("clr");
static const String ARG_COLOR_CODE = String("clrcode");
static const String ARG_MODE = String("mode");

unsigned char h2int(char c) {
  if (c >= '0' && c <='9'){
    return((unsigned char)c - '0');
  }
  if (c >= 'a' && c <='f'){
    return((unsigned char)c - 'a' + 10);
  }
  if (c >= 'A' && c <='F'){
    return((unsigned char)c - 'A' + 10);
  }
  return(0);
}

String urldecode(String str) {
  String decodedString="";
  char c;
  char code0;
  char code1;
  for (auto i = 0; i < str.length(); i++){
    c=str.charAt(i);
    if (c == '+'){
      decodedString+=' ';  
    } else if (c == '%') {
      code0=str.charAt(++i);
      code1=str.charAt(++i);
      c = (h2int(code0) << 4) | h2int(code1);
      decodedString += c;
    } else {
      decodedString += c;  
    }
  }
  
  return decodedString;
}

int ConfigServer::start(display::Display& disp) {
  auto orange = CRGB(255, 127, 0);
  auto black = CRGB(0,0,0);

  disp.clear();
  disp(0,0) = orange;
  disp.show();

  WiFi.disconnect();
  WiFi.config(ip, gateway, subnet, dns);
  WiFi.begin(ssid, pswd);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    disp(0,0) = (disp(0,0) == black) ? orange : black;
    disp.show();
  }
  Serial.print("Wifi connected ");
  Serial.println(WiFi.localIP());
  server.on("/", [this](){ handleRoot(); });
  server.on("/speed", HTTP_POST, [this]() { postSpeed(); });
  server.on("/mode", HTTP_POST, [this]() { postMode(); });
  server.on("/mode", HTTP_GET, [this]() { getMode(); });
  server.onNotFound([this]() { handleNotFound(); });
  server.begin();

  esp_err_t res = ESP_FAIL;
  while(res != ESP_OK) {
    delay(100);
    disp(0,0) = (disp(0,0) == black) ? orange : black;
    disp.show();
    res = mdns_init();
  }
  res = mdns_hostname_set("tinman");
  if (res != ESP_OK) {
    Serial.print("mdns hostname set");
    Serial.println(res);
  }
  res = mdns_service_add("tinman", "_http", "_tcp", 80, {}, 0);
  if (res != ESP_OK) {
    Serial.print("mdns add svc failed");
    Serial.println(res);
  }
  Serial.println("Server started");
  disp.clear();
  disp.show();
  return res;
}

void ConfigServer::handleRoot() {
  server.send(200, "text/plain", "hello monster\n");
}

void ConfigServer::handleNotFound() {
  server.send(404, "text/plain", "404: Not found\n");
}

boolean setSpeed(DisplayConfig& config) {
  if (server.hasArg(ARG_SPEED)) {
    auto fields = sscanf(server.arg(ARG_SPEED).c_str(), "%u", &config.speed);
    return fields == 1;
  }
  return false;
}

void ConfigServer::postSpeed() {
  Serial.println("postSpeed");
  auto res = setSpeed(config);
  Serial.println(config.speed);
  if (res) {
    Serial.println("ok request");
    server.send(200, "text/plain", "OK\n");
  } else {
    Serial.println("bad request");
    server.send(400, "text/plain", "Bad request\n");
  }
}

void ConfigServer::getMode() {
  Serial.println("getMode");
  server.send(200, "text/plain", ModeStrings[config.mode] + "\n");
}

static const std::map<String, uint32_t> COLOR_CODES{
  {String("RED"),    0x8B0000},
  {String("ORANGE"), 0xFFA500},
  {String("YELLOW"), 0xFFFF00},
  {String("GREEN"),  0x00FF00},
  {String("BLUE"),   0x0000FF},
  {String("PURPLE"), 0x800080},
  {String("PINK"),   0xFFC0CB},
  {String("WHITE"),  0xFFFFFF}
};

void setTicker(DisplayConfig& config) {
  config.mode = Mode::TICKER;
  config.speed = 30;
  setSpeed(config);
  if (server.hasArg(ARG_MSG)) {
    config.message = server.arg(ARG_MSG);
  }
  if (server.hasArg(ARG_COLOR_CODE)) {
    sscanf(server.arg(ARG_COLOR_CODE).c_str(), "0x%6x", &config.textColor);
  } else if (server.hasArg(ARG_CLR)) {
    String color = server.arg(ARG_CLR);
    color.toUpperCase();
    auto entry = COLOR_CODES.find(color);
    if (entry != COLOR_CODES.end()) {
      config.textColor = entry->second;
    }
  }
}

void setDigRain(DisplayConfig& config) {
  config.mode = Mode::DIGITAL_RAIN;
  config.speed = 8;
  setSpeed(config);
}

void setSparkle(DisplayConfig& config) {
  config.mode = Mode::SPARKLE;
  config.speed = 25;
  setSpeed(config);
}

void setFire(DisplayConfig& config) {
  config.mode = Mode::FIRE;
  config.speed = 15;
  setSpeed(config);
}

void setKitt(DisplayConfig& config) {
  config.mode = Mode::KITT;
  config.speed = 8;
  setSpeed(config);
}

void setStatic(DisplayConfig& config) {
  config.mode = Mode::STATIC;
  config.speed = 30;
  setSpeed(config);
}

void setNyanCat(DisplayConfig& config) {
  config.mode = Mode::NYANCAT;
  config.speed = 7;
  setSpeed(config);
}

static const std::map<String, std::function<void(DisplayConfig&)>> MODE_HANDLERS{
  {ModeStrings[Mode::TICKER], setTicker},
  {ModeStrings[Mode::DIGITAL_RAIN], setDigRain},
  {ModeStrings[Mode::SPARKLE], setSparkle},
  {ModeStrings[Mode::FIRE], setFire},
  {ModeStrings[Mode::KITT], setKitt},
  {ModeStrings[Mode::STATIC], setStatic},
  {ModeStrings[Mode::NYANCAT], setNyanCat}
};

void ConfigServer::postMode() {
  Serial.println("postMode");
  if (server.hasArg("mode")) {
    auto mode = server.arg("mode");
    mode.toUpperCase();
    auto handler = MODE_HANDLERS.find(mode);
    if (handler != MODE_HANDLERS.end()) {
      handler->second(config);
      Serial.println("ok request");
      server.send(200, "text/plain", "OK\n");
    } else {
      Serial.println("bad request, unsupported mode");
      server.send(400, "text/plain", "Unsupported mode\n");
    }
  } else {
    Serial.println("bad request, no mode");
    server.send(400, "text/plain", "Missing mode\n");
  }
}

ConfigServer::ConfigServer(): 
  config({ "", 0xffff00, 0x0, 15, Mode::TICKER }) {
}

DisplayConfig& ConfigServer::loop() {
  server.handleClient();
  return config;
}

}