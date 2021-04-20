#ifndef HW_CONFIG_H
#define HW_CONFIG_H


#define kCh2PwmPin                23
#define kCh3PwmPin                22
#define kCh4PwmPin                 3
#define kCh5PwmPin                10
#define kCh6PwmPin                 5
#define kAux2PwmPin                9

#define Ch2LedPin                  6
#define Ch3LedPin                  4
#define Ch4LedPin                  12
#define Ch5LedPin                  14
#define Ch6LedPin                  15
#define Ch7LedPin                  16
#define Ch8LedPin                  17
#define Ch9LedPin                  20
#define Ch10LedPin                 2

#define kNoDeviceId               254 /* -1 i.e. oxff is used for the broadcast */
#define kTelemetry0Id            0x30 /* flight controller */
#define kElevatorDeviceId         0x61
#define kRudderDeviceId           0x62
#define kFlapsDeviceId            0x63
#define kGearsDeviceId            0x64
//#define kNavigationLightsDeviceId 0x64
//#define kLandingLightsDeviceId    0x65

#define WATCH_LED          13

#endif
