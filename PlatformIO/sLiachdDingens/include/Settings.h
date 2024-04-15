// Settings.h
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
