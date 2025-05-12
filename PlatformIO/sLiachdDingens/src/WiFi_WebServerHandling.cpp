// accesspoint.cpp
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


#include <LittleFS.h>
#include <AsyncElegantOTA.h>
#include "WiFi_WebServerHandling.h"
#include "GlobalFnc.h"

#include "globalvariables.h"
#include "main.h"
#include "Settings.h"

/* Prototypen Decalration */
void handleBody(AsyncWebServerRequest*, uint8_t*, size_t, size_t, size_t);
void handleFileUpload(AsyncWebServerRequest*, String, size_t, uint8_t *, size_t, bool);
void handleFSf ( AsyncWebServerRequest*, const String&);


AsyncWebServer server(80);


WiFi_WebServerHandling::WiFi_WebServerHandling(){
    disableWifi();
}

void WiFi_WebServerHandling::disableWifi(){
    /* WiFi nicht in Flash schreiben */
    WiFi.persistent(false);
    /* WiFi deaktivieren */
    WiFi.setAutoReconnect(false);
    WiFi.disconnect(true);
    WiFi.softAPdisconnect(true);
    WiFi.mode(WIFI_OFF);
}

void WiFi_WebServerHandling::AP_starten()  // INI im Access Point Modus
{
    WiFi.mode(WIFI_AP);
    //IPAddress apIP(192, 168, 4, 1);             // Hier wird IP bestimmt

    IPAddress apIP; apIP.fromString(gJsonHelper.getConfigApIp());
    WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
    WiFi.softAP(gJsonHelper.getConfigApSsid(), gJsonHelper.getConfigApPwd()); // SSID und Passwort des Access Points
    WiFi.hostname(gJsonHelper.getConfigProductName());

    //wifi_station_set_hostname(ap_name.c_str());       // geht nicht bei WIFI_AP

    delay(500);

    Serial.print(F("Starte AP  ")); Serial.println(gJsonHelper.getConfigApSsid());
    Serial.print(F("IP Adresse ")); Serial.println(WiFi.softAPIP());   //Ausgabe aktueller IP des Servers

    server.rewrite("/", "index.html");
    /*
    server.on("/", [](AsyncWebServerRequest *request) {         // index.html an Browser senden
        Serial.println(F("on /"));
        Serial.println(F("send /index.html"));
        request->send(LittleFS, "/index.html", "text/html");
    });
    */

    server.on("/reset", [](AsyncWebServerRequest *request) {
        request->send(200, "text/plain", "Antwort");
        // Alle Ports auf 0 setzen
        setAllLightPortsOff();
        mapSlot2Port(0);

        WiFi.softAPdisconnect(true);
        WiFi.mode(WIFI_OFF);
        delay(100);
        ESP.restart() ;
    });

    server.on("/upload", HTTP_POST, [](AsyncWebServerRequest *request) {  // Datei in ESP hochladen
        if(request->hasParam("datei", true, true)){
            request->send(200);
        }else{
            request->send(500);
        }
        
    }, handleFileUpload);

    server.on("/uploadFinish", HTTP_ANY, [](AsyncWebServerRequest *request) {  // Datei in ESP hochladen
        const char* sRet = "Upload durchgef&uuml;hrt";
        if(request->params() > 0) {
            AsyncWebParameter* p = request->getParam("erg");
            if(String(p->value().c_str()) != "true"){
                sRet = "Upload fehlgeschlagen";
            }
        }
        request->send(200, "text/plain", sRet);
    });

    server.onNotFound([](AsyncWebServerRequest *request) {
#ifdef DEV_SERIAL_OUT
        //Serial.print(F("on NotFound: ")); Serial.println(request->url());
#endif
        handleFSf ( request, request->url() ) ;
    });

    server.onRequestBody(handleBody);

    // Config - RC Kanäle
    server.on("/config_channel", [](AsyncWebServerRequest *request) {

        // Channel 1 - Boot Position
        if(request->hasParam("cmbCh1Pos", true)) {
            gJsonHelper.getSequence()->ch1Boot = request->getParam("cmbCh1Pos", true)->value().toInt();
        }
        // Channel 2 - Boot Position
        if(request->hasParam("cmbCh2Pos", true)) {
            gJsonHelper.getSequence()->ch2Boot = request->getParam("cmbCh2Pos", true)->value().toInt();
        }

        // Save to FS
        if(gJsonHelper.saveJsonSequence()) {
            request->send(200); // ok
        } else {
            request->send(500); // Fehlerhaft
        }
    });


    // Config - AccesPoint
    // Config - thresholdVolt
    server.on("/config_config", [](AsyncWebServerRequest *request) {

        // Übertragen der Daten in gConfig
        // ModelName
        if(request->hasParam("ModelName", true)) {
            gJsonHelper.setConfigModelName( request->getParam("ModelName", true)->value().c_str() );
        }
        // AP_SSID
        if(request->hasParam("AP_SSID", true)) {
            gJsonHelper.setConfigApSsid( request->getParam("AP_SSID", true)->value().c_str() );
        }
        // AP_PWD
        if(request->hasParam("AP_PWD", true)) {
            gJsonHelper.setConfigApPwd( request->getParam("AP_PWD", true)->value().c_str() );
        }
        // Batt - warningVolt
        if(request->hasParam("warningVolt", true)) {
            gJsonHelper.setConfigBattWarningVolt( request->getParam("warningVolt", true)->value().toFloat() );
        }
        // Batt - curOffVolt
        if(request->hasParam("cutOffVolt", true)) {
            gJsonHelper.setConfigBattCutOffVolt( request->getParam("cutOffVolt", true)->value().toFloat() );
        }

        // Save to FS
        if(gJsonHelper.saveJsonConfig())
            request->send(200);
        else 
            request->send(500);
        
    });

    server.on("xxx/LightSequence", [](AsyncWebServerRequest *request) {         // setup.ini an Browser senden
        #ifdef DEV_SERIAL_OUT
        int params = request->params();
        for(int i=0;i<params;i++){
            AsyncWebParameter* p = request->getParam(i);
            if(p->isFile()){ //p->isPost() is also true
                Serial.printf("FILE[%s]: %s, size: %u\n", p->name().c_str(), p->value().c_str(), p->size());
            } else if(p->isPost()){
                Serial.printf("POST[%s]: %s\n", p->name().c_str(), p->value().c_str());
            } else {
                Serial.printf("GET[%s]: %s\n", p->name().c_str(), p->value().c_str());
            }
        }
        #endif

        request->send(200);
    });

/*
    AsyncCallbackJsonWebHandler* handler = new AsyncCallbackJsonWebHandler("saveLightSequence", [](AsyncWebServerRequest *request, JsonVariant &json) {
        //JsonObject& jsonObj = json.as<JsonObject>();
        // ...
        Serial.println("W");
        StaticJsonDocument<400> data;
        if (json.is<JsonArray>())
        {
            data = json.as<JsonArray>();
        }
        else if (json.is<JsonObject>())
        {
            data = json.as<JsonObject>();
        }
        String response;
        serializeJson(data, response);
        request->send(200, "application/json", response);
        Serial.println(response);

    });
    server.addHandler(handler);
*/
    // LightPorts
    server.on("/LightSequence", [](AsyncWebServerRequest *request) { 

        bool bPrev=true;
        if(request->hasParam("mode", true)){
            bPrev = String(request->getParam("mode", true)->value().c_str()).equals("prev");
        }

        // Übertragen in giSlot für Vorschau
        setPrev(true);
        // LightPort - Übertragen in giSlot für Vorschau
        for(byte prof=0; prof < gEeprom.getProfileCount(); prof++){
            for(uint8_t x = 0; x < gEeprom.getPortCount(); x++){
                for(uint8_t y = 0; y < gEeprom.getSlotCount(); y++){
                    String sid = String("LP" + String(x+1) + "P" + String(prof) + "S" + String(y));
                    if(request->hasParam(sid, true)) {
                        uint8_t slt = request->getParam(sid, true)->value().toInt();
                        gJsonHelper.getSequence()->seq[GlobalFnc::MA(x, prof, y)] = slt;
                    }
                }
            }
        }

        // LightPort - Descriptions
        if(!bPrev) {
            for(uint8_t x = 0; x < gEeprom.getPortCount(); x++){
                String sid = String("LPname_" + String(x+1));
                if(request->hasParam(sid, true)) {
                    gJsonHelper.getSequence()->name[x] = request->getParam(sid, true)->value();
                }
            }
        }

        // Leistung - Übertragen in giSlot für Vorschau
        for(byte prof=0; prof < gEeprom.getProfileCount(); prof++){
            for(uint8_t x = 0; x < gEeprom.getPortCount(); x++){
                String sid = String("LP" + String(x+1) + "P" + String(prof) + "P");
                if(request->hasParam(sid, true)) {
                    uint8_t pwr = request->getParam(sid, true)->value().toInt();
                    //if(!bPrev) {
                        gJsonHelper.getSequence()->pwr[GlobalFnc::MA(x, prof)] = pwr;
                    //}
                }
            }
        }
        // Leistung - Verringerte Leistung
        if(request->hasParam("LPP_LOW", true)) {
            uint8_t lowPwrPercent = request->getParam("LPP_LOW", true)->value().toInt();
            gLowPwr = lowPwrPercent;
            if(!bPrev) {
                gJsonHelper.getSequence()->lowPwrPercent = lowPwrPercent;
            }
        }
        setPrev(false);

        if(!bPrev){
            // Save to FS
            if(gJsonHelper.saveJsonSequence()) {
                request->send(200); // ok
            } else {
                request->send(500); // Fehlerhaft
            }
        } else {
            request->send(200);
        }
    });

    // Batt - Faktor kalibrieren
    server.on("/batt_calib", [](AsyncWebServerRequest *request) { 
        bool bRet = false;
        float istVolt = 0.0;
        const char* sError = "";

        if(gBatt.getCellCount() > 0){
            if(request->hasParam("istVolt", true)){
                istVolt = request->getParam("istVolt", true)->value().toFloat();
            }
            if(istVolt != 0.0){
                // Berechnung newFactor = 1 / vBatt * istBatt * curFactor
                if(gBatt.getVolt() != 0.0){
                    gEeprom.setBattCalibFactor( 1.0 / gBatt.getVolt() * istVolt * gEeprom.getBattCalibFactor() );
                }
                /* EEPROM damit die Hardware nur einmal kalibriert werden muss ... bleibt also bei OTA Update beibehalten */
                bRet = gEeprom.saveEeprom();
            }
        } else {
            sError = "Zum kalibrieren muss ein Akku angeschlossen sein.";
        }

        if(bRet) {
            request->send(200, "text/plain", String(gBatt.getVolt())); // ok
        } else {
            request->send(500, "text/plain", sError); // Fehlerhaft
        }
    });
    // Batt - Batteriespannung senden
    server.on("/batt_voltage", [](AsyncWebServerRequest *request) { 
        request->send(200, "text/plain", String(gBatt.getVolt())); // ok
    });
    // Batt - Anzahl Zellen senden
    server.on("/batt_cell", [](AsyncWebServerRequest *request) { 
        request->send(200, "text/plain", String(gBatt.getCellCount())); // ok
    });
    
    // Anzahl Hardware Ports
    server.on("/getPortCnt", [](AsyncWebServerRequest *request) { 
        request->send(200, "text/plain", String(gEeprom.getPortCount())); // ok
    });

    // Anzahl Zeitslots
    server.on("/getSlotCnt", [](AsyncWebServerRequest *request) { 
        request->send(200, "text/plain", String(gEeprom.getSlotCount())); // ok
    });


/*
    if(!MDNS.begin("sLichadDingens.local")){
        while(1) { delay(1000); }
    }
    MDNS.addService("http", "tcp", 80);
*/

    AsyncElegantOTA.begin(&server);    // Start ElegantOTA
    server.begin();
    #ifdef DEV_SERIAL_OUT
    Serial.println(F("HTTP Server gestarted"));
    #endif
}

