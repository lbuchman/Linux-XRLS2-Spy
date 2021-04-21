#ifndef __I2C_MASTER_TELEMETRY_H__
#define __I2C_MASTER_TELEMETRY_H__

#include <cstdint>

#include <simpleEvents.h>

#ifdef ARDUINO
#include <i2c_t3.h>

#include "elapsedTime.h"
class MasterI2CTelemetry: public SimpleEvent {
    public:
        MasterI2CTelemetry() {};
        
};
    
#else


#endif
#endif
