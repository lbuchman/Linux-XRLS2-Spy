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

SET(RPI_ROOT $ENV{HOME}/RPI/sysroot)

#SET(CMAKE_SYSROOT $ENV{HOME}/RPI/sysroot) 

if(PI1)
    SET(CROSS /opt/cross-pi-gcc-8.3.0-0/bin/arm-linux-gnueabihf)
    add_compile_options(-march=armv6zk -mcpu=arm1176jzf-s -mfloat-abi=hard -mfpu=vfp )
endif()
 
if(PI2) 
    add_compile_options(-mcpu=cortex-a7 -mfpu=neon-vfpv4 -mfloat-abi=hard)
endif()

if(PI4)
    SET(CROSS /opt/cross-pi-gcc-10.2.0-2/bin/arm-linux-gnueabihf)
    add_compile_options(-march=armv8-a+crc -mcpu=cortex-a72)
endif()

if(PI4u)
    SET(CROSS /opt/cross-pi-gcc-9.3.0-64/bin/aarch64-linux-gnu)
    add_compile_options(-march=armv8-a+fp+simd -mcpu=cortex-a72)
endif()
 
SET(CMAKE_C_COMPILER ${CROSS}-gcc)
SET(CMAKE_CXX_COMPILER ${CROSS}-gcc)
# Define the sysroot path for the RaspberryPi distribution in our tools folder 
SET(CMAKE_FIND_ROOT_PATH ${RPI_ROOT}/)# 
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# Search for libraries and headers in the target directories only
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
