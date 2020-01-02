#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include "TickerServer.h"
#include "Ticker.h"

namespace TickerServer {

IPAddress ip(192, 168, 1, 110);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(192, 168, 1, 1);

static const char* ssid = "furious office 2G";
static const char* pswd = "";

ESP8266WebServer server(80);

static TickerConfig config = { "", 0xffffff, 0x0, 15 };

void handleRoot();
void handleNotFound();
void handleMsg();
void handleClr();
void handleBkgClr();
void handleSpeed();

void setup() {
  WiFi.disconnect();
  WiFi.config(ip, gateway, subnet, dns);
  WiFi.begin(ssid, pswd);
  while (WiFi.status() != WL_CONNECTED) {
      delay(100);
  }
  Serial.print("Wifi connected ");
  Serial.println(WiFi.localIP());
  server.on("/", handleRoot);
  server.on("/msg", HTTP_POST, handleMsg);
  server.on("/clr", HTTP_POST, handleClr);
  server.on("/bkgclr", HTTP_POST, handleBkgClr);
  server.on("/speed", HTTP_POST, handleSpeed);
  server.onNotFound(handleNotFound);
  server.begin();
}

void loop() {
  server.handleClient();
}

void handleRoot() {
  server.send(200, "text/plain", "hello monster");
}

void handleNotFound() {
  server.send(404, "text/plain", "404: Not found");
}

void handleMsg() {
  Serial.println("handleMsg");
  Serial.println(server.arg("plain"));
  config.message = server.arg("plain");
  if (config.message.startsWith("msg=")) {
    config.message = config.message.substring(4);
    Ticker::sanitize(config.message);
    server.send(200, "text/plain", "OK\n");
  } else {
    server.send(400, "text/plain", "Bad request");
  }
  Serial.println(config.message);
}

void handleClr() {
  Serial.println("handleClr");
  Serial.println(server.arg("plain"));
  String clr = server.arg("plain");
  int fields = sscanf(clr.c_str(), "clr=0x%x", &config.textColor);
  if (fields == 1) {
    server.send(200, "text/plain", "OK\n");
  } else {
    server.send(400, "text/plain", "Bad request");
  }
}

void handleBkgClr() {
  Serial.println("handleBkClr");
  Serial.println(server.arg("plain"));
  String clr = server.arg("plain");
  int fields = sscanf(clr.c_str(), "clr=0x%x", &config.bkgColor);
  if (fields == 1) {
    server.send(200, "text/plain", "OK\n");
  } else {
    server.send(400, "text/plain", "Bad request");
  }
}

void handleSpeed() {
  Serial.println("handleSpeed");
  Serial.println(server.arg("plain"));
  String speed = server.arg("plain");
  int fields = sscanf(speed.c_str(), "speed=%u", &config.speed);
  Serial.println(config.speed);
  if (fields == 1) {
    server.send(200, "text/plain", "OK\n");
  } else {
    server.send(400, "text/plain", "Bad request");
  }
}

TickerConfig& getConfig() {
  return config;
}

}