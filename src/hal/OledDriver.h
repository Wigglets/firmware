#ifndef OLEDDRIVER_H
#define OLEDDRIVER_H

#include <Adafruit_SSD1306.h>

#include "SensorDriver.h"
#include "domain/Types.h"

#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1


class OledDriver {
public:
    OledDriver();
    bool begin();
    void render(const VisualState& vs);
    void renderDebug(const Sensors& sd, const VisualState& vs, const EmotionState& es);

private:
    Adafruit_SSD1306 _left;
    Adafruit_SSD1306 _right;

    void drawEye(Adafruit_SSD1306& d, const VisualState& vs, bool isLeft);
};

#endif // OLEDDRIVER_H
