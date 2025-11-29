//
// Created by ub on 11/21/25.
//

#include "SensorDriver.h"

#include <Wire.h>

#include "Adafruit_CAP1188.h"
#include "Adafruit_VEML7700.h"
#include "SparkFunLSM6DS3.h"
#include "HalUtils.h"

#define CAPACITIVE_BUS 4
#define IMU_BUS 5
#define TEMPERATURE_BUS 5
#define LIGHT_BUS 6

const int MIC_PIN = 0;          // GPIO0 (ADC1_CH0)
const int N_SAMPLES = 400;      // more samples = smoother, slower
const float REF_RMS = 10.0f;    // reference RMS (tune this after seeing values)

Adafruit_CAP1188 cap = Adafruit_CAP1188();
LSM6DS3 imu(I2C_MODE, 0x6B);
Adafruit_VEML7700 veml = Adafruit_VEML7700();


const int sampleWindow = 50; // Meetperiode in ms
unsigned int sample;


void SensorsDriver::begin() {
    Wire.begin();

    SwitchBus(4);
    if (!cap.begin(0x29)) {
        Serial.println("CAP1188 not found");
    }

    SwitchBus(5);
    if (!imu.begin()) {
        Serial.println("IMU not found");
    }

    SwitchBus(6);
    if (!veml.begin()) {
        Serial.println("VEML7700 not found");
    }

    analogReadResolution(12); // Zorg voor 0-4095 bereik
    analogSetAttenuation(ADC_11db);
}

Sensors SensorsDriver::read() {
    Sensors s{};

    readTouch(s);
    readTemperature(s);
    readAcceleromete(s);
    readLight(s);
    readSound(s);
    return s;
}

void SensorsDriver::readTouch(Sensors &s) {
    SwitchBus(CAPACITIVE_BUS);
    uint8_t touched = cap.touched();
    s.was_touched = (touched > 0);
    for (uint8_t i = 0; i < 7; i++) {
        s.touch_points[i] = (touched & (1 << i));
    }
}


void SensorsDriver::readLight(Sensors &s) {
    SwitchBus(LIGHT_BUS);
    s.light_level = veml.readLux(VEML_LUX_NORMAL_NOWAIT);
}

void SensorsDriver::readSound(Sensors &s) {
    long sum = 0;
    long sumSq = 0;
    for (int i = 0; i < N_SAMPLES; i++) {
        int v = analogRead(MIC_PIN);
        sum += v;
        sumSq += (long)v * (long)v;
        delayMicroseconds(200);
    }

    float mean = (float)sum / N_SAMPLES;
    float meanSq = (float)sumSq / N_SAMPLES;
    float variance = meanSq - mean * mean;
    if (variance < 0) variance = 0;

    float rms = sqrtf(variance);

    float dB;
    if (rms <= 0.0f) {
        dB = -80.0f;
    } else {
        dB = 20.0f * log10f(rms / REF_RMS);
    }

    s.sound_db = dB;
}

void SensorsDriver::readTemperature(Sensors &s) {
    SwitchBus(TEMPERATURE_BUS);
    s.temperature = imu.readTempC();
}

void SensorsDriver::readAcceleromete(Sensors &s) {
    SwitchBus(IMU_BUS);
    float ax = imu.readFloatAccelX();
    float ay = imu.readFloatAccelY();
    float az = imu.readFloatAccelZ();

    float total_force = sqrt(ax*ax + ay*ay + az*az);

    if (total_force > 2.5) {
        s.is_shaken = true;
    } else {
        s.is_shaken = false;
    }
}





