#include <string.h>
#include <iostream>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <uart.h>

#include "loggingLib/log.h"
#include "SRXL2/srxl2Bus.h"
#include "SRXL2/srxl2Servo.h"
#include "CmdArduino/Cmd.h"
#include "TaskScheduler/src/TaskScheduler.h"

using namespace std;

static SerialTerminal serialTerminal;
static Scheduler ts;
static Srxl2Bus srxl2Bus;
static Srxl2Servo elerons{"elerons", 0x60, kChannnel2, true, 14};
static Srxl2Servo rudder{"rudder", 0x61, kChannnel4, true, 15};
static Srxl2Servo elevator{"elevator", 0x62, kChannnel3, false, 16};
static Srxl2Servo flaps{"flaps", 0x63, kChannnel6, false, 17};
static Srxl2Servo lights{"flaps", 0x75, kChannnel5, false, 17};

/*
 * 
 * 
 * 
 */

int main(int argc, char **argv) {
    int baudrate = 115200;
    char deviceFile[32] = "/dev/tnt3";
    char terminalDeviceFile[32] = "/dev/tnt1";
    printme(NEWLINE, TIMESTAMP, "SRXL2 Spy Linux Rev 0.1");

    int c;

    opterr = 0;

    while((c = getopt(argc, argv, "t:p:b:h")) != -1) {
        switch(c) {
            case 'b':
                baudrate = std::stoi(optarg);
                break;

            case 'p':
                strcpy(deviceFile, optarg);
                break;

            case 't':
                strcpy(terminalDeviceFile, optarg);
                break;
                
            case 'h':
                fprintf(stderr, "usage: %s -b baudrate -p portDevice\n", argv[0]);
                break;

            case '?':
                fprintf(stderr, "invalid command line option %c\n\r", c);

            default:
                abort();
        }
    }
#ifdef CmdArduino
    Serial.begin(baudrate);
#else
   Serial.begin(terminalDeviceFile, baudrate);
#endif

    int8_t uart = uartInit(deviceFile, baudrate);

    if(uart < 0) {
        return -1;
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
    while(true) {

        srxl2Bus.run();
        serialTerminal.cmdPoll();
        ts.execute();
    }

    // uartClose(uart);
    // printme(NEWLINE, NO_TIMESTAMP, "uart %s is closed", deviceFile);
    // return 0;

}
