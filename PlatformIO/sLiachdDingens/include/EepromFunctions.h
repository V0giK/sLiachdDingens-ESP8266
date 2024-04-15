// EepromFunctions.h
#pragma once
#include <Arduino.h>

class EepromFunctions {
    public:
        struct EEPROM_Data {
            String  file;
            String  hwVersion;       // Version Hardware
            uint8_t portCnt;         // Anzahl LightPorts
            uint8_t profileCnt;      // Anzahl Profile
            uint8_t slotCnt;         // Anzahl LightSlots je Durchlauf
            float   battCalibFactor; // Kalibrierungsfaktor zur Spannungsmessung
        };
        //extern struct EEPROM_Data gEeprom;

        EepromFunctions();
        //void begin();

        // EEPROM clearen
        bool eraseEeprom();

        // Daten in EEPROM schreiben
        bool saveEeprom();
        // Daten aus EEPROM lesen
        EEPROM_Data loadEeprom();

        EEPROM_Data getEeprom();
        void setEeprom(EEPROM_Data& eeprom);

        uint8_t getPortCount();
        uint8_t getProfileCount();
        uint8_t getSlotCount();
        float getBattCalibFactor();
        void setBattCalibFactor(float);

    private:
        //EepromFunctions::EEPROM_Data _Eeprom;
        EEPROM_Data _Eeprom;
};