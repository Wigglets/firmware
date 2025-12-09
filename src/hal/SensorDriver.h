//
// Created by ub on 11/21/25.
//

#ifndef SENSORDRIVER_H
#define SENSORDRIVER_H

#include "domain/Types.h"

class SensorsDriver {
public:
    void begin() const;
    Sensors read();

private:
    void readLight(Sensors &s);
    void readSound(Sensors &s);
    void readTemperature(Sensors &s);
    void readTouch(Sensors &s);
    void readAcceleromete(Sensors &s);
};



#endif //SENSORDRIVER_H
