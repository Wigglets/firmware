//
// OledDriver.cpp
//

#include "hal/OledDriver.h"

// Deze wordt ergens anders geïmplementeerd (je gebruikt hem al in SensorDriver)
void SwitchBus(uint8_t bus);

OledDriver::OledDriver()
    : _left(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET),
      _right(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET)
{
}

bool OledDriver::begin() {
    // Als je Wire al ergens anders initialiseert kun je dit weghalen of aanpassen
    Wire.begin(21, 22);  // pas aan naar jouw SDA/SCL indien nodig

    // LEFT eye
    SwitchBus(OLED_BUS_LEFT);
    if (!_left.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("Left OLED init failed"));
        return false;
    }
    _left.clearDisplay();
    _left.display();

    // RIGHT eye
    SwitchBus(OLED_BUS_RIGHT);
    if (!_right.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("Right OLED init failed"));
        return false;
    }
    _right.clearDisplay();
    _right.display();

    return true;
}

void OledDriver::drawEye(Adafruit_SSD1306& d, const VisualState& vs, bool isLeft) {
    d.clearDisplay();

    const int cx = SCREEN_WIDTH / 2;
    const int cy = SCREEN_HEIGHT / 2;
    const int eyeRadius   = 24;
    const int pupilRadius = 8;

    switch (vs.id) {
        case VisualStateID::Neutral:
            d.fillCircle(cx, cy, eyeRadius, SSD1306_WHITE);
            d.fillCircle(cx, cy, pupilRadius, SSD1306_BLACK);
            break;

        case VisualStateID::RelaxedHappy:
            d.fillCircle(cx, cy, eyeRadius, SSD1306_WHITE);
            d.fillCircle(cx, cy - 3, pupilRadius, SSD1306_BLACK);
            d.drawLine(cx - 9, cy - 7, cx + 9, cy - 9, SSD1306_WHITE);
            break;

        case VisualStateID::Excited:
            d.drawCircle(cx, cy, eyeRadius, SSD1306_WHITE);
            d.drawCircle(cx, cy, eyeRadius - 1, SSD1306_WHITE);
            d.fillCircle(cx, cy, pupilRadius - 1, SSD1306_WHITE);
            break;

        case VisualStateID::Sad:
            d.fillCircle(cx, cy, eyeRadius, SSD1306_WHITE);
            d.fillCircle(cx, cy + 3, pupilRadius, SSD1306_BLACK);
            d.drawLine(cx - 9, cy - 9, cx + 9, cy - 7, SSD1306_WHITE);
            break;

        case VisualStateID::Angry:
            d.fillCircle(cx, cy, eyeRadius, SSD1306_WHITE);
            d.fillCircle(cx, cy - 2, pupilRadius, SSD1306_BLACK);
            if (isLeft) {
                d.drawLine(cx - 12, cy - 10, cx + 8, cy - 4, SSD1306_WHITE);
            } else {
                d.drawLine(cx + 12, cy - 10, cx - 8, cy - 4, SSD1306_WHITE);
            }
            break;

        case VisualStateID::Sleeping:
            d.drawLine(cx - 10, cy, cx + 10, cy, SSD1306_WHITE);
            d.drawLine(cx - 8, cy, cx - 5, cy + 3, SSD1306_WHITE);
            d.drawLine(cx + 5, cy + 3, cx + 8, cy, SSD1306_WHITE);
            break;
    }

    // oog-openheid (masker boven/onder)
    float op = vs.eye_openness;
    if (op < 1.0f) {
        op = constrain(op, 0.0f, 1.0f);
        int cover = (int)((1.0f - op) * eyeRadius);
        if (cover > 0) {
            d.fillRect(0, 0, SCREEN_WIDTH, cover, SSD1306_BLACK);
            d.fillRect(0, SCREEN_HEIGHT - cover, SCREEN_WIDTH, cover, SSD1306_BLACK);
        }
    }

    d.display();
}

void OledDriver::render(const VisualState& vs) {
    // linker oog
    SwitchBus(OLED_BUS_LEFT);
    drawEye(_left, vs, true);

    // rechter oog
    SwitchBus(OLED_BUS_RIGHT);
    drawEye(_right, vs, false);
}
