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
#else
#include <Arduino.h>
#endif

#include <cstdint>
#include <serial.h>
#include <uart.h>
#include <init.h>
#include <log.h>
#include <srxl2Bus.h>
#include <srxl2Servo.h>
#include <Cmd.h>


//  cmake -DCMAKE_TOOLCHAIN_FILE=../arm-teensy-gnueabihf.cmake  -DHW=32 -DCMAKE_BUILD_TYPE=DEBUG ../

#ifdef ARDUINO
namespace std {
void __throw_bad_alloc() {
    // Serial.println("Unable to allocate memory");

    while(1);
}

void __throw_length_error(char const*e) {
    // Serial.print("Length Error :");
    // Serial.println(e);

    while(1);
}

void __throw_bad_function_call() {
    // Serial.println("Bad function call!");

    while(1);
}

void __throw_out_of_range_fmt(const char*, ...) {
    Serial.println("Bad function call!");

    while(1);
}
}
#endif

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
    int8_t uart = uartInit(deviceFile, baudrate);
    setUart(uart);
#else
    Serial.begin(baudrate);

#endif

    setup();
    loop();
}



