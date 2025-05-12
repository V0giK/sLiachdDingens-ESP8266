// BattHelper.h
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

class BattHelper {
    public:
        BattHelper();
        void begin(uint8_t messurePin, float warningVolt, float cutOffVolt, float calibFactor, uint8_t warnLow);
        void refresh();
        static uint8_t calcCellCount(float volt);
        float getVolt();
        uint8_t getCellCount();
        bool isUndervoltage();
        bool isCutOff();
        uint8_t getWarnPwr();

    private:
        uint8_t _messurePin;
        float _warningVolt;
        float _cutOffVolt;
        float _calibFactor;
        uint8_t _warnLow;

        float _battVoltage;
        uint8_t _cellCount;
        uint8_t _lastWarnPwr;
};
