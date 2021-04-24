#ifndef __I2C_MASTER_TELEMETRY_H__
#define __I2C_MASTER_TELEMETRY_H__

#include <cstdint>
#include <vector>

#include <simpleEvents.h>
#include <spektrumTelemetrySensors.h>
#include <hw.h>
#include <spm_srxl.h>


#ifdef ARDUINO
#include <i2c_t3.h>

struct Device {
    uint8_t id;
    int count;
};

typedef void (*Srxl2TelemetryEvent)(SrxlTelemetryData*);

#include "elapsedTime.h"
class MasterI2CTelemetry: public SimpleEvent {
    public:
        MasterI2CTelemetry() {
            self = this;
        };

        /*
        * ****************************************************************************************
        */
        bool begin() {
            Wire.begin(I2C_MASTER, 0x00, i2cPins, I2C_PULLUP_EXT, 100000);
            Wire.setDefaultTimeout(20000);
            // Wire.setOpMode(I2C_OP_MODE_DMA);
            Wire.onTransmitDone(transmitDone);
            Wire.onReqFromDone(requestDone);
            Wire.onError(errorEvent);
            return true;
        }

        /*
        * ****************************************************************************************
        */
        void list() {
            for(std::vector<Device>::iterator it = devices.begin();  it != devices.end(); it++) {
                printme(NEWLINE, NO_TIMESTAMP, "\t\t device 0x%x,  count = %d", it->id, it->count);
            }
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
            if(enumerating) {
                enumerate();
                return;
            }


            if(lastError) {
                String strError =  i2cErrorToStrint(lastError);
                lastError = 0;
                logme(kLogError, LINEINFOFORMAT "I2C Error target = 0x%x, %s", LINEINFO, target, strError.c_str());
            }

            clearTimeout();

            if(requestIsDone) {
                size_t sizeIn = Wire.available();

                if(sizeIn) {
                    Wire.read(buffer, sizeIn);

                    if(buffer[0]) {
                        //     hexdump(buffer, 16, true, kLogError, micros(), 0);
                    }
                }
            }

            if(isBusy()) {
                return;
            }


            target = 0x12;
            requestData(sizeof(SrxlTelemetryData));
        }


        /*
         *
         */
        void scanXbus() {
            enumerating = true;
            target = 0x03;
            Wire.resetBus();
            clearFlags();
            requestData(sizeof(SrxlTelemetryData));
        }

        /*
         *
         */
        String getI2CErrorSTat() {
            String ret;
            ret = "\t\tI2C_ERRCNT_RESET_BUS: = " + String(Wire.getErrorCount(I2C_ERRCNT_RESET_BUS)) + "\n\r";
            ret += "\t\tI2C_ERRCNT_TIMEOUT: = " + String(Wire.getErrorCount(I2C_ERRCNT_TIMEOUT)) + "\n\r";
            ret += "\t\tI2C_ERRCNT_ADDR_NAK: = " + String(Wire.getErrorCount(I2C_ERRCNT_ADDR_NAK)) + "\n\r";
            ret = "\t\tI2C_ERRCNT_DATA_NAK: = " + String(Wire.getErrorCount(I2C_ERRCNT_DATA_NAK)) + "\n\r";
            ret += "\t\tI2C_ERRCNT_ARBL: = " + String(Wire.getErrorCount(I2C_ERRCNT_ARBL)) + "\n\r";
            ret += "\t\tI2C_ERRCNT_NOT_ACQ: = " + String(Wire.getErrorCount(I2C_ERRCNT_NOT_ACQ)) + "\n\r";
            ret += "\t\tI2C_ERRCNT_DMA_ERR: = " + String(Wire.getErrorCount(I2C_ERRCNT_DMA_ERR)) + "\n\r";

            return ret;
        }
        /*
         *
         *
         *
                      device 0x11,  count = 0
                      device 0x12,  count = 0
                      device 0x16,  count = 0
                      device 0x17,  count = 0
                      device 0x34,  count = 0
                      device 0x50,  count = 0
         **/

