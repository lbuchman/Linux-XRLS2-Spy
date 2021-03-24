# CMAKE

cmake -DCMAKE_TOOLCHAIN_FILE=../cross/arm-teensy??-gnueabihf.cmake  -DHW=32 -DCMAKE_BUILD_TYPE=Release ../



# Linux-XRLS2-Spy/*

https://github.com/lcgamboa/tty0tty
https://sourceforge.net/projects/tty0tty/files/ I used this
1. Change 1
#include <linux/version.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(4, 11, 0)
#include <linux/sched/signal.h>
#endif
*/
2. Change 2
change TTY0TTY_MAJOR	to 260
 /etc/udev/rules.d/99-tty0tty.rules
SUBSYSTEM=="tty", KERNEL=="tnt0", GROUP="dialout", MODE="0660", SYMLINK+="ttyUSB10"
SUBSYSTEM=="tty", KERNEL=="tnt1", GROUP="dialout", MODE="0660", SYMLINK+="ttyUSB11"
SUBSYSTEM=="tty", KERNEL=="tnt2", GROUP="dialout", MODE="0660"
SUBSYSTEM=="tty", KERNEL=="tnt3", GROUP="dialout", MODE="0660"
SUBSYSTEM=="tty", KERNEL=="tnt4", GROUP="dialout", MODE="0660"
SUBSYSTEM=="tty", KERNEL=="tnt5", GROUP="dialout", MODE="0660"
SUBSYSTEM=="tty", KERNEL=="tnt6", GROUP="dialout", MODE="0660"
SUBSYSTEM=="tty", KERNEL=="tnt7", GROUP="dialout", MODE="0660"

```/home/lbuchman/myGitHub/aa/Linux-XRLS2-Spy/extLib/loggingLib/src/loggingLib
/home/lbuchman/myGitHub/aa/Linux-XRLS2-Spy/extLib/loggingLib/src/loggingLib
/home/lbuchman/myGitHub/aa/Linux-XRLS2-Spy/extLib/SimpleEvents/src/SimpleEvents
/home/lbuchman/myGitHub/aa/Linux-XRLS2-Spy/extLib/SRXL2/src/SRXL2
/home/lbuchman/myGitHub/aa/Linux-XRLS2-Spy/extLib/TaskScheduler/src/TaskScheduler/src
/home/lbuchman/myGitHub/aa/Linux-XRLS2-Spy/inc
/home/lbuchman/myGitHub/aa/Linux-XRLS2-Spy/extLib/CmdArduino/src/CmdArduino
```



# Core Libs

For a given core

got to /home/lbuchman/arduino-1.8.13/hardware/teensy/avr/cores/

teensy4/

teensy3/



For each board build libs based on the Makefile there

Delete main.cpp

put  libteensy40.a files libteensy41.a   libteensy36.a into   /home/lbuchman/arduino-1.8.13/hardware/tools/arm/arm-none-eabi/lib/armv7e-m/fpu/fpv5-d16

put  libteensy32.a  into    /home/lbuchman/arduino-1.8.13/hardware/tools/arm/arm-none-eabi/lib/armv7e-m/

add to CPPFLAGS -s to Makefile                  

$(TARGET).a: $(OBJS) $(MCU_LD)
	$(AR) rcs $@ $^

AR = $(COMPILERPATH)/arm-none-eabi-gcc-ar

