#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>

#include "configserver/DisplayConfigServer.h"
#include "display/Display.h"
#include "configserver/NetworkConfig.h"

namespace DisplayConfigServer {

ESP8266WebServer server(80);

static DisplayConfig config = { "", 0xffffff, 0x0, 15, TEXT_SCROLL };

void handleRoot();
void handleNotFound();
void handleMsg();
void handleClr();
void handleBkgClr();
void handleSpeed();
void handleMode();

String urldecode(String);
unsigned char h2int(char);

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
  server.on("/mode", HTTP_POST, handleMode);
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
    config.message = urldecode(config.message.substring(4));
    Display::sanitize(config.message);
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

void handleMode() {
  Serial.println("handleMode");
  Serial.println(server.arg("plain"));
  String mode = server.arg("plain");
  if (mode.startsWith("mode=")) {
    mode = urldecode(mode.substring(5));
    mode.toUpperCase();
    if (mode == String("TEXT_SCROLL")) {
      config.mode = TEXT_SCROLL;
      server.send(200, "text/plain", "OK\n");
    } else if (mode == String("DIGITAL_RAIN")) {
      config.mode = DIGITAL_RAIN;
      server.send(200, "text/plain", "OK\n");
    } else {
      server.send(400, "text/plain", "Bad request");
    }
  } else {
    server.send(400, "text/plain", "Bad request");
  }
  Serial.println(config.message);
}

DisplayConfig& getConfig() {
  return config;
}

String urldecode(String str)
{
  String encodedString="";
  char c;
  char code0;
  char code1;
  for (uint i =0; i < str.length(); i++){
    c=str.charAt(i);
    if (c == '+'){
      encodedString+=' ';  
    } else if (c == '%') {
      i++;
      code0=str.charAt(i);
      i++;
      code1=str.charAt(i);
      c = (h2int(code0) << 4) | h2int(code1);
      encodedString+=c;
    } else {
      encodedString+=c;  
    }
  }
  
  return encodedString;
}

unsigned char h2int(char c)
{
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

}