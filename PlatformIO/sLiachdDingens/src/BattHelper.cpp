// BattHelper.cpp
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


#include "BattHelper.h"
#include "Settings.h"

BattHelper::BattHelper(){
    _lastWarnPwr = 100;
    _battVoltage = 0.0;
    _cellCount = 0;
};

/**
 * Initialisierungen
 * - messurePin     : Pin an dem gemessen wird
 * - battWarningVolt: V/Cell
 * - battCutOffVolt : V/Cell
 * - battWarnLow    : 0-100% Leistung im LOW Status des Blinkens
*/
void BattHelper::begin(uint8_t messurePin, float warningVolt, float cutOffVolt, float calibFactor, uint8_t warnLow = 5){
    _messurePin = messurePin;
    _warningVolt = warningVolt;
    _cutOffVolt = cutOffVolt;
    _calibFactor = calibFactor;
    _warnLow = warnLow;
}

/**
 * Spannung von A0 lesen und Zellenzahl berechnen
 */
void BattHelper::refresh(){
  static int battRaw = analogRead(_messurePin);
  _battVoltage = battRaw * 12.6 / 1000.0 * _calibFactor;
  _cellCount = calcCellCount(_battVoltage);
}

/**
 * Anzahl Zellen berechnen
 */
uint8_t BattHelper::calcCellCount(float volt){
  if(volt > 13.0)
    return 4;
  else if(volt > 9.0)
    return 3;
  else if(volt > 5.0)
    return 2;
  else if(volt > 2.0)
    return 1;
  return 0;
}

/**
 * nächste WarnPower (0-100%) ermitteln und zurückgeben
*/
uint8_t BattHelper::getWarnPwr(){
    if(!isUndervoltage()){
        return 100;
    }

    if(_lastWarnPwr == 100) {
        _lastWarnPwr = _warnLow;
    } else {
        _lastWarnPwr = 100;
    }
    return _lastWarnPwr;
}

/**
 * Spannung in Volt zurückgeben
*/
float BattHelper::getVolt(){
    return _battVoltage;
}

/**
 * Anzahl Zellen zurückgeben
*/
uint8_t BattHelper::getCellCount() {
    return _cellCount;
}

/**
 * Prüfen ob Unterspannung unterschritten ist
*/
bool BattHelper::isUndervoltage(){
    return _battVoltage < _cellCount * _warningVolt;
}

/**
 * Prüfen ob CutOff Spannung unterschritten ist
*/
bool BattHelper::isCutOff(){
    return _battVoltage < _cellCount * _cutOffVolt;
}
