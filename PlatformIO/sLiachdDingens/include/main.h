// main.h
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

/* Prototypen-dekalration */

void mapSlot2Port(uint8_t);
//void startPrev(bool);
void setAllLightPorts(uint8_t);
void setAllLightPortsOff();
long seqSlotVal(uint8_t, uint8_t, uint8_t);
void handleRcChannels();
void IRAM_ATTR RcCalcChange();
void IRAM_ATTR RcLowPwrChange();
bool getPrev();
void setPrev(bool);
