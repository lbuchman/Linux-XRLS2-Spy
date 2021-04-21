#ifndef __I2C_MASTER_TELEMETRY_H__
#define __I2C_MASTER_TELEMETRY_H__

#include <cstdint>

#include <simpleEvents.h>
#include <spektrumTelemetrySensors.h>
#include <hw.h>

#ifdef ARDUINO
#include <i2c_t3.h>

typedef void (*Srxl2TelemetryEvent)(SrxlTelemetryData*);

#include "elapsedTime.h"
class MasterI2CTelemetry: public SimpleEvent {
    public:
        MasterI2CTelemetry() {

        };

        /*
        * ****************************************************************************************
        */
        bool begin() {
            Wire.begin(I2C_MASTER, 0x00, i2cPins, I2C_PULLUP_EXT, 100000);
            Wire.setDefaultTimeout(20000);
            Wire.setOpMode(I2C_OP_MODE_DMA);

            return true;
        }

        /*
        * ****************************************************************************************
        */
        void onTelementryDataIn(Srxl2TelemetryEvent eventFunction) {
            logme(kLogInfo, LINEINFOFORMAT "Register Event kTelementry = %d", LINEINFO, kTelementry);
            registerEvent(kTelementry, (event) eventFunction);
        }

        /*
        * ****************************************************************************************
        */
        void run() {
            Wire.sendRequest(0x12, 16, I2C_STOP);  // NON-blocking read (request 256 bytes)
           Wire.finish();
            int count = 0;
            uint8_t buffer[16];

            while(Wire.available()) {
                buffer[count] =  Wire.readByte();
                count += 1;
            }

            hexdump(buffer, 16, true, kLogError, micros(), 0);
        }


    private:
        const static int kTelementry = 0;
        i2c_pins i2cPins = I2C_PINS_18_19;

        String i2cErrorToStrint(int error) {
            switch(error) {
                case I2C_TIMEOUT:
                    return "I2C timeout";

                case I2C_ADDR_NAK:
                    return "Slave addr not acknowledged";

                case I2C_DATA_NAK:
                    return "Slave data not acknowledged";

                case I2C_ARB_LOST:
                    return "Arbitration Lost, possible pullup problem";

                case I2C_BUF_OVF:
                    return "I2C buffer overflow";

                case I2C_NOT_ACQ:
                    return "Cannot acquire bus, possible stuck SDA/SCL";

                case I2C_DMA_ERR:
                    return "DMA Error";

                default:
                    return "not known error";
                    break;
            }
        }
};

#else


#endif
#endif