    private:
        std::vector<Device> devices;
        const int kEnumCount = 3;
        int enumCount = kEnumCount;
        bool enumerating = false;
        const static int kTelementry = 0;
        bool transmitIsDone = true;
        bool requestIsDone = true;
        i2c_pins i2cPins = I2C_PINS_18_19;
        static MasterI2CTelemetry* self;
        uint8_t buffer[256];
        uint8_t target;
        int timer = millis();
        int lastError = 0;
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

        bool incTarget(bool force = false) {
            if(enumCount && !force) {
                enumCount -= 1;
                logme(kLogDebug, LINEINFOFORMAT "Increment target = 0x%x try = %d", LINEINFO, target, enumCount);
                if(!enumCount) {
                    target += 1;
                    enumCount = kEnumCount;
                    logme(kLogDebug, LINEINFOFORMAT "Inc target = 0x%x try = %d", LINEINFO, target, enumCount);
                    if(target > TELE_DEVICE_MAX) {
                        return false;
                    }
                }
            }
            else {
                target += 1;
                enumCount = kEnumCount;
                logme(kLogDebug, LINEINFOFORMAT "Inc target = 0x%x", LINEINFO, target);
                if(target > TELE_DEVICE_MAX) {
                    return false;
                }
            }

            return true;
        }

        /*
         *
         */
        bool clearTimeout() {
            if((millis() - timer) > 20) {
                Wire.resetBus();
                clearFlags();
                return true;
            }

            return false;
        }

        /*
         *
         */
        void clearFlags() {
            transmitIsDone = requestIsDone = true;
        }
        /*
         *
         */
        void requestData(size_t size) {
            timer = millis();
            requestIsDone = false;
            // logme(kLogDebug, LINEINFOFORMAT "Req from target = 0x%x", LINEINFO, target);
            Wire.sendRequest(target, size); // Read from Slave (string len unknown, request full buffer), non-blocking
        }

        //
        /*
         *
         */
        bool isBusy() {
            return !transmitIsDone || !requestIsDone;
        }

        //
        /*
         *
         */
        static void transmitDone(void) {
            self->transmitIsDone = true;
        }

        /*
         *
         */
        static void requestDone(void) {
            self->requestIsDone = true;
        }

        /*
         *
         */
        static void errorEvent(void) {
            self->clearFlags();
            self->lastError = Wire.status();

        }


        /*
         *
         */
        void enumerate() {
            if(lastError) {
                String strError =  i2cErrorToStrint(lastError);
                lastError = 0;
                logme(kLogError, LINEINFOFORMAT "I2C Error target = 0x%x, %s", LINEINFO, target, strError.c_str());

                if(!incTarget()) {
                    enumerating = false;
                    return;
                }

                requestData(sizeof(SrxlTelemetryData));
            }

            if(clearTimeout()) {
                logme(kLogError, LINEINFOFORMAT "I2C Timeout target = 0x%x", LINEINFO, target);

                if(!incTarget()) {
                    enumerating = false;
                    return;
                }

                requestData(sizeof(SrxlTelemetryData));
                return;
            }

            if(requestIsDone) {

                size_t sizeIn = Wire.available();

                if(sizeIn) {
                    Wire.read(buffer, sizeIn);
                    Device device = { target, 0 };
                    bool incForce = true;
                    if(buffer[0]) {
                        devices.push_back(device);
                        logme(kLogInfo, LINEINFOFORMAT "found device = 0x%x", LINEINFO, target);
                    }
                    else {
                       incForce = false;    
                    }

                    if(!incTarget(incForce)) {
                        enumerating = false;
                        return;
                    }

                    requestData(sizeof(SrxlTelemetryData));
                }
            }

        }
};

#else


#endif
#endif
