// GlobalFnc.cpp
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


#include "GlobalFnc.h"
#include "globalvariables.h"
#include "Settings.h"

/**
 * Index für dynamische mehrdimensionale  Arrays erstellen
 * - also alle die mit malloc(...) erstellt wurden
*/
uint16_t GlobalFnc::MA(uint8_t portCnt, uint8_t profileCnt) {
    return portCnt + ( profileCnt * gEeprom.getPortCount());
};

/**
 * Index für dynamische mehrdimensionale  Arrays erstellen
 * - also alle die mit malloc(...) erstellt wurden
*/
uint16_t GlobalFnc::MA(uint8_t portCnt, uint8_t profileCnt, uint8_t slotCnt) {
    return portCnt + ( profileCnt * gEeprom.getPortCount()) + ( slotCnt * (gEeprom.getPortCount()*gEeprom.getProfileCount()));
};
