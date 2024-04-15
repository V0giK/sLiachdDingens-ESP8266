// GlobalFnc.h
#pragma once
#include <Arduino.h>

class GlobalFnc {
    public:
        static uint16_t MA(uint8_t portCnt, uint8_t profileCnt);
        static uint16_t MA(uint8_t portCnt, uint8_t profileCnt, uint8_t slotCnt);
};