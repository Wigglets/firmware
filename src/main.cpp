#include <Arduino.h>

#include "hal/SensorDriver.h"

SensorsDriver sensors_driver;

void setup() {
  Serial.begin(115200);
  sensors_driver.begin();

    Serial.println("Hello!");
}

void loop() {
 Sensors result =  sensors_driver.read();

    for (int i = 0; i < 7; i++) {
        if (result.touch_points[i]) {
            Serial.print("Touch detected at point ");
            Serial.println(i);
        }
    }
  delay(50);
}