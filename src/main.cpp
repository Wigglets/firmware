#include <Arduino.h>

#include "domain/Types.h"
#include "hal/SensorDriver.h"
#include "hal/OledDriver.h"
#include "hal/LigthsDriver.h"

SensorsDriver sensors_driver;
OledDriver    oled;
LightsDriver  lights;

// één persoonlijkheid voor dit wezen
Personality personality = {
    .noise_preference = -0.4f,
    .touch_affinity   =  0.8f,
    .light_preference = 200.0f,  // lux waar hij blij van wordt
    .irritability     =  0.4f,
    .curiosity        =  0.7f
};

EmotionState emotion = { 0.0f, 0.3f };  // neutraal, beetje wakker
uint32_t lastUpdateMs = 0;

// --- sensoren + persoonlijkheid -> emotie ---
void updateEmotion(const Sensors& s,
                   const Personality& p,
                   EmotionState& e,
                   float dt)
{
    float dV = 0.0f;
    float dA = 0.0f;

    // natuurlijke demping
    e.valence *= 0.98f;
    e.arousal *= 0.98f;

    // touch
    bool touched = false;
    for (int i = 0; i < 7; ++i) {
        if (s.touch_points[i]) {
            touched = true;
            break;
        }
    }
    if (touched) {
        dV += 0.4f * p.touch_affinity;
        dA += 0.2f * (0.3f + p.irritability);
    }

    // schudden
    if (s.is_shaken) {
        dA += 0.6f + 0.4f * p.irritability;
        dV -= 0.3f * p.irritability;
    }

    // licht
    if (p.light_preference > 0.0f && s.light_level > 0.0f) {
        float normErr = (s.light_level - p.light_preference) / p.light_preference;
        if (normErr > 0.3f) {
            float stress = normErr;
            dV -= 0.3f * stress;
            dA += 0.2f * stress;
        } else if (normErr < -0.3f) {
            float calm = -normErr;
            dA -= 0.2f * calm;
        }
    }

    // geluid (als je sound_db gaat vullen)
    if (s.sound_db > 0.0f) {
        float soundLevel = constrain((s.sound_db - 40.0f) / 40.0f, 0.0f, 1.0f);
        dA += soundLevel * 0.5f * p.curiosity;
        dV += soundLevel * 0.3f * p.noise_preference;
    }

    e.valence += dV * dt;
    e.arousal += dA * dt;

    e.valence = constrain(e.valence, -1.0f, 1.0f);
    e.arousal = constrain(e.arousal,  0.0f, 1.0f);
}

// --- emotie -> visuele state ---
VisualState decideVisual(const EmotionState& e, const Sensors& s) {
    VisualState vs{};

    bool touched = false;
    for (int i = 0; i < 7; ++i) {
        if (s.touch_points[i]) {
            touched = true;
            break;
        }
    }

    // slapen bij weinig prikkels
    if (!touched && !s.is_shaken && e.arousal < 0.15f) {
        vs.id = VisualStateID::Sleeping;
        vs.eye_openness = 0.0f;
        return vs;
    }

    if (e.valence > 0.4f && e.arousal < 0.4f) {
        vs.id = VisualStateID::RelaxedHappy;
    } else if (e.valence > 0.4f && e.arousal >= 0.4f) {
        vs.id = VisualStateID::Excited;
    } else if (e.valence < -0.4f && e.arousal < 0.4f) {
        vs.id = VisualStateID::Sad;
    } else if (e.valence < -0.4f && e.arousal >= 0.4f) {
        vs.id = VisualStateID::Angry;
    } else {
        vs.id = VisualStateID::Neutral;
    }

    float baseOpen = 0.7f;
    float arousalEffect = (e.arousal - 0.5f) * 0.4f;
    vs.eye_openness = constrain(baseOpen + arousalEffect, 0.1f, 1.0f);

    return vs;
}

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

    lastUpdateMs = millis();
    Serial.println("Creature started");
}

void loop() {
    uint32_t now = millis();
    float dt = (now - lastUpdateMs) / 1000.0f;
    if (dt <= 0.0f) dt = 0.01f;
    lastUpdateMs = now;

    Sensors s = sensors_driver.read();

    updateEmotion(s, personality, emotion, dt);
    VisualState vs = decideVisual(emotion, s);

    oled.render(vs);
    lights.render(emotion, vs);

    delay(50);
}
