#include "hal/OledDriver.h"

#include "config.h"

void SwitchBus(uint8_t bus);

OledDriver::OledDriver()
    : _left(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET),
      _right(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET)
{
}

bool OledDriver::begin() {
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

    d.setTextSize(2);          // Normal 1:1 pixel font scale
    d.setTextColor(SSD1306_WHITE); // White text on black background
    d.setCursor(0,0);

    switch (vs.id) {
        case VisualStateID::Neutral:

            d.println("Neutral");

            break;

        case VisualStateID::RelaxedHappy:
            d.println("RelaxedHappy");

            break;

        case VisualStateID::Excited:
            d.println("Excited");
            break;

        case VisualStateID::Sad:
            d.println("Sad");
            break;

        case VisualStateID::Angry:
            d.println("Angry");
            break;

        case VisualStateID::Sleeping:
            d.println("Sleeping");
            break;
    }
    d.display();
}

void OledDriver::render(const VisualState& vs) {
    SwitchBus(OLED_BUS_LEFT);
    drawEye(_left, vs, true);

    SwitchBus(OLED_BUS_RIGHT);
    drawEye(_right, vs, false);
}
