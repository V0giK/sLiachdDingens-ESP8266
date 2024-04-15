// RcSwitchHelper.h
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