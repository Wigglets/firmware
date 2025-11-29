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
    // -1 = haat lawaai, +1 = houdt van drukte
    float noise_preference;

    // -1 = vindt aanraking irritant, +1 = knuffelig
    float touch_affinity;

    // gewenste lichtniveau (zelfde schaal als light_level)
    float light_preference;

    // 0..1: hoe snel reageert hij negatief op stress
    float irritability;

    // 0..1: hoe sterk arousal reageert op prikkels
    float curiosity;
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
    float eye_openness;
};


#endif //CORE_TYPES_H


#endif //TYPES_H
