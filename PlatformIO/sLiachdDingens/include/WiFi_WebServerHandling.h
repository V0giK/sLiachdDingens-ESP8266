// accesspoint.h
/*
  Projekt: sLiachdDingens-ESP8266
  Copyright © 2025 V0giK

  Dieses Programm ist freie Software: Du kannst es unter den Bedingungen
  der GNU General Public License, wie von der Free Software Foundation
  veröffentlicht, weitergeben und/oder modifizieren – entweder gemäß
  Version 3 der Lizenz oder (nach Deiner Wahl) einer späteren Version.

  Der vollständige Lizenztext ist in der Datei LICENSE enthalten:
  https://github.com/V0giK/sLiachdDingens-ESP8266/blob/main/LICENSE
*/

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
