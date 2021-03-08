#include <Arduino.h>

#include <WiFi.h>
#include <WebServer.h>
#include <mdns.h>

#include "configserver/DisplayConfigServer.h"
#include "display/Display.h"
#include "configserver/NetworkConfig.h"

namespace DisplayConfigServer {

WebServer server(80);

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

  esp_err_t res = ESP_FAIL;
  while(res != ESP_OK) {
    delay(100);
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
  if (server.hasArg("msg")) {
    Serial.println("ok request");
    config.message = server.arg("msg");
    config.message = urldecode(config.message);
    Display::sanitize(config.message);
    server.send(200, "text/plain", "OK\n");
  } else {
    Serial.println("bad request");
    server.send(400, "text/plain", "Bad request");
  }
  Serial.println(config.message);
}

void handleClr() {
  Serial.println("handleClr");
  String clr = server.arg("clr");
  int fields = sscanf(clr.c_str(), "0x%x", &config.textColor);
  if (fields == 1) {
    Serial.println("ok request");
    server.send(200, "text/plain", "OK\n");
  } else {
    Serial.println("bad request");
    server.send(400, "text/plain", "Bad request");
  }
}

void handleBkgClr() {
  Serial.println("handleBkClr");
  String clr = server.arg("bkgclr");
  int fields = sscanf(clr.c_str(), "0x%x", &config.bkgColor);
  if (fields == 1) {
    Serial.println("ok request");
    server.send(200, "text/plain", "OK\n");
  } else {
    Serial.println("bad request");
    server.send(400, "text/plain", "Bad request");
  }
}

void handleSpeed() {
  Serial.println("handleSpeed");
  String speed = server.arg("speed");
  int fields = sscanf(speed.c_str(), "%u", &config.speed);
  Serial.println(config.speed);
  if (fields == 1) {
    Serial.println("ok request");
    server.send(200, "text/plain", "OK\n");
  } else {
    Serial.println("bad request");
    server.send(400, "text/plain", "Bad request");
  }
}

void handleMode() {
  Serial.println("handleMode");
  if (server.hasArg("mode")) {
    String mode = server.arg("mode");
    mode = urldecode(mode);
    mode.toUpperCase();
    if (mode == String("TEXT_SCROLL")) {
      config.mode = TEXT_SCROLL;
      Serial.println("ok request");
      server.send(200, "text/plain", "OK\n");
    } else if (mode == String("DIGITAL_RAIN")) {
      config.mode = DIGITAL_RAIN;
      Serial.println("ok request");
      server.send(200, "text/plain", "OK\n");
    } else {
      Serial.println("bad request");
      server.send(400, "text/plain", "Bad request");
    }
  } else {
    Serial.println("bad request");
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