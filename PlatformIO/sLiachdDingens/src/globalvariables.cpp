// globalvariables.cpp
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


#include "globalvariables.h"
#include "Settings.h"

EepromFunctions gEeprom;
JsonHelper2 gJsonHelper;
BattHelper gBatt;


/* Slot Variablen */
uint8_t giCurSlot = 0;

/* Leistung an den Ausgängen in % */
uint8_t gLowPwr = 50;

