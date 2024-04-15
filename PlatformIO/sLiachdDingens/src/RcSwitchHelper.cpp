// RcSwitchHelper.cpp

#include "RcSwitchHelper.h"
#include "Settings.h"

// Constructor
RcSwitchHelper::RcSwitchHelper(){
    _bootPos = 0;
    _bToggle = false;   // Taster oder Schalter

    _bBoot = true;  // erster Durchlauf
    _pos = 0;       // aktuelle Position
}

void RcSwitchHelper::begin(uint8_t bootPos, bool bToggleSwitsch = false) {
    _pos = _bootPos = bootPos;
    _bToggle = bToggleSwitsch;
}

uint8_t RcSwitchHelper::getPosition() {
    return _pos;
}

void RcSwitchHelper::setPosition(uint8_t newPos) {
    // Ist SchalterStartPosition das erste mal erreicht, dann darf auch eine andere Position verarbeitet werden
    if((_bBoot && _bootPos == newPos) || _bToggle){
        _bBoot = false;
    }
    if(!_bToggle){
        if(_pos != newPos){
            if(_bBoot) {
                // SchalterStartPosition
                _pos = _bootPos;      
            } else {
                // neue Schalterposition
                _pos = newPos;
            }
        }
    } else {
        _pos = (_pos+1)%2;  // Toggle
    }
}

bool RcSwitchHelper::isToggleSwitch(){
    return _bToggle;
}