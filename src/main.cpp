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

using namespace std;

int main(int argc, char **argv) {
    int baudrate = 115200;
    char deviceFile[32] = "/dev/ttyACM0";
    printme(NEWLINE, TIMESTAMP, "SRXL2 Spy Linux Rev 0.1");

    int c;

    opterr = 0;

    while((c = getopt(argc, argv, "p:b:h")) != -1) {
        switch(c) {
            case 'b':
                baudrate = std::stoi(optarg);
                break;

            case 'p':
                strcpy(deviceFile, optarg);
                break;

            case 'h':
                fprintf(stderr, "usage: %s -b baudrate -p portDevice\n", argv[0]);
                break;

            case '?':
                fprintf(stderr, "invalid command line option %c\n", c);

            default:
                abort();
        }
    }

    fprintf(stderr, "starting serial device %s at baudrate %d\n", deviceFile, baudrate);



    int8_t uart = uartInit(deviceFile, baudrate);

    if(uart < 0) {
        return -1;
    }

    Srxl2Bus srxl2Bus(uart);
    Srxl2Servo elerons("elerons", 0x60, 2, true, 14);
    Srxl2Servo rudder("rudder", 0x61, 3, true, 15);
    Srxl2Servo elevator("elevator", 0x62, 3, true, 16);
    Srxl2Servo flaps("flaps", 0x63, 3, false, 17);
    srxl2Bus.addDevice(elerons);
    srxl2Bus.addDevice(rudder);
    srxl2Bus.addDevice(elevator);
    srxl2Bus.addDevice(flaps);

    while(true) {

        srxl2Bus.run();
    }

    uartClose(uart);
    printme(NEWLINE, NO_TIMESTAMP, "uart %s is closed", deviceFile);
    return 0;

}
