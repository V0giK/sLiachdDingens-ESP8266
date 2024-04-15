// accesspoint.h
#pragma once

#include <ESP8266WiFi.h>
#include <ESPAsyncWebServer.h>
//#include <include/WiFiState.h> // WiFiState structure details


class WiFi_WebServerHandling {
    public:
        WiFi_WebServerHandling();
        void AP_starten();
        static void disableWifi();
    private:

        

//extern File fsUploadFile;
//extern AsyncWebServer server;


};