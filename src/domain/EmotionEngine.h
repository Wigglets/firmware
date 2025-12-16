//
// Created by ub on 11/21/25.
//

#ifndef EMOTIONENGINE_H
#define EMOTIONENGINE_H

#include "domain/Types.h"
#include <Arduino.h>

class EmotionEngine {
public:
    EmotionEngine();

    void setPersonality(const Personality& p);

    void update(const Sensors& sensors, uint32_t dt_ms);

    // Getters voor de drivers
    VisualState getVisualState() const;
    EmotionState getEmotionState() const;

private:
    EmotionState _currentEmotion;
    Personality  _personality;

    void processSensors(const Sensors& sensors);
    void decayEmotions(uint32_t dt_ms);
    void clampValues();
    VisualStateID mapEmotionToVisual() const;
};

#endif //EMOTIONENGINE_H