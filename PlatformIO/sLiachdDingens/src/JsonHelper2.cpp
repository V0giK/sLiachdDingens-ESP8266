// JsonHelper2.cpp

#include "JsonHelper2.h"
#include <LittleFS.h>
#include <AsyncJson.h> // wird im AsyncWebServer verwendet

#include "globalvariables.h"
#include "GlobalFnc.h"
#include "Settings.h"


JsonHelper2::CLS_LD_Sequence::CLS_LD_Sequence(uint8_t portCnt, uint8_t profileCnt, uint8_t slotCnt){
    name = (String*) malloc(portCnt * sizeof(String));
    pwr  = (uint8_t *) malloc(portCnt * profileCnt * sizeof(uint8_t));
    seq  = (uint8_t *) malloc(portCnt * profileCnt * slotCnt * sizeof(uint8_t));
}
JsonHelper2::CLS_LD_Sequence* JsonHelper2::getSequence(){
    return _LD_Sequence;
}


JsonHelper2::JsonHelper2(){
    //
}

void JsonHelper2::begin(uint8_t portCnt, uint8_t profileCnt, uint8_t slotCnt){
    _portCount = portCnt;
    _profileCount = profileCnt;
    _slotCount = slotCnt;

    _LD_Sequence = new CLS_LD_Sequence(portCnt, profileCnt, slotCnt);
}


/**
 * config.json einlesen
 * 
 * config - Datenstruktur die befüllt werden soll
 */
boolean JsonHelper2::loadJsonConfig(){
#ifdef READ_JSON_CONFIG
    File file = LittleFS.open (PATH_CONFIG, "r" ) ;
    // Allocate a temporary JsonDocument
    //StaticJsonDocument<512> doc;
    // Deserialize the JSON document
    DeserializationError error = deserializeJson(_jsonConfig, file);
    if (error){
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        Serial.println(F("Failed to read file, using default configuration"));
    }

#ifdef DEV_SERIAL_OUT
    Serial.print("Config capacity(): "); Serial.print(_jsonConfig.capacity()); Serial.print(" - memoryUsage(): "); Serial.println(_jsonConfig.memoryUsage());
#endif

    // Copy values from the JsonDocument to the Config
    /*
    config.file             = _jsonConfig["file"]                | "config";
    config.productName      = _jsonConfig["ProductName"]         | "";
    config.fwVersion        = _jsonConfig["FwVersion"]           | "";
    config.webVersion       = _jsonConfig["WebVersion"]          | "";
    config.modelName        = _jsonConfig["ModelName"]           | "";
    */

    file.close();

    if(!strlen(getConfigApSsid()))   { setConfigApSsid(AP_SSID); setConfigApPwd(""); }
    if(!strlen(getConfigApPwd()) && strlen(getConfigApPwd()) < 8) { setConfigApPwd(AP_PWD); }  // Passwort muss mindestens 0 oder 8 Zeichen haben, ansonsten gibt es beim AP Nebenwirkungen
#endif
    return true;
}

/**
 * config.json speichern
 * 
 * config - Datenstruktur die gespeichert werden soll
 * file   - die zu speichernde Config Datei
 */
boolean JsonHelper2::saveJsonConfig(){
#ifdef SAVE_JSON_CONFIG

    // Allocate a temporary JsonDocument
    //StaticJsonDocument<512> doc;

    _jsonConfig["file"] = "config";

    // Copy values from the config to JsonDocument
    /*
    doc["file"]                = "config";
    doc["ProductName"]         = config.productName;
    doc["FwVersion"]           = config.fwVersion;
    doc["ModelName"]           = config.modelName;
    doc["AP"]["ssid"]          = gJsonHelper.getConfigApSsid();
    doc["AP"]["pwd"]           = gJsonHelper.getConfigApPwd();
    doc["AP"]["ip"]            = gJsonHelper.getConfigApIp();
    doc["Batt"]["warningVolt"] = gJsonHelper.getConfigBattWarningVolt();
    doc["Batt"]["cutOffVolt"]  = gJsonHelper.getConfigBattCutOffVolt();
    */

    //if(doc.overflowed())
    //    Serial.println(F("DynamicJsonDocument ist zu klein."));    

#ifdef DEV_SERIAL_OUT
    Serial.print("saveConfig capacity(): "); Serial.print(_jsonConfig.capacity()); Serial.print(" - memoryUsage(): "); Serial.println(_jsonConfig.memoryUsage());
#endif

    // Serialize the JSON document
    File file = LittleFS.open(PATH_CONFIG, "w+" ) ;
    if(serializeJsonPretty(_jsonConfig, file) == 0) {
        Serial.println(F("Failed to write to file"));
    }
    file.close();
#endif

    return true;
}

