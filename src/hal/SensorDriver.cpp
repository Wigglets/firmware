//
// Created by ub on 11/21/25.
//

#include "SensorDriver.h"

#include <Wire.h>

#include "Adafruit_CAP1188.h"
#include "Adafruit_VEML7700.h"
#include "config.h"
#include "SparkFunLSM6DS3.h"
#include "HalUtils.h"

const int N_SAMPLES = 400;
const float REF_RMS = 10.0f;

Adafruit_CAP1188 cap = Adafruit_CAP1188();
LSM6DS3 imu(I2C_MODE, 0x6B);
Adafruit_VEML7700 veml = Adafruit_VEML7700();


const int sampleWindow = 50;
unsigned int sample;

const int thresholdSilence = 20;  // Below this is Silence
const int thresholdLoud = 400;


void SensorsDriver::begin() const {
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

/**
 *
 * @param s
 */
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
    unsigned long startMillis = millis();
    unsigned int peakToPeak = 0;

    unsigned int signalMax = 0;
    unsigned int signalMin = 1024;

    // 1. Gather data for 50ms
    while (millis() - startMillis < sampleWindow) {
        sample = analogRead(A0);

        if (sample < 1024) {
            if (sample > signalMax) {
                signalMax = sample;
            }
            if (sample < signalMin) {
                signalMin = sample;
            }
        }
    }

    // --- SAFETY CHECK (The Fix) ---
    // Only calculate if we actually found a valid Max and Min
    if (signalMax > signalMin) {
        peakToPeak = signalMax - signalMin;
    } else {
        peakToPeak = 0; // If math fails, assume silence
    }

    // 3. Categorize the Sound
    Serial.print("Raw Value: ");
    Serial.print(peakToPeak);
    Serial.print(" | Status: ");

    if (peakToPeak <= thresholdSilence) {
        Serial.println("SILENCE");
    }
    else if (peakToPeak >= thresholdLoud) {
        Serial.println("LOUD!!");
    }
    else {
        Serial.println("Normal");
    }
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





