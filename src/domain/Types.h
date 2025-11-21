//
// Created by ub on 11/21/25.
//

#ifndef TYPES_H
#define TYPES_H

//
// Created by ub on 11/21/25.
//

#ifndef CORE_TYPES_H
#define CORE_TYPES_H

// core_types.h
#pragma once
#include <stdint.h>

struct Sensors {
    float temperature;
    float sound_db;
    bool  was_touched;
    [7]bool touch_points;
    float light_level;
};

struct NeighborInfo {
    uint32_t id;
    float avg_rssi;
    uint32_t last_seen_ms;
};

struct SocialContext {
    int total_neighbors;
    int close_neighbors;
    bool just_became_not_alone;
};

/*
 * Personality
 *  All values range from -1 to 1.
 */
struct Personality {
    float noise_preference;
    float touch_affinity;
    float temp_preference;
    float irritability;
    float curiosity;
    float sociality;
};

struct EmotionState {
    float valence;   // -1..1
    float arousal;   // 0..1
};


// TODO eventually remove this and replace with algorithm that generates visual representation based on personality and emotionstate. Instead of fixed emotions.
enum class VisualStateID {
    Neutral,
    RelaxedHappy,
    Excited,
    Sad,
    Angry,
    Sleeping
};

struct VisualState {
    VisualStateID id;
    // evt. parameters: eye_openness, mouth_curve, blink_speed, led_intensity, etc.
};


#endif //CORE_TYPES_H


#endif //TYPES_H