char* body;
void handleBody(AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
    if(!index){
        //Serial.printf("BodyStart: %u B\n", total);
        body = (char*)malloc(sizeof(char)*total);
    }

    for(size_t i=0; i<len; i++){
        body[index+i] = data[i];
    }

    if(index + len == total){
        //Serial.printf("BodyEnd: %u B\n", total);

/*
        bool bPrevLight = request->url().endsWith("prevLightSequence");
        bool bSaveLight = request->url().endsWith("saveLightSequence");

        //DynamicJsonDocument doc(3072);
        StaticJsonDocument<2048> doc;
        deserializeJson(doc, body);

        if(bPrevLight || bSaveLight){
            //LD_Sequence savSeq;
            if(bSaveLight){
                //readJsonSequence(savSeq, PATH_SEQUENCE);
            }
            // Übertragen in giSlot für Vorschau
            startPrev(true);
            for(uint8_t x = 0; x < gConfig.portCnt; x++){
                for(uint8_t y = 0; y < gConfig.portSlots; y++){
                    String sid = "LP" + String(x+1) + "_S" + String(y);
                    if(doc.containsKey(sid)) {
                        String slt = doc[sid] | "0";
                        giSlot[x][y] = map(slt.toInt(), 0, 8, 0, 1023);
                        if(bSaveLight) {
                            gSequence.seq[x][y] = slt.toInt();
                        }
                    }
                }
            }
            startPrev(false);
            if(bSaveLight){
                saveJsonSequence(gSequence, PATH_SEQUENCE);
            }
        }
*/
        free(body);
    }

    request->send(200, "text/plain", "Antwort");
}

