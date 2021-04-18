#ifndef HW_CONFIG_H
#define HW_CONFIG_H


#define kEleronsPwmPin            23
#define kElevatorPwmPin           22
#define kRudderPwmPin              3
#define kFlapsPwmPin               10
#define kNavigationLightsPwmPin    10 /* not connected */
#define kLandingLightsPwmPin       11 /* not connected */
#define kGearsPwmPin               5
#define kAux2PwmPin                9

#define Ch2LedPin                  6
#define Ch3LedPin                  4
#define Ch4LedPin                  12
#define Ch5LedPin                  1
#define Ch6LedPin                  14
#define Ch7LedPin                  16
#define Ch8LedPin                  17
#define Ch9LedPin                  20
#define Ch10LedPin                 2

#define kNoDeviceId               254 /* -1 i.e. oxff is used for the broadcast */
#define kEleronsDeviceId          0x60
#define kElevatorDeviceId         0x61
#define kRudderDeviceId           0x62
#define kFlapsDeviceId            0x63
#define kGearsDeviceId            0x64
//#define kNavigationLightsDeviceId 0x64
//#define kLandingLightsDeviceId    0x65

#define WATCH_LED          13

#endif
