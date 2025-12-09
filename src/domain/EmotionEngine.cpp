//
// Created by ub on 11/21/25.
//

#include "EmotionEngine.h"
#include <Arduino.h>

struct EmotionPoint {
    VisualStateID id;
    float p;
    float a;
    float d;
};

const EmotionPoint TARGET_EMOTIONS[] = {
    {VisualStateID::Neutral,   0.00f,  0.00f,  0.00f}, // P(-0.1~0.1), A(-0.1~0.1), D(-0.1~0.1)
    {VisualStateID::Relaxed,   0.60f, -0.40f,  0.25f}, // P(0.4~0.8),  A(-0.6~-0.2), D(0.1~0.4)
    {VisualStateID::Happy,     0.80f,  0.35f,  0.50f}, // P(0.6~1.0),  A(0.2~0.5),  D(0.3~0.7)
    {VisualStateID::Excited,   0.70f,  0.80f,  0.40f}, // P(0.5~0.9),  A(0.6~1.0),  D(0.2~0.6)
    {VisualStateID::Sad,      -0.60f, -0.35f, -0.60f}, // P(-0.8~-0.4), A(-0.5~-0.2), D(-0.8~-0.4)
    {VisualStateID::Angry,    -0.60f,  0.80f,  0.80f}, // P(-0.8~-0.4), A(0.6~1.0),  D(0.6~1.0)
    {VisualStateID::Sleeping,  0.00f, -0.90f,  0.00f}  // P(0),        A(-1.0~-0.8), D(0)
};

EmotionEngine::EmotionEngine() {
    _currentEmotion = {0.0f, 0.0f, 0.0f};

    // Default persoonlijkheid (neutraal)
    _personality = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
}

void EmotionEngine::setPersonality(const Personality& p) {
    _personality = p;
}

void EmotionEngine::update(const Sensors& sensors, uint32_t dt_ms) {
    processSensors(sensors);
    decayEmotions(dt_ms);
    clampValues();
}

void EmotionEngine::processSensors(const Sensors& sensors) {
    // Touch
    if (sensors.was_touched) {
        _currentEmotion.arousal += 0.05f;

        float effect = 0.1f * _personality.touch_affinity;

        if (_personality.touch_affinity == 0.0f) effect = 0.05f;

        _currentEmotion.pleasure += effect;

        if (_personality.touch_affinity < -0.5f) {
            _currentEmotion.dominance += 0.05f;
        }
    }

    // Shake
    if (sensors.is_shaken) {
        float base_stress = 0.1f;

        float multiplier = 1.0f + _personality.irritability;
        if (multiplier < 0.2f) multiplier = 0.2f;

        _currentEmotion.arousal   += (base_stress * multiplier);
        _currentEmotion.pleasure  -= (0.05f * multiplier);
        _currentEmotion.dominance += (0.05f * multiplier);
    }

    // Sound
    if (sensors.sound_db > 55.0f) {
        _currentEmotion.arousal += 0.05f;

        if (_personality.noise_preference > 0.2f) {
            _currentEmotion.pleasure += 0.02f * _personality.noise_preference;
        } else {
            _currentEmotion.pleasure += 0.05f * (_personality.noise_preference - 0.2f);
        }
    }

    // Light
    bool is_dark = (sensors.light_level < 50.0f);
    bool likes_dark = (_personality.light_preference < -0.2f);

    if (is_dark && likes_dark) {
        _currentEmotion.pleasure += 0.005f;
        _currentEmotion.arousal -= 0.01f;
    } else if (!is_dark && !likes_dark) {
        _currentEmotion.pleasure += 0.005f;
    } else {
        _currentEmotion.pleasure -= 0.002f;
    }
}

void EmotionEngine::decayEmotions(uint32_t dt_ms) {
    // Basis verval naar 0
    float decay = 0.98f;

    if (_personality.curiosity > 0.5f) {
        decay = 0.995f;
    } else if (_personality.curiosity < -0.5f) {
        decay = 0.90f;
    }

    _currentEmotion.pleasure *= decay;
    _currentEmotion.arousal *= decay;
    _currentEmotion.dominance *= decay;
}

void EmotionEngine::clampValues() {
    auto clamp = [](float& v) { if(v > 1.0f) v=1.0f; if(v < -1.0f) v=-1.0f; };
    clamp(_currentEmotion.pleasure);
    clamp(_currentEmotion.arousal);
    clamp(_currentEmotion.dominance);
}

VisualStateID EmotionEngine::mapEmotionToVisual() const {
    float p = _currentEmotion.pleasure;
    float a = _currentEmotion.arousal;
    float d = _currentEmotion.dominance;

    if (a < -0.5f) return VisualStateID::Sleeping;

    if (p > 0.2f) {
        if (a > 0.2f) return VisualStateID::Excited;
        return VisualStateID::RelaxedHappy;
    }

    if (p < -0.2f) {
        if (d > 0.1f) return VisualStateID::Angry;
        return VisualStateID::Sad;
    }

    return VisualStateID::Neutral;
}

VisualState EmotionEngine::getVisualState() const {
    VisualState vs;
    vs.id = mapEmotionToVisual();
    return vs;
}

EmotionState EmotionEngine::getEmotionState() const {
    return _currentEmotion;
}