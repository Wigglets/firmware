//
// Created by ub on 11/21/25.
//

#include "SensorDriver.h"

#include <Wire.h>

#include "Adafruit_CAP1188.h"
#include "SparkFunLSM6DS3.h"

Adafruit_CAP1188 cap = Adafruit_CAP1188();
LSM6DS3 imu(I2C_MODE, 0x6B);

void SwitchBus(uint8_t bus){
    Wire.beginTransmission(0x70);
    Wire.write(1 << bus);
    Wire.endTransmission();
}


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

}

Sensors SensorsDriver::read() {

    Sensors s{};
    SwitchBus(4);

    uint8_t touched = cap.touched();

    for (uint8_t i=0; i<7; i++) {
        if (touched & (1 << i)) {
            s.touch_points[i] = true;
        }
    }

    SwitchBus(5);  // altijd kanaal selecteren vóór lezen

    float gx = imu.readFloatGyroX();
    float gy = imu.readFloatGyroY();
    float gz = imu.readFloatGyroZ();
    
    Serial.print(gx); Serial.print('\t');
    Serial.print(gy); Serial.print('\t');
    Serial.println(gz);


    return s;
}



