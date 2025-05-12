// RcSwitchHelper.h
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

// Interrupt Variablen
struct ITR_STRUCT {
    bool bHasBooking = false;
    bool bIsToggleSwitsch = false;
    unsigned long startTime;    // Time of the rising edge.
    unsigned long pulseLength;  // Pulselenght in uS.
    uint8_t newVal = 5;         // fiktiver Startwert der ungleich der möglichen Schalterpositionen ist
};

class RcSwitchHelper {
    public:
        RcSwitchHelper();
        void begin(uint8_t bootPos, bool bToggleSwitsch);
        bool isToggleSwitch();
        uint8_t getPosition();
        void setPosition(uint8_t);

    private:
        uint8_t _bootPos;   // boot Position
        bool _bToggle;      // ist es ein Taster - dann nur Wechsel zwischen LOW/HIGH möglich

        bool _bBoot;        // Boot - erster Durchlauf
        uint8_t _pos;       // aktuelle Position

};
