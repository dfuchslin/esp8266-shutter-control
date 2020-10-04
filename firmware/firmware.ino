// https://www.mischianti.org/2020/05/16/how-to-create-a-rest-server-on-esp8266-and-esp32-startup-part-1/

#include "Arduino.h"
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <WiFiManager.h>


const char* hostname = "shutter-control";
const int upPin = 0;
const int downPin = 2;
 
ESP8266WebServer server(80);

void health() {
    server.send(200, "application/json", "{\"status\": \"ok\"}");
}

void metrics() {
    String message = "# HELP esp8266_up Is this host up\n";
    message += "# HELP esp8266_up gauge\n";
    message += "esp8266_up 1\n";
    server.send(200, "text/plain", message);
}

void up() {
    Serial.println("up triggered");
    server.send(200, "application/json", "{\"status\": \"ok\"}");
    pulseOutput(upPin);
}

void down() {
    Serial.println("down triggered");
    server.send(200, "application/json", "{\"status\": \"ok\"}");
    pulseOutput(downPin);
}

void routing() {
    server.on("/", HTTP_GET, []() {
        server.send(200, F("text/html"),
            F("Welcome to the REST Web Server"));
    });
    server.on(F("/health"), HTTP_GET, health);
    server.on(F("/metrics"), HTTP_GET, metrics);
    server.on(F("/up"), HTTP_GET, up);
    server.on(F("/down"), HTTP_GET, down);
}

void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
}

void resetOutput(int pin) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}

void pulseOutput(int pin) {
  digitalWrite(pin, HIGH);
  delay(150);
  digitalWrite(pin, LOW);
}
 
void setup(void) {
  Serial.begin(115200);
  Serial.println("wifimanager autoconnect start");

  WiFiManager wifiManager;
  wifiManager.setHostname(hostname);
  wifiManager.autoConnect();

  Serial.println("Wifimanager connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("Hostname: ");
  Serial.println(hostname);
 
  if (MDNS.begin(hostname)) {
    Serial.println("MDNS responder started");
  }

  routing();
  server.onNotFound(handleNotFound);
  server.begin();
  
  Serial.println("HTTP server started");

  MDNS.addService("http", "tcp", 80);

  resetOutput(upPin);
  resetOutput(downPin);
}
 
void loop(void) {
  MDNS.update();
  server.handleClient();
}
