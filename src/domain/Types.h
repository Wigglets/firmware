//
// Created by ub on 11/21/25.
//

#ifndef TYPES_H
#define TYPES_H

#ifndef CORE_TYPES_H
#define CORE_TYPES_H

#include <Arduino.h>


// MAX9814 is not able to measure db accurate. So use sound levels
enum class NoiseLevel {
    Silent,
    Normal,
    Loud
};

struct Orientation {
    float accelX;
    float accelY;
    float accelZ;
    float gyroX;
    float gyroY;
    float gyroZ;
};

struct Sensors {
    float temperature;
    bool  was_touched;
    bool touch_points[7];
    float light_level;
    float noise_level;
    Orientation orientation;
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
