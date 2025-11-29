//
// OledDriver.h
//

#ifndef OLEDDRIVER_H
#define OLEDDRIVER_H

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "domain/Types.h"

#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1

// Pas deze aan naar de TCA9548A-kanalen waar je ogen op zitten
#define OLED_BUS_LEFT   2
#define OLED_BUS_RIGHT  3

class OledDriver {
public:
    OledDriver();

    // return false als init faalt
    bool begin();

    // Teken ogen op basis van VisualState
    void render(const VisualState& vs);

private:
    Adafruit_SSD1306 _left;
    Adafruit_SSD1306 _right;

    void drawEye(Adafruit_SSD1306& d, const VisualState& vs, bool isLeft);
};

#endif // OLEDDRIVER_H
