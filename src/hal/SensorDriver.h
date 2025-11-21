//
// Created by ub on 11/21/25.
//

#ifndef SENSORDRIVER_H
#define SENSORDRIVER_H

#include "domain/Types.h"

class SensorsDriver {
public:
    void begin();
    Sensors read();
};



#endif //SENSORDRIVER_H
