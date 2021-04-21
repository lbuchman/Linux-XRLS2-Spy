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
#include "srxl2Light.h"
#include "Cmd.h"
#include "TaskScheduler.h"
#include <hw.h>
#include <watchdog.h>
#include <teensy3i2cTelemetryMaster.h>

using namespace std;

static SerialTerminal serialTerminal;
static Scheduler ts;
static Srxl2Bus srxl2Bus;
static Srxl2Servo elerons{"elerons", kNoDeviceId, kChannnel2, kCh2PwmPin, Ch2LedPin};
static Srxl2Servo elevator{"elevator", kNoDeviceId, kChannnel3, kCh3PwmPin, Ch3LedPin};
static Srxl2Servo rudder{"rudder", kNoDeviceId, kChannnel4, kCh4PwmPin, Ch4LedPin};
static Srxl2Servo flaps{"flaps", kNoDeviceId, kChannnel6, kCh5PwmPin, Ch6LedPin}; //aux1
static Srxl2Servo gears{"gears", kNoDeviceId, kChannnel5, kCh6PwmPin, Ch5LedPin};
static Srxl2Servo aux2{"aux2Ch", kNoDeviceId, kChannnel7, kAux2PwmPin, Ch7LedPin};
static Srxl2Servo aux3{"aux3Ch", kNoDeviceId, kChannnel8, 0, Ch8LedPin};
static Srxl2Servo aux4{"aux4Ch", kNoDeviceId, kChannnel9, 0, Ch9LedPin};
static Srxl2Servo aux5{"aux5Ch", kNoDeviceId, kChannnel10, 0, Ch10LedPin};

static Srxl2Device telemetry0{"telemetry0", kTelemetry0Id, kNoChannnel};



//static Srxl2Light navigationLights{"navigationLights", kNavigationLightsDeviceId, kChannnel9, false, kNavigationLightsPwmPin};
//static Srxl2Light landingLights{"landingLights", kLandingLightsDeviceId, kChannnel10, false, kLandingLightsDeviceId};

Task watchdogTask(700, TASK_FOREVER, [](void) -> void {
    digitalWrite(WATCH_LED, digitalRead(WATCH_LED) ^ 1);
    watchdog();
}, &ts, true, NULL, NULL);

Task terminalTask(100, TASK_FOREVER, [](void) -> void {
    serialTerminal.cmdPoll();
}, &ts, true, NULL, NULL);

Task srxl2Task(1, TASK_FOREVER, [](void) -> void {
    srxl2Bus.run();
}, &ts, true, NULL, NULL);

/*
 *
 *
 *
 */
int setupFw(int8_t uart) {
    printme(NEWLINE, TIMESTAMP, "SRXL2 Spy Linux Rev 0.1");

    if(uart < 0) {
        // Todo    return  -1;
    }

 
    srxl2Bus.begin(uart);
    srxl2Bus.addDevice(elerons);
    srxl2Bus.addDevice(rudder);
    srxl2Bus.addDevice(elevator);
    srxl2Bus.addDevice(flaps);
    srxl2Bus.addDevice(gears);
    srxl2Bus.addDevice(aux2);
    srxl2Bus.addDevice(aux3);
    srxl2Bus.addDevice(aux4);
    srxl2Bus.addDevice(aux5);
    
    srxl2Bus.addDevice(telemetry0);
   // srxl2Bus.addDevice(navigationLights);
   // srxl2Bus.addDevice(landingLights);

    serialTerminal.begin(&Serial);

    serialTerminal.cmdAdd("list", "list devices", [](int arg_cnt, char **args) -> void {
        srxl2Bus.listDevices();
    });

    serialTerminal.cmdAdd("help", "display help", [](int arg_cnt, char **args) -> void {
        serialTerminal.help();
    });


    serialTerminal.cmdAdd("hang", "watchdo test", [](int arg_cnt, char **args) -> void {
        while(1);
    });

    pinMode(WATCH_LED, OUTPUT);
    enableWatchdog();
    return 0;
}

void mainLoop() {

    while(true) {
        ts.execute();
    }
}
