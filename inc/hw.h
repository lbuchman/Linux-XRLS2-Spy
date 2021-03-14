#ifndef HW_CONFIG_H
#define HW_CONFIG_H


#define kEleronsPwmPin            23
#define kElevatorPwmPin           22
#define kRudderPwmPin              2
#define kFlapsPwmPin               3
#define kNavigationLightsPwmPin    4
#define kLandingLightsPwmPin       5

#define kEleronsDeviceId          0x60
#define kElevatorDeviceId         0x61
#define kRudderDeviceId           0x62
#define kFlapsDeviceId            0x63
#define kNavigationLightsDeviceId 0x64
#define kLandingLightsDeviceId    0x65 

#define WATCH_LED          13
#define NEO_Status_Led     11
#define sdCardLogLed       23
#define Xbus_Status_led    12
#define Srxl2_Status_led    5
#define Pitot_Status_led   22
#define Bmp280_led         15




#define MOTOR_TEMP_AD_PIN A7
#define I2C_CLOCK_CONTROL 20
#define EXT_NEO_INT_PIN 6
#define Neo_Power_Control 14

#define LANDING_LIGHTS 9
#define NAVIGATION_LIGHTS 10

#if defined(__MK20DX256__)
#define i2cPort  Wire1
#define i2cPins  I2C_PINS_29_30
#endif
#if defined(__MK66FX1M0__)
#define i2cPort Wire2
#define i2cPins I2C_PINS_3_4
#endif

#endif
