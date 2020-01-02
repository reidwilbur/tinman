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

void handleRoot();
void handleNotFound();
void handleMsg();

void setup() {
  WiFi.begin(ssid, pswd);
  while (WiFi.status() != WL_CONNECTED) {
      delay(100);
  }
  Serial.print("Wifi connected ");
  Serial.println(WiFi.localIP());
  server.on("/", handleRoot);
  server.on("/msg", HTTP_POST, handleMsg);
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
  Serial.println(server.arg("plain"));
  tickerMsg = server.arg("plain");
  Ticker::sanitize(tickerMsg);
  Serial.println(tickerMsg);
  server.send(200, "text/plain", "OK\n");
}

}