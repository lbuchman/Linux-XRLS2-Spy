/* copyright by Leo Buchman */

#include "Arduino.h"
#include <watchdog.h>

// https://github.com/tonton81/WDT_T4.git

#if defined(ARDUINO_TEENSY41) || defined(ARDUINO_TEENSY40)
#include "Watchdog_t4.h"
WDT_T4<WDT1> wdt;
void wdCallback() {
    Serial.println("Watchdog will expire in 1 sec...");
}

/*
*/
void enableWatchdog() {
    WDT_timings_t config;
    config.trigger = 1; /* in seconds, 0->128 */
    config.timeout = 2; /* in seconds, 0->128 */
    config.callback = wdCallback;
    wdt.begin(config);
}

/*
*/
void watchdog() {
    wdt.feed();
}
#else 
#if defined(ARDUINO_TEENSY32) || defined(ARDUINO_TEENSY36)
/*
*/ 
void watchdog() {
    noInterrupts();
    WDOG_REFRESH = 0xA602;
    WDOG_REFRESH = 0xB480;
    interrupts();
}


/*
*/
void enableWatchdog() {
    WDOG_UNLOCK = WDOG_UNLOCK_SEQ1;
    WDOG_UNLOCK = WDOG_UNLOCK_SEQ2;
    delayMicroseconds(1);    // Need to wait a bit..
    WDOG_STCTRLH = 0x0001; // Enable WDG
 
    // for this, we will use 1 second WDT timeout (e.g. you must reset it in < 1 sec or a boot occurs)

    WDOG_TOVALL = 1000; // The next 2 lines sets the time-out value. This is the value that the watchdog timer compare itself to. 
    WDOG_TOVALH = 0;
    WDOG_PRESC = 0; //     // Thi s sets prescale clock so that the watchdog timer ticks at 7.2MHz
}

/*
*/
void waitWithWatchdog(int wait) {
    uint32_t end = millis() + wait;

    while(millis() < end) {
        watchdog();
    }
}
#else
void enableWatchdog() {}
void watchdog() {}
#endif
#endif
