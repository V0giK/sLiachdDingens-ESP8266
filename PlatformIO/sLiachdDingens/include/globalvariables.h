// globalvariables.h
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
#include <Arduino.h>

#include "EepromFunctions.h"
#include "JsonHelper2.h"
#include "BattHelper.h"

/*******************************************************************************************************************************************************************/

// nur zur Deklaration von Array's hier in globalvariables.h
//#define PORTS 5
//#define SLOTS 20
//#define PROFILE 3


// AP defaults
#define AP_SSID "FlightLight (default)"
#define AP_PWD ""
#define AP_IP "1.2.3.4"

// JSON Pfade
#define PATH_CONFIG "/json/config.json"
#define PATH_SEQUENCE "/json/lightProfil.json"
#define PATH_PRESET "/json/lightPresets.json"
//#define PATH_PRESET_CUSTOM "/json/lightCustomPresets.json"
#define PATH_HW "/json/HW.json"
#define PATH_TMPUPLOAD "/tmpUpload"
#define PATH_JSON "/json"

// Batt
#define BATT_WARN_LOW  5
#define BATT_WARN_HIGH 100
#define BATT_WARN_OFF  100



/*******************************************************************************************************************************************************************/

extern EepromFunctions gEeprom;
extern JsonHelper2 gJsonHelper; 
extern BattHelper gBatt;



/* Slot Variablen */
extern uint8_t giCurSlot;

/* Leistung an den Ausgängen in % */
extern uint8_t gLowPwr;

/* Schalter Poistion */
//extern uint8_t gNewPosCH1;
//extern uint8_t gNewPosCH2;

/* Kalibrierungsfaktor der Batteriespannungsmessung */
//extern float gBattVoltage;
//extern uint8_t gBattCellCount;

/*******************************************************************************************************************************************************************/