void handleFileUpload(AsyncWebServerRequest *request, String filename, size_t index, uint8_t *data, size_t len, bool final) {
    static File fsUploadFile;
    if(!index){
        if (!filename.startsWith("/")) filename = "/" + filename;
        //Serial.printf("UploadStart: %s\n", filename.c_str()); Serial.println("");
        fsUploadFile = LittleFS.open(PATH_TMPUPLOAD+filename, "w");
    }
    if (fsUploadFile){
        for(size_t i=0; i<len; i++){
            //Serial.write(data[i]);
            fsUploadFile.write(data[i]); //, upload.currentSize);
        }
    }
    if(final){
        //Serial.println(""); Serial.printf("UploadEnd: %s, %u B\n", filename.c_str(), index+len);  Serial.println("");
        if (fsUploadFile)
            fsUploadFile.close();

        bool bMove = gJsonHelper.moveUploadedFile(filename);
        String sMove = "true";
        if(!bMove) sMove = "false";
        request->redirect("/uploadFinish?erg=" + sMove);
    }
}

//******************************************************************************************
//                                H A N D L E F S F                                        *
//******************************************************************************************
// Handling of requesting files from the SPIFFS/PROGMEM. Example: /favicon.ico             *
//******************************************************************************************
void handleFSf ( AsyncWebServerRequest* request, const String& filename )
{
    static String          ct ;                           // Content type
    AsyncWebServerResponse *response ;                    // For extra headers

#ifdef DEV_SERIAL_OUT
    //Serial.printf( "FileRequest received %s", filename.c_str() ) ; Serial.println();
#endif
    ct = "text/html" ;                    // Get content type
    if ( ( ct == "" ) || ( filename == "" ) ) {            // Empty is illegal
        request->send ( 404, "text/plain", "File not found" ) ;
    } else {
        if( !LittleFS.exists(filename) > 0) // Datei in Filesystem nicht vorhanden
            return;
        if ( filename.indexOf ( "lights.html" ) >= 0 )       // Index page is in PROGMEM
        {
            response = request->beginResponse ( LittleFS, filename, ct ) ;
        }
        else
        {
            if( filename.endsWith(".css"))
                ct = "text/css";
            if( filename.endsWith(".js"))
                ct = "text/javascript";
            if( filename.endsWith(".json"))
                ct = "application/json";
                //ct = "text/plain";

#ifdef DEV_SERIAL_OUT
            Serial.printf( "beginResponse %s - %s", filename.c_str(), ct.c_str() ) ; Serial.println();
#endif
            response = request->beginResponse ( LittleFS, filename, ct ) ;
        }
        // Add extra headers
        response->addHeader("Cache-Control", "no-cache");
        request->send ( response ) ;
    }
}
