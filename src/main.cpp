#ifdef ARDUINO
#include <Arduino.h>
#endif

// by taken off #include <iostream> => -100K in size

#ifndef ARDUINO
#include <string.h>
#include <iostream>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <getopt.h>
#include <string.h>
#endif

#include <cstdint>
#include <serial.h>
#include <uart.h>
#include <init.h>
#include <log.h>
#include <srxl2Bus.h>
#include <srxl2Servo.h>
#include <Cmd.h>


using namespace std;

#ifndef ARDUINO
static char deviceFile[32] = "/dev/ttyACM0";
static char terminalDeviceFile[32] = "/dev/ttyUSB11";
#endif
static int baudrate = 115200;


/*
 *
 *
 */

int main(int argc, char **argv) {
    int8_t uart = -1;
#ifndef ARDUINO
    int c;

    opterr = 0;

    while((c = getopt(argc, argv, "t:p:b:h")) != -1) {
        switch(c) {
            case 'b':
                baudrate = stoi(optarg);
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


    Serial.begin(terminalDeviceFile, baudrate);
    uart = uartInit(deviceFile, baudrate);
#else
    Serial.begin(baudrate);

   // while(!Serial);

    Serial.println("I am here");
    uart = uartInit("Serial1", baudrate);
#endif

    setupFw(uart);
    mainLoop();
}



