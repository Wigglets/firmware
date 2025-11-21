//
// Created by ub on 11/21/25.
//

#ifndef BLERADIO_H
#define BLERADIO_H
#include "domain/Types.h"

class BleRadio {
public:
    void begin(uint16_t selfId);
    void loop();                        // NimBLEDevice::getScan()->… etc.
    void getSocialContext(SocialContext &ctx) const;
};

#endif //BLERADIO_H
