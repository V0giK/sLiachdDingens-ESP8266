// JsonHelper.h
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

#ifndef ARDUINOJSON_DECODE_UNICODE
#define ARDUINOJSON_DECODE_UNICODE 0
#endif
#include <ArduinoJson.h>
#include "EepromFunctions.h"
//#include "Settings.h"


class JsonHelper2 {
    public:
        /*
        struct LD_Sequence {
            const char* file;
            const char* version;
            uint8_t     lowPwrPercent;
            uint8_t     ch1Boot;
            uint8_t     ch2Boot;
            const char* name[PORTS];                // name (LightPort)
            uint8_t     pwr[PORTS][PROFILE];        // Power pwr[port] (%)
            uint8_t     seq[PORTS][PROFILE][SLOTS]; // Sequence seq[port][slot]
        };
        */
        class CLS_LD_Sequence {
            public:
                CLS_LD_Sequence(uint8_t portCnt, uint8_t profileCnt, uint8_t slotCnt);
                
                //const char* file;
                //const char*      version;
                uint8_t     lowPwrPercent;
                uint8_t     ch1Boot;
                uint8_t     ch2Boot;
                String      *name; // name (LightPort)          name[PORTS]
                uint8_t     *pwr;  // Power pwr[port] (0-100%)  pwr[PORTS][PROFILE]
                uint8_t     *seq;  // Sequence seq[port][slot]  seq[PORTS][PROFILE][SLOTS]
                //String      name[PORTS];       // name (LightPort)
                //uint8_t     pwr[PORTS][PROFILE];     // Power pwr[port][profile] (%)
                //uint8_t     seq2[PORTS][PROFILE][SLOTS]; // Sequence seq[port][profile][slot]
            private:
        };
        JsonHelper2();
        void begin(uint8_t portCnt, uint8_t profileCnt, uint8_t slotCnt);
        boolean loadJsonConfig();
        boolean saveJsonConfig();
        boolean loadJsonSequence();
        boolean saveJsonSequence();
        boolean readDeleteEraseJsonHW(EepromFunctions& clsEeprom);
        bool moveUploadedFile(String);
        bool moveFile(String , String );
        StaticJsonDocument<512>& getConfigPtr();
        float getConfigBattCutOffVolt();
        void setConfigBattCutOffVolt(float);
        float getConfigBattWarningVolt();
        void setConfigBattWarningVolt(float);
        const char* getConfigApSsid();
        void setConfigApSsid(const char*);
        const char* getConfigApPwd();
        void setConfigApPwd(const char*);
        const char* getConfigApIp();
        void setConfigApIp(const char*);
        const char* getConfigModelName();
        void setConfigModelName(const char*);
        const char* getConfigProductName();

        CLS_LD_Sequence* getSequence();

    private:
        uint8_t _portCount;
        uint8_t _profileCount;
        uint8_t _slotCount;
        StaticJsonDocument<512> _jsonConfig;
        CLS_LD_Sequence *_LD_Sequence;
};