StaticJsonDocument<512>& JsonHelper2::getConfigPtr(){
    return _jsonConfig;
}

float JsonHelper2::getConfigBattCutOffVolt(){
    return _jsonConfig["Batt"]["cutOffVolt"] | 3.5;
}
void JsonHelper2::setConfigBattCutOffVolt(float val){
    _jsonConfig["Batt"]["cutOffVolt"] = val;
}

float JsonHelper2::getConfigBattWarningVolt(){
    return _jsonConfig["Batt"]["warningVolt"] | 3.5;
}
void JsonHelper2::setConfigBattWarningVolt(float val){
    _jsonConfig["Batt"]["warningVolt"] = val;
}

const char* JsonHelper2::getConfigApSsid(){
    return _jsonConfig["AP"]["ssid"] | AP_SSID;
}
void JsonHelper2::setConfigApSsid(const char* val){
    _jsonConfig["AP"]["ssid"] = val;
}

const char* JsonHelper2::getConfigApPwd(){
    return _jsonConfig["AP"]["pwd"] | AP_PWD;
}
void JsonHelper2::setConfigApPwd(const char* val){
    _jsonConfig["AP"]["pwd"] = val;
}

const char* JsonHelper2::getConfigApIp(){
    return _jsonConfig["AP"]["ip"] | AP_IP;
}
void JsonHelper2::setConfigApIp(const char* val){
    _jsonConfig["AP"]["ip"] = val;
}

const char* JsonHelper2::getConfigModelName(){
    return _jsonConfig["ModelName"] | "Airplane";
}
void JsonHelper2::setConfigModelName(const char* val){
    _jsonConfig["ModelName"] = val;
}

const char* JsonHelper2::getConfigProductName(){
    return _jsonConfig["ProductName"];
}



/**
 * Sequence einlesen
 * 
 * sequence - Datenstruktur die befüllt werden soll
 * file     - die zu lesende Sequence Datei
 */
boolean JsonHelper2::loadJsonSequence(){
#ifdef READ_JSON_SEQUENCE

    File file = LittleFS.open(PATH_SEQUENCE, "r" ) ;
    // Allocate a temporary JsonDocument
    DynamicJsonDocument doc(6656);
    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, file);
    #ifdef DEV_SERIAL_OUT
    if (error){
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        Serial.println(F("Failed to read file, using default LightSequenze"));
    }
    if(doc.overflowed())
        Serial.println(F("DynamicJsonDocument ist zu klein."));
    #endif

#ifdef DEV_SERIAL_OUT
   Serial.print("readSequence capacity(): "); Serial.print(doc.capacity()); Serial.print(" - memoryUsage(): "); Serial.println(doc.memoryUsage());
#endif

    // Copy values from the JsonDocument to the Profil
    /*
    sequence.file          = doc["file"]          | "profil";
    sequence.version       = doc["Version"]       | "";
    sequence.lowPwrPercent = doc["lowPwrPercent"] | 50;
    sequence.ch1Boot       = doc["ch1Boot"]       | 0;
    sequence.ch2Boot       = doc["ch2Boot"]       | 0;
    */

    //_LD_Sequence->file          = doc["file"]          | "profil";
    //_LD_Sequence->version       = doc["Version"]       | "0.0";
    _LD_Sequence->lowPwrPercent = doc["lowPwrPercent"] | 50;
    _LD_Sequence->ch1Boot       = doc["ch1Boot"]       | 0;
    _LD_Sequence->ch2Boot       = doc["ch2Boot"]       | 0;

    for(uint8_t x=0; x < doc["ports"].size(); x++){
        uint8_t curPort = doc["ports"][x]["port"];
        if(--curPort < _portCount){
            _LD_Sequence->name[curPort] = doc["ports"][curPort]["name"] | String("LightPort "+String(x+1));
            for(uint8_t prof=0; prof < doc["ports"][curPort]["profile"].size(); prof++){
                if(prof < _profileCount) {
                    _LD_Sequence->pwr[GlobalFnc::MA(curPort, prof)]  = doc["ports"][curPort]["profile"][prof]["pwr"]   | 14;
                    for(uint8_t y=0; y < _slotCount; y++){
                        _LD_Sequence->seq[GlobalFnc::MA(curPort,prof,y)] = doc["ports"][curPort]["profile"][prof]["seq"][y] | 0;  // wenn Anzahl HW-Ports > als in File --> Rest mit LichtAus belegen. (0)
                    }
                }
            }
        }
    }

    file.close();
#endif

    return true;
}

/**
 * Sequence speichern
 * 
 * sequence - Datenstruktur die gespeichert werden soll
 * file     - die zu speichernde Sequence Datei
 */
boolean JsonHelper2::saveJsonSequence(){
#ifdef SAVE_JSON_SEQUENCE

    // Allocate a temporary JsonDocument
    DynamicJsonDocument doc(6656);

    // Copy values from the sequence to JsonDocument
    /*
    doc["file"]          = "profil";
    doc["Version"]       = sequence.version;
    doc["lowPwrPercent"] = sequence.lowPwrPercent;
    doc["ch1Boot"]       = sequence.ch1Boot;
    doc["ch2Boot"]       = sequence.ch2Boot;
    */

    doc["file"]          = "profil";
    //doc["Version"]       = _LD_Sequence->version;
    doc["lowPwrPercent"] = _LD_Sequence->lowPwrPercent;
    doc["ch1Boot"]       = _LD_Sequence->ch1Boot;
    doc["ch2Boot"]       = _LD_Sequence->ch2Boot;


    for(byte port=0; port < _portCount; port++){
        doc["ports"][port]["port"] = (port+1);
        doc["ports"][port]["name"] = _LD_Sequence->name[port];
        for(byte prof=0; prof < _profileCount; prof++){
            doc["ports"][port]["profile"][prof]["pwr"]  = _LD_Sequence->pwr[GlobalFnc::MA(port, prof)];
            for(byte y=0; y < _slotCount; y++){
                doc["ports"][port]["profile"][prof]["seq"][y] = _LD_Sequence->seq[GlobalFnc::MA(port,prof,y)];
            }
        }
    }

    #ifdef DEV_SERIAL_OUT
    if(doc.overflowed())
        Serial.println(F("DynamicJsonDocument ist zu klein."));    
    #endif

#ifdef DEV_SERIAL_OUT
    Serial.print("saveSequence capacity(): "); Serial.print(doc.capacity()); Serial.print(" - memoryUsage(): "); Serial.println(doc.memoryUsage());
#endif

    // Serialize the JSON document
    File file = LittleFS.open(PATH_SEQUENCE, "w+" ) ;
    if(serializeJsonPretty(doc, file) == 0) {
        Serial.println(F("Failed to write to file"));
    }
    file.close();
#endif

    return true;
}

/**
 * HW.json einlesen und löschen
 * 
 * eeprom - Datenstruktur die befüllt werden soll
 */
boolean JsonHelper2::readDeleteEraseJsonHW(EepromFunctions& clsEeprom){
    File file = LittleFS.open (PATH_HW, "r" ) ;

    // Allocate a temporary JsonDocument
    StaticJsonDocument<192> doc;
    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, file);

    #ifdef DEV_SERIAL_OUT
    if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        Serial.println(F("Failed to read file, using default EEPROM"));
    }
    #endif

    if(doc.containsKey("file") && doc["file"] == "hw") {
        // Copy values from the JsonDocument to the Config
        if(doc.containsKey("erase") && doc["erase"] == 1)
            clsEeprom.eraseEeprom();
        clsEeprom.loadEeprom();

        EepromFunctions::EEPROM_Data strcEeprom = clsEeprom.getEeprom();        
        strcEeprom.hwVersion       = doc["HwVersion"]   | "";
        strcEeprom.battCalibFactor = doc["CalibFactor"] | 1.0;
        strcEeprom.portCnt         = doc["PortCnt"]     | _portCount;
        strcEeprom.profileCnt      = doc["ProfileCnt"]  | _profileCount;
        strcEeprom.slotCnt         = doc["SlotCnt"]     | _slotCount;

        clsEeprom.setEeprom(strcEeprom);
    }

    file.close();
    bool bRet = LittleFS.remove(PATH_HW);

    return bRet;
}

bool JsonHelper2::moveUploadedFile(String tmpUploadedFile){
    bool bRet = false;

    // mit Pfad aufbereiten
    if (!tmpUploadedFile.startsWith("/")) tmpUploadedFile = "/" + tmpUploadedFile;
    tmpUploadedFile = PATH_TMPUPLOAD + tmpUploadedFile;

    File file = LittleFS.open(tmpUploadedFile, "r" ) ;

    // Allocate Filter
    StaticJsonDocument<16> filter;
    filter["file"] = true;
    // Allocate JsonDocument
    StaticJsonDocument<125> doc;
    // Deserialize the JSON document
    DeserializationError error = deserializeJson(doc, file, DeserializationOption::Filter(filter));
    #ifdef DEV_SERIAL_OUT
    if (error)
        Serial.println("Failed to read file. "+tmpUploadedFile);
    if(doc.overflowed())
        Serial.println(F("DynamicJsonDocument ist zu klein."));
    #endif
    file.close();

#ifdef DEV_SERIAL_OUT
    Serial.print("moveUploadedFile capacity(): "); Serial.print(doc.capacity()); Serial.print(" - memoryUsage(): "); Serial.println(doc.memoryUsage());
#endif

    if(doc.containsKey("file")){
        String sFile = doc["file"]; 
        sFile.toLowerCase();
        if(sFile == "profil"){
            return moveFile(tmpUploadedFile, PATH_SEQUENCE);
        } else if(sFile == "config"){
            return moveFile(tmpUploadedFile, PATH_CONFIG);
        } else if(sFile == "presets"){
            return moveFile(tmpUploadedFile, PATH_PRESET);
        } else if(sFile == "hw"){
            return moveFile(tmpUploadedFile, PATH_HW);
        }
    }

    LittleFS.remove(tmpUploadedFile);

    return bRet;
}

/**
 * Datei im FS verschieben inkl. rename
 * 
 * fileFrom - Ausgangsdatei inkl. Pfad
 * fileTo   - Zieldatei inkl. Pfad
 */
bool JsonHelper2::moveFile(String fileFrom, String fileTo)
{
    bool bRet = true;
    char ibuffer[64];  // buffer

    #ifdef DEV_SERIAL_OUT
    Serial.println("Move File "+fileFrom+" to File "+fileTo);
    #endif

    //if (LittleFS.exists(fileTo) == true) {
    //    LittleFS.remove(fileTo);
    //}

    File fFrom = LittleFS.open(fileFrom, "r");  // open source file to read
    if (!fFrom) {
        bRet = false;
        #ifdef DEV_SERIAL_OUT
        Serial.println("MovieFile error 1");  //debug
        #endif
    }

    File fTo = LittleFS.open(fileTo, "w");  // open destination file to write
    if (!fTo) {
        bRet = false;
        #ifdef DEV_SERIAL_OUT
        Serial.println("MoveFile errror 2");    //debug
        #endif
    }
   
    if(bRet) {
        while (fFrom.available() > 0) {
            byte i = fFrom.readBytes(ibuffer, 64); // i = number of bytes placed in buffer from file fFrom
            fTo.write(ibuffer, i);                 // write i bytes from buffer to file fTo
        }
    }
   
    fTo.close(); // done, close the destination file
    fFrom.close(); // done, close the source file

    bRet = bRet && LittleFS.remove(fileFrom);

    return bRet;
}
