//
// Created by ub on 11/21/25.
//

#ifndef TYPES_H
#define TYPES_H

#ifndef CORE_TYPES_H
#define CORE_TYPES_H

// core_types.h
#pragma once
#include <stdint.h>

struct Sensors {
    float temperature;
    float sound_db;
    bool  was_touched;
    bool touch_points[7];
    float light_level;
    bool is_shaken;
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
    float light_preference;
    float irritability;
    float curiosity;
};

struct EmotionState {
    float pleasure;
    float arousal;
    float dominance;
};


// TODO eventually remove this and replace with algorithm that generates visual representation based on personality and emotionstate. Instead of fixed emotions.
enum class VisualStateID {
    Neutral,
    Relaxed,
    Happy,
    Excited,
    Sad,
    Angry,
    Sleeping
};

struct VisualState {
    VisualStateID id;
};


#endif //CORE_TYPES_H


#endif //TYPES_H
