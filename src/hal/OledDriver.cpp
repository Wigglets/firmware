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

        case VisualStateID::Relaxed:
            d.println("RelaxedHappy");

            break;

        case VisualStateID::Happy:
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

void OledDriver::renderDebug(const Sensors& sd, const VisualState &vs, const EmotionState &es) {

    Adafruit_SSD1306& d = _right;
    // Render debug info on oled
    SwitchBus(OLED_BUS_RIGHT);
    d.setTextColor(SSD1306_WHITE); // White text on black background

    d.clearDisplay();
    d.setCursor(0, 0);
    d.setTextSize(1);
    d.print("P:"); d.print(es.pleasure);
    d.print(" A:"); d.print(es.arousal);
    d.print(" D:"); d.println(es.dominance);

    d.print("Temp:"); d.println(sd.temperature);
    d.print("Light:"); d.println(sd.light_level);

    d.print("AX:"); d.print(sd.orientation.accelX);
    d.print("AY:"); d.print(sd.orientation.accelY);
    d.print("AZ:"); d.println(sd.orientation.accelZ);
    d.print("GX:"); d.print(sd.orientation.gyroX);
    d.print("GY:"); d.print(sd.orientation.gyroY);
    d.print("GZ:"); d.println(sd.orientation.gyroZ);

    d.display();

    SwitchBus(OLED_BUS_LEFT);
    drawEye(_left, vs, true);
}


void OledDriver::render(const VisualState& vs) {
    SwitchBus(OLED_BUS_LEFT);
    drawEye(_left, vs, true);

    SwitchBus(OLED_BUS_RIGHT);
    drawEye(_right, vs, false);
}
