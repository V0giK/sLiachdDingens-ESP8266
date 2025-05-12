// Settings.h
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

/*******************************************************************************************************************************************************************/

/*********************************
 * Hard-/Softwarespezifikationen *
 *********************************/

// Anzahl Hardwareports
//#define PORTS 5
// Anzahl Profile - momentan für 3-Stufenschalter
//#define PROFILE 3
// Anzahl Zeitslots - es ist alles auf genau 20 ausgelegt (0,06s)
//#define SLOTS 20

/*******************************************************************************************************************************************************************/

/*************************
 * Entwicklung und Debug *
 *************************/

#define SERIAL_SPEED 115200

// Entwickler Ausgaben auf SerialMonitor
//#define DEV_SERIAL_OUT

// lesen/schreiben JsonFiles ausführen
#define READ_JSON_CONFIG
#define SAVE_JSON_CONFIG
#define READ_JSON_SEQUENCE
#define SAVE_JSON_SEQUENCE

// AccessPoint immer an
#define DBG_AP_ON false
