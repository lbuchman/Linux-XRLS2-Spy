# this one is important

#install cross compile arm for every arch
# make sure prebuild all additional libs and place .h files in sysroot which is  like  /opt/cross-pi-gcc-9.3.0-64/aarch64-linux-gnu/libc/usr/include


#do not define CMAKE_SYSROOT, what will ahppen if include is taken fro sysroot it will brake 64 bit gcc because it will take gcc .h from the board not from the local gcc dir
#sudo apt-get install gdb-multiarch
#gdb-multiarch  #make sure to do stepi in gdp before doing c or the program will not run
#gdbserver --multi mysticlakelinux:2001

#cmake -DRPI2=1 -DCMAKE_TOOLCHAIN_FILE=../arm-linux-gnueabihf.cmake ..

CMAKE_MINIMUM_REQUIRED(VERSION 3.20) 

SET(CMAKE_SYSTEM_NAME Linux)
SET(CMAKE_SYSTEM_VERSION 1)# Define the cross compiler locations


SET(CMAKE_SYSTEM_NAME Generic)
SET(CMAKE_SYSTEM_PROCESSOR arm)
SET(CMAKE_CROSSCOMPILING 1)
get_filename_component(ARDUINO_INST "~/arduino-ide" REALPATH BASE_DIR)

SET(TOOLSPATH ${ARDUINO_INST}/hardware/tools)
SET(COMPILERPATH  ${TOOLSPATH}/arm/bin)
SET(CROSS ${COMPILERPATH}/arm-none-eabi)
SET(CMAKE_SYSROOT, ${TOOLSPATH}/arm)
SET(TEENSY_ROOT ${TOOLSPATH}/arm)

SET(TEENSY_VARIANT teensy4.1)
SET(TEENSY_CORE_SPEED 60000000)
SET(TEENSY_TYPE teensy4)
SET(MCU TEENSY41)
SET(CPU_FLAGS -ffunction-sections -fdata-sections -DARDUINO_TEENSY41 -D__IMXRT1062__ -mcpu=cortex-m7 -mthumb -mfloat-abi=hard -mfpu=fpv5-d16 -DF_CPU=${TEENSY_CORE_SPEED})
SET (CORE_PATH ${ARDUINO_INST}/hardware/teensy/avr/cores/${TEENSY_TYPE})
SET(c_flags -DUSB_SERIAL -DLAYOUT_US_ENGLISH -DTEENSYDUINO=150 ${CPU_FLAGS} -I${CORE_PATH}  -DARDUINO -Iinc -nostdlib -fno-exceptions   -Wno-error=narrowing -fstack-usage -DHW=${HW})
SET(cxx_flags ${c_flags} -felide-constructors -fno-rtti -fpermissive  -fno-threadsafe-statics)


add_compile_options(
    "$<$<COMPILE_LANGUAGE:C>:${c_flags}>"
    "$<$<COMPILE_LANGUAGE:CXX>:${cxx_flags}>"
)

add_link_options(-Wl,--gc-sections,--relax -T${CORE_PATH}/imxrt1062_t41.ld ${CPU_FLAGS})

SET(OBJCOPY ${CROSS}-objcopy) 
set(SIZE  ${CROSS}-size) 
SET(CMAKE_C_COMPILER ${CROSS}-gcc)
SET(CMAKE_CXX_COMPILER ${CROSS}-gcc)
# Define the sysroot path for the RaspberryPi distribution in our tools folder 
SET(CMAKE_FIND_ROOT_PATH ${TEENSY_ROOT}/)# 
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# Search for libraries and headers in the target directories only
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
