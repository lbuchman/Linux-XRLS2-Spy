# this one is important

#install cross compile arm for every arch
# make sure prebuild all additional libs and place .h files in sysroot which is  like  /opt/cross-pi-gcc-9.3.0-64/aarch64-linux-gnu/libc/usr/include


#do not define CMAKE_SYSROOT, what will ahppen if include is taken fro sysroot it will brake 64 bit gcc because it will take gcc .h from the board not from the local gcc dir
#sudo apt-get install gdb-multiarch
#gdb-multiarch  #make sure to do stepi in gdp before doing c or the program will not run
#gdbserver --multi mysticlakelinux:2001

#cmake -DRPI2=1 -DCMAKE_TOOLCHAIN_FILE=../arm-linux-gnueabihf.cmake ..

SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_VERSION 1)# Define the cross compiler locations

CMAKE_MINIMUM_REQUIRED(VERSION 3.20) 


SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_PROCESSOR arm)
SET(CMAKE_CROSSCOMPILING 1)

get_filename_component(ARDUINO_INST "~/arduino-ide" REALPATH BASE_DIR)


SET(TOOLSPATH ${ARDUINO_INST}/hardware/tools)
SET(COMPILERPATH  ${TOOLSPATH}/arm/bin)
SET(CROSS ${COMPILERPATH}/arm-none-eabi)
SET(CMAKE_SYSROOT, ${TOOLSPATH}/arm)
SET(TEENSY_ROOT ${TOOLSPATH}/arm)
SET(TEENSY_VARIANT teensy3.6)
SET(MCU TEENSY36)
SET(TEENSY_CORE_SPEED 180000000)
SET(TEENSY_TYPE teensy3)
SET(CPU_FLAGS  -fsingle-precision-constant -mfloat-abi=hard -mfpu=fpv4-sp-d16 -D__MK66FX1M0__  -DHW=${HW} -mcpu=cortex-m4 -mthumb -DF_CPU=${TEENSY_CORE_SPEED} -I${CORE_PATH} -DUSB_SERIAL -DLAYOUT_US_ENGLISH -DTEENSYDUINO=150 -ffunction-sections -fdata-sections) 

SET (CORE_PATH ${ARDUINO_INST}/hardware/teensy/avr/cores/${TEENSY_TYPE})
SET(c_flags ${CPU_FLAGS}  -DHW=$(HW) -fno-exceptions -fstack-usage)
SET(cxx_flags ${c_flags} -felide-constructors -fno-rtti)


add_compile_options(
    "$<$<COMPILE_LANGUAGE:C>:${c_flags}>"
    "$<$<COMPILE_LANGUAGE:CXX>:${cxx_flags}>"
)

add_link_options(-Wl,--gc-sections,--relax -T${CORE_PATH}/mk66fx1m0.ld ${CPU_FLAGS} -larm_cortexM4l_math)

SET(OBJCOPY ${CROSS}-objcopy) 
set(SIZE ${CROSS}-size) 
SET(CMAKE_C_COMPILER ${CROSS}-gcc)
SET(CMAKE_CXX_COMPILER ${CROSS}-gcc)
# Define the sysroot path for the RaspberryPi distribution in our tools folder 
SET(CMAKE_FIND_ROOT_PATH ${TEENSY_ROOT}/)# 
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# Search for libraries and headers in the target directories only
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
