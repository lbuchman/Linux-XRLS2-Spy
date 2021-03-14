#ifndef ARDUINO
#include <string.h>
#include <iostream>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#endif

#include <cstdint>

#include <uart.h>
#include "log.h"
#include <srxl2Bus.h>
#include "srxl2Servo.h"
#include "Cmd.h"
#include "TaskScheduler.h"

// cmake -DCMAKE_TOOLCHAIN_FILE=../arm-teensy-gnueabihf.cmake  -DHW=32 -DCMAKE_BUILD_TYPE=DEBUG ../


using namespace std;

static SerialTerminal serialTerminal;
static Scheduler ts;
static Srxl2Bus srxl2Bus;
static Srxl2Servo elerons{"elerons", 0x60, kChannnel2, true, 14};
static Srxl2Servo rudder{"rudder", 0x61, kChannnel4, true, 15};
static Srxl2Servo elevator{"elevator", 0x62, kChannnel3, false, 16};
static Srxl2Servo flaps{"flaps", 0x63, kChannnel6, false, 17};
static Srxl2Servo lights{"flaps", 0x75, kChannnel5, false, 17};
static int8_t uart = -1;;

#include <cstdint>


/*
 *
 *
 *
 */
void setUart(int8_t _uart) {
    uart = _uart;
}

/*
 *
 *
 *
 */
int setupFw(int8_t _uart) {
    uart = _uart;
    printme(NEWLINE, TIMESTAMP, "SRXL2 Spy Linux Rev 0.1");

    if(uart < 0) {
    // Todo    return  -1;
    }

     srxl2Bus.begin(uart);
     srxl2Bus.addDevice(elerons);
     srxl2Bus.addDevice(rudder);
     srxl2Bus.addDevice(elevator);
     srxl2Bus.addDevice(flaps);
     srxl2Bus.addDevice(lights);

    serialTerminal.begin(&Serial);

    serialTerminal.cmdAdd("list", "list devices", [](int arg_cnt, char **args) -> void {
        srxl2Bus.listDevices();
    });

    serialTerminal.cmdAdd("help", "display help", [](int arg_cnt, char **args) -> void {
        serialTerminal.help();
    });

    return 0;
}

void mainLoop() {

    while(true) {
        srxl2Bus.run();
        serialTerminal.cmdPoll();
        ts.execute();
    }
}
