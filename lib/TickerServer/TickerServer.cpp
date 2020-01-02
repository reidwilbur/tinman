#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

#include "TickerServer.h"
#include "Ticker.h"

namespace TickerServer {

static const char* ssid = "furious office 2G";
static const char* pswd = "";

ESP8266WebServer server(80);

String tickerMsg;
uint32 txtColor = 0xffffff;
uint32 bkgColor = 0x0;

void handleRoot();
void handleNotFound();
void handleMsg();
void handleClr();
void handleBkgClr();

void setup() {
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
  server.onNotFound(handleNotFound);
  server.begin();
}

String& loop() {
  server.handleClient();
  return tickerMsg;
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
  tickerMsg = server.arg("plain");
  Ticker::sanitize(tickerMsg);
  Serial.println(tickerMsg);
  server.send(200, "text/plain", "OK\n");
}

void handleClr() {
  Serial.println("handleClr");
  Serial.println(server.arg("plain"));
  String clr = server.arg("plain");
  if (clr.length() == 8) {
    sscanf(clr.c_str(), "0x%x", &txtColor);
    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "text/plain", "Bad request");
  }
}

void handleBkgClr() {
  Serial.println("handleBkClr");
  Serial.println(server.arg("plain"));
  String clr = server.arg("plain");
  if (clr.length() == 8) {
    sscanf(clr.c_str(), "0x%x", &bkgColor);
    server.send(200, "text/plain", "OK");
  } else {
    server.send(400, "text/plain", "Bad request");
  }
}

uint32 textColor() {
  return txtColor;
}

uint32 backgroundColor() {
  return bkgColor;
}

}