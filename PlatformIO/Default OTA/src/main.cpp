/*
  -----------------------
  ElegantOTA - Async Demo Example
  -----------------------

  NOTE: Make sure you have enabled Async Mode in ElegantOTA before compiling this example!
  Guide: https://docs.elegantota.pro/async-mode/

  Skill Level: Beginner

  This example provides with a bare minimal app with ElegantOTA functionality which works
  with AsyncWebServer.

  Github: https://github.com/ayushsharma82/ElegantOTA
  WiKi: https://docs.elegantota.pro

  Works with both ESP8266 & ESP32

  -------------------------------

  Upgrade to ElegantOTA Pro: https://elegantota.pro

*/

#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
  #include <ESP8266mDNS.h>
#elif defined(ESP32)
  #include <WiFi.h>
  #include <AsyncTCP.h>
#endif

#include <ESPAsyncWebServer.h>
#include <AsyncElegantOTA.h>

const char* ssid = "TP-Link_991A";
const char* password = "71420530";

AsyncWebServer server(80);

unsigned long ota_progress_millis = 0;

void onOTAStart() {
  // Log when OTA has started
  Serial.println("OTA update started!");
  // <Add your own code here>
}

void onOTAProgress(size_t current, size_t final) {
  // Log every 1 second
  if (millis() - ota_progress_millis > 1000) {
    ota_progress_millis = millis();
    Serial.printf("OTA Progress Current: %u bytes, Final: %u bytes\n", current, final);
  }
}

void onOTAEnd(bool success) {
  // Log when OTA has finished
  if (success) {
    Serial.println("OTA update finished successfully!");
  } else {
    Serial.println("There was an error during OTA update!");
  }
  // <Add your own code here>
}

void setup(void) {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.hostname("initialOTA");
  WiFi.begin(ssid, password);
  Serial.println("");

  MDNS.begin("initialOTA");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send(200, "text/plain", "Hi! This is ElegantOTA AsyncDemo.");
  });


  AsyncElegantOTA.begin(&server);    // Start ElegantOTA


  server.begin();
  Serial.println("HTTP server started");

  pinMode(D4, OUTPUT);
  digitalWrite(D4,LOW);
}

void loop(void) {
  MDNS.update();
  AsyncElegantOTA.loop();
}