#include "hal/OledDriver.h"

#include "config.h"
#include "Face.h"

void SwitchBus(uint8_t bus);

Face *face;
static uint8_t faceBuffer[128 * 64 / 8];

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

    face = new Face(/* screenWidth = */ 256, /* screenHeight = */ 64, /* eyeSize = */ 40);
    face->RandomBehavior = true;

    // Automatically blink
    face->RandomBlink = true;
    // Set blink rate
    face->Blink.Timer.SetIntervalMillis(4000);    return true;
}

void OledDriver::drawEye(Adafruit_SSD1306& d, const VisualState& vs, bool isLeft) {


    // const int cx = SCREEN_WIDTH / 2;
    // const int cy = SCREEN_HEIGHT / 2;
    // const int eyeRadius   = 24;
    // const int pupilRadius = 8;
    //
    // d.setTextSize(2);          // Normal 1:1 pixel font scale
    // d.setTextColor(SSD1306_WHITE); // White text on black background
    // d.setCursor(0,0);
    //
    // switch (vs.id) {
    //     case VisualStateID::Neutral:
    //
    //         d.println("Neutral");
    //
    //         break;
    //
    //     case VisualStateID::Relaxed:
    //         d.println("RelaxedHappy");
    //
    //         break;
    //
    //     case VisualStateID::Happy:
    //         d.println("RelaxedHappy");
    //
    //         break;
    //
    //     case VisualStateID::Excited:
    //         d.println("Excited");
    //         break;
    //
    //     case VisualStateID::Sad:
    //         d.println("Sad");
    //         break;
    //
    //     case VisualStateID::Angry:
    //         d.println("Angry");
    //         break;
    //
    //     case VisualStateID::Sleeping:
    //         d.println("Sleeping");
    //         break;
    // }
    // d.display();
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
    u8g2.setI2CAddress(0x3C << 1);
    u8g2.begin();

}

static uint8_t leftBmp[64 * 64 / 8];
static uint8_t rightBmp[64 * 64 / 8];


static void blitWindow(Adafruit_SSD1306& d,
                       const uint8_t* src128x64,
                       int srcCenterX,
                       int winW = 64)
{
    const int srcW = 128;
    const int dstW = 64;
    const int pages = 8;

    int srcX0 = srcCenterX - winW / 2;
    if (srcX0 < 0) srcX0 = 0;
    if (srcX0 > srcW - winW) srcX0 = srcW - winW;

    const int dstX0 = (dstW - winW) / 2;

    uint8_t* dst = d.getBuffer();
    memset(dst, 0, dstW * pages);

    for (int page = 0; page < pages; page++) {
        const uint8_t* srcRow = src128x64 + page * srcW + srcX0;
        uint8_t* dstRow       = dst        + page * dstW + dstX0;
        memcpy(dstRow, srcRow, winW);
    }
}


void OledDriver::render(const VisualState& vs) {
    face->Update();                    // Update ONCE per frame
    const uint8_t* fb = face->GetBufferPtr();

    int cxL = face->LeftEye.CenterX;
    int cxR = face->RightEye.CenterX;

    SwitchBus(OLED_BUS_LEFT);
    blitWindow(_left, fb, cxL, 56);    // try 56 if you see the other eye creep in
    _left.display();

    SwitchBus(OLED_BUS_RIGHT);
    blitWindow(_right, fb, cxR, 56);
    _right.display();
}
