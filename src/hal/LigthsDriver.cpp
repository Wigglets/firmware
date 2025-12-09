// hal/LightsDriver.cpp
#include "hal/LigthsDriver.h"

#include "config.h"

LightsDriver::LightsDriver()
    : _strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800)
{
}

bool LightsDriver::begin() {
    _strip.begin();
    _strip.show();        // all off
    _strip.setBrightness(255);
    return true;
}

void LightsDriver::setAll(uint8_t r, uint8_t g, uint8_t b) {
    for (uint16_t i = 0; i < LED_COUNT; ++i) {
        _strip.setPixelColor(i, _strip.Color(r, g, b));
    }
    _strip.show();
}

void LightsDriver::render(const EmotionState& e, const VisualState& vs) {
    uint8_t r = 0, g = 0, b = 0;

    switch (vs.id) {
        case VisualStateID::Relaxed:
            // warm yellow
            r = 255; g = 180; b = 40;
            break;

        case VisualStateID::Happy:

            r = 0; g = 255; b = 0;
            break;

        case VisualStateID::Excited:
            // bright magenta
            r = 255; g = 0;   b = 150;
            break;
        case VisualStateID::Sad:
            // blue
            r = 40;  g = 80;  b = 255;
            break;
        case VisualStateID::Angry:
            // red
            r = 255; g = 40;  b = 20;
            break;
        case VisualStateID::Sleeping:
            // very dim soft blue
            r = 0;   g = 0;   b = 40;
            break;
        case VisualStateID::Neutral:
        default:
            // soft white
            r = 180; g = 180; b = 180;
            break;
    }

    float a = constrain(e.arousal, 0.0f, 1.0f);
    uint8_t brightness = (uint8_t)map((int)(a * 100.0f), 0, 100, 30, 255);

    uint16_t r2 = (r * brightness) / 255;
    uint16_t g2 = (g * brightness) / 255;
    uint16_t b2 = (b * brightness) / 255;

    setAll((uint8_t)r2, (uint8_t)g2, (uint8_t)b2);
}
