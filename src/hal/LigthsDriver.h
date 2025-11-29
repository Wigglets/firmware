// hal/LightsDriver.h
#ifndef LIGHTSDRIVER_H
#define LIGHTSDRIVER_H

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include "domain/Types.h"

// adjust to your strip
#define LED_PIN      2
#define LED_COUNT    8   // <-- change to the number of LEDs you have

class LightsDriver {
public:
    LightsDriver();

    bool begin();

    // Use both emotion & visual for color/brightness
    void render(const EmotionState& e, const VisualState& vs);

private:
    Adafruit_NeoPixel _strip;

    void setAll(uint8_t r, uint8_t g, uint8_t b);
};

#endif // LIGHTSDRIVER_H
