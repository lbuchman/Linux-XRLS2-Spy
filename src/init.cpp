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

using namespace std;

static SerialTerminal serialTerminal;
static Scheduler ts;
static Srxl2Bus srxl2Bus;
static Srxl2Servo elerons{"elerons", kEleronsDeviceId, kChannnel2, false, kEleronsPwmPin};
static Srxl2Servo rudder{"rudder", kRudderDeviceId, kChannnel4, false, kRudderPwmPin};
static Srxl2Servo elevator{"elevator", kElevatorDeviceId, kChannnel3, false, kElevatorPwmPin};
static Srxl2Servo flaps{"flaps", kFlapsDeviceId, kChannnel6, false, kFlapsPwmPin};
static Srxl2Light navigationLights{"navigationLights", kNavigationLightsDeviceId, kChannnel9, false, kNavigationLightsPwmPin};
static Srxl2Light landingLights{"landingLights", kLandingLightsDeviceId, kChannnel10, false, kLandingLightsDeviceId};

Task watchdogTask(700, TASK_FOREVER, [](void) -> void {
  //  digitalWrite(WATCH_LED, digitalRead(WATCH_LED) ^ 1);
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
    srxl2Bus.addDevice(navigationLights);
    srxl2Bus.addDevice(landingLights);

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
