#include <Arduino.h>

#include <WiFi.h>
#include <WebServer.h>
#include <mdns.h>
#include <map>

#include "DisplayConfigServer.h"
#include "NetworkConfig.h"

namespace display_config_server {

using namespace display;

WebServer server(80);

static const char FORMAT[] = "text/plain";

static const String RESP_OK("OK\n");
static const String RESP_BAD_REQ("Bad request\n");

static const String ARG_MSG("msg");
static const String ARG_SPEED("speed");
static const String ARG_CLR("clr");
static const String ARG_COLOR_CODE("clrcode");
static const String ARG_MODE("mode");
static const String ARG_SLEEP("sleep");
static const String ARG_BRIGHT("bright");

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
  WiFi.begin(ssid, pswd);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    disp(0,0) = (disp(0,0) == black) ? orange : black;
    disp.show();
  }
  Serial.print("Wifi connected ");
  Serial.println(WiFi.localIP());
  server.on("/", [this](){ handleRoot(); });
  server.on("/mode", HTTP_POST, [this]() { postMode(); });
  server.on("/mode", HTTP_GET, [this]() { getMode(); });
  server.on("/sleep", HTTP_POST, [this]() { postSleep(); });
  server.on("/sleep", HTTP_GET, [this]() { getSleep(); });
  server.onNotFound([this]() { handleNotFound(); });
  server.enableDelay(false);
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
  server.send(200, FORMAT, "hello monster\n");
}

void ConfigServer::handleNotFound() {
  server.send(404, FORMAT, "404: Not found\n");
}

void ConfigServer::postSleep() {
  if (server.hasArg(ARG_SLEEP)) {
    auto sleepVal = server.arg(ARG_SLEEP);
    sleepVal.toUpperCase();
    config.sleep = (sleepVal == String("TRUE"));
  }
  server.send(200, FORMAT, RESP_OK);
}

void ConfigServer::getSleep() {
  if (config.sleep) {
    server.send(200, FORMAT, "true\n");
  } else {
    server.send(200, FORMAT, "false\n");
  }
}

boolean setSpeed(DisplayConfig& config) {
  if (server.hasArg(ARG_SPEED)) {
    auto fields = sscanf(server.arg(ARG_SPEED).c_str(), "%u", &config.speed);
    return fields == 1;
  }
  return false;
}

boolean setBrightness(DisplayConfig& config) {
  if (server.hasArg(ARG_BRIGHT)) {
    auto fields = sscanf(server.arg(ARG_BRIGHT).c_str(), "%u", &config.speed);
    if (fields == 1) {
      return true;
    } else {
      return false;
    }
  }
  return false;
}

void ConfigServer::postMode() {
  Serial.println("postMode");
  if (server.hasArg("mode")) {
    auto mode = server.arg("mode");
    mode.toUpperCase();
    auto routine = routines.getRoutine(mode);
    if (routine != NULL) {
      auto defCfg = routine->getDefaultConfig();
      config.mode = defCfg.mode;
      config.speed = defCfg.speed;
      setSpeed(config);
      setBrightness(config);
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
      server.send(200, FORMAT, RESP_OK);
    } else {
      server.send(400, FORMAT, "Unsupported mode\n");
    }
  } else {
    server.send(400, FORMAT, "Missing mode\n");
  }
}

void ConfigServer::getMode() {
  Serial.println("getMode");
  server.send(200, FORMAT, MODE_STRINGS[config.mode] + "\n");
}

ConfigServer::ConfigServer(display_routine::DisplayRoutines& rts): 
  config({ "", 0xffff00, 0x0, 15, Mode::TICKER, false, 64 }),
  routines(rts) {
}

DisplayConfig& ConfigServer::loop() {
  server.handleClient();
  return config;
}

}