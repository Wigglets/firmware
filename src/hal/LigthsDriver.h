#ifndef LIGHTSDRIVER_H
#define LIGHTSDRIVER_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "domain/Types.h"

#define LED_COUNT    8

class LightsDriver {
public:
    LightsDriver();

    bool begin();
    void render(const EmotionState& e, const VisualState& vs);

private:
    Adafruit_NeoPixel _strip;

    void setAll(uint8_t r, uint8_t g, uint8_t b);
};

#endif // LIGHTSDRIVER_H
