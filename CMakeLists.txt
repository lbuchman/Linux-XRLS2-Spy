#Copyright (c) Microsoft. All rights reserved.
#Lic`sed under the MIT license. See LICENSE file in the project root for full license information.


#cmake -DRPI2=1 -DCMAKE_TOOLCHAIN_FILE=../arm-linux-gnueabihf.cmake ..

cmake_minimum_required(VERSION 3.20)
set(PROJECT_NAME SRXL2_SPY)

include(ExternalProject)


project (sxrl2Spy)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s -Os -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s -std=gnu++17 -Os -Wall")

set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG}  -O0 -Wall -g")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}  -std=gnu++17 -O0 -g -Wall")


IF(${CMAKE_CROSSCOMPILING} MATCHES "1")
message(STATUS -----------------------------Cross Compiling --------------------)
file(GLOB_RECURSE target_source_cpp_files ${ARDUINO_INST}/hardware/teensy/avr/cores/${TEENSY_TYPE}/*.cpp)
file(GLOB_RECURSE target_source_c_files ${ARDUINO_INST}/hardware/teensy/avr/cores/${TEENSY_TYPE}/*.c)
file(GLOB_RECURSE target_source_h_files ${ARDUINO_INST}/hardware/teensy/avr/cores/${TEENSY_TYPE}/*.h)


# add_custom_command does not create a new target. You have to define targets explicitly
# by add_executable, add_library or add_custom_target in order to make them visible to make

#https://dornerworks.com/blog/moving-embedded-projects-to-cmake/

ENDIF(${CMAKE_CROSSCOMPILING} MATCHES "1")

message(STATUS "Running cmake for Tennsy ${TEENSY_VARIANT}")

# Application files
file(GLOB_RECURSE app_source_cpp_files ./src/*.cpp)
file(GLOB_RECURSE app_source_h_files ./inc/*.h)
include_directories(./ ./inc ./extLib)

# platform ctest_read_custom_files
file(GLOB_RECURSE app_platform_cpp_files ./linux/*.cpp ./teensy/*.cpp)

file(GLOB_RECURSE lib_cpp_files ./extLib/*.cpp)
file(GLOB_RECURSE lib_source_h_files ./extLib/*.h)


add_executable(${PROJECT_NAME} ${app_source_h_files} ${app_source_cpp_files}  ${app_source_h_files} ${app_platform_cpp_files} ${lib_cpp_files} ${lib_source_h_files} ${target_source_cpp_files} ${target_source_c_files} ${target_source_h_files})

IF(NOT ${CMAKE_CROSSCOMPILING} MATCHES "1")
target_link_libraries(${PROJECT_NAME}
     m
     pthread
)
ELSE()
target_link_libraries(${PROJECT_NAME}
     m
)
ENDIF(NOT ${CMAKE_CROSSCOMPILING} MATCHES "1")

install(CODE "MESSAGE(\"installing target\")") 
 
 
add_custom_target(format
    COMMAND
        ../format.sh
    VERBATIM
)

add_custom_target(size
    COMMAND
        ${SIZE} ${PROJECT_NAME}
    VERBATIM
)

add_custom_command(TARGET ${PROJECT_NAME}
    COMMAND
        ${OBJCOPY} -O ihex -R .eeprom ${PROJECT_NAME} ${PROJECT_NAME}.hex
    VERBATIM
)

add_custom_target(flash
    COMMAND
        teensy_loader_cli -s -mmcu=${MCU} -w -v  ${PROJECT_NAME}.hex
    VERBATIM
)

ExternalProject_Add(
    CmdArduino
    GIT_REPOSITORY https://github.com/lbuchman/CmdArduino.git
    GIT_TAG linuxSrxl2
    PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/extLib/CmdArduino
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
) 

ExternalProject_Add(
    loggingLib
    GIT_REPOSITORY https://github.com/lbuchman/loggingLib.git
    GIT_TAG platformIndependent
    PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/extLib/loggingLib
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
) 

ExternalProject_Add(
    SimpleEvents
    GIT_REPOSITORY https://github.com/lbuchman/SimpleEvents.git
    GIT_TAG master
    PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/extLib/SimpleEvents
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
) 

 ExternalProject_Add(
    TaskScheduler
    GIT_REPOSITORY https://github.com/arkhipenko/TaskScheduler.git
    GIT_TAG master
    PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/extLib/TaskScheduler
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
) 

ExternalProject_Add(
    SRXL2
    GIT_REPOSITORY https://github.com/lbuchman/SRXL2.git
    GIT_TAG main
    PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/extLib/SRXL2
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
) 
ExternalProject_Get_Property(SRXL2 source_dir)
message(STATUS ${source_dir})
ExternalProject_Get_Property(TaskScheduler source_dir)
message(STATUS ${source_dir})
ExternalProject_Get_Property(SimpleEvents source_dir)
ExternalProject_Get_Property(loggingLib source_dir)
ExternalProject_Get_Property(CmdArduino source_dir)
