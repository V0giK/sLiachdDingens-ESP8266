// BattHelper.h
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