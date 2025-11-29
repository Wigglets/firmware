//
// Created by omi on 11/26/25.
//

#include "HalUtils.h"
#include <Wire.h>

void SwitchBus(uint8_t bus){
    Wire.beginTransmission(0x70);
    Wire.write(1 << bus);
    Wire.endTransmission();
}
