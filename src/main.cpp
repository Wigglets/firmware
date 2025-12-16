#include <Arduino.h>

#include "domain/EmotionEngine.h"
#include "hal/SensorDriver.h"
#include "hal/OledDriver.h"
#include "hal/LigthsDriver.h"

SensorsDriver sensors_driver;
OledDriver    oled;
LightsDriver  lights;
EmotionEngine emotionEngine;

uint32_t lastUpdate = 0;

void setup() {
    Serial.begin(115200);

    sensors_driver.begin();
    if (!oled.begin()) {
        Serial.println("OLED init failed");
        while (true) { delay(1000); }
    }


    if (!lights.begin()) {
        Serial.println("LED strip init failed");
        while (true) { delay(1000); }
    }

    Serial.println("Creature started");
    lastUpdate = millis();

    Personality grumpy;
    grumpy.noise_preference = -0.8f;  // Haat geluid
    grumpy.touch_affinity   = -0.5f;  // Wil niet aangeraakt worden (wordt boos)
    grumpy.light_preference = -0.6f;  // Houdt van donker
    grumpy.irritability     =  0.9f;  // Heel snel geïrriteerd (hoge arousal bij stress)
    grumpy.curiosity        = -0.2f;  // Behoudend
    emotionEngine.setPersonality(grumpy);
}

void loop() {
    uint32_t now = millis();
    uint32_t dt = now - lastUpdate;

    // Voer logica uit (bijv. elke 50ms)
    if (dt >= 50) {
        // 1. Lees sensoren
        Sensors s = sensors_driver.read();

        // 2. Update emoties (PAD model)
        emotionEngine.update(s, dt);

        // 3. Haal resultaat op
        VisualState vs = emotionEngine.getVisualState();
        EmotionState es = emotionEngine.getEmotionState();

        Serial.println();

        // 4. Render output
        oled.renderDebug(s, vs, es);
        lights.render(es, vs); // LightsDriver gebruikt al arousal voor brightness

        lastUpdate = now;

        Serial.print("P:"); Serial.print(es.pleasure);
        Serial.print(" A:"); Serial.print(es.arousal);
        Serial.print(" D:"); Serial.print(es.dominance);
        Serial.println();
    }
}
