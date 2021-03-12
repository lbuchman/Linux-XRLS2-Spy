#Copyright (c) Microsoft. All rights reserved.
#Lic`sed under the MIT license. See LICENSE file in the project root for full license information.


#cmake -DRPI2=1 -DCMAKE_TOOLCHAIN_FILE=../arm-linux-gnueabihf.cmake ..

cmake_minimum_required(VERSION 3.20)
set(PROJECT_NAME SRXL2_SPY)

include(ExternalProject)


project (sxrl2Spy)

 

set(CMAKE_VERBOSE_MAKEFILE ON)

SET(MY_DEBUG_OPTIONS -O0 -Wall)
SET(MY_RELEASE_OPTIONS  -s -Os -Wall)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/inc)


IF(${CMAKE_CROSSCOMPILING} MATCHES "1")
message(STATUS -----------------------------Cross Compiling --------------------)
#file(GLOB_RECURSE target_source_cpp_files ${CORE_PATH}/*.cpp)
#file(GLOB_RECURSE target_source_c_files ${CORE_PATH}/hardware/teensy/avr/cores/${TEENSY_TYPE}/*.c)
#file(GLOB_RECURSE target_source_h_files ${CORE_PATH}/hardware/teensy/avr/cores/${TEENSY_TYPE}/*.h)
message(STATUS "Running cmake for Tensy ${MCU_DEF}")
ELSE()
SET(OBJCOPY objcopy) 
ENDIF(${CMAKE_CROSSCOMPILING} MATCHES "1")

# Application files


ExternalProject_Add(
    CmdArduino
    GIT_REPOSITORY https://github.com/lbuchman/CmdArduino.git
    GIT_TAG linuxSrxl2
    PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/extLib/CmdArduino
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    UPDATE_COMMAND ""
) 

ExternalProject_Add(
    loggingLib
    GIT_REPOSITORY https://github.com/lbuchman/loggingLib.git
    GIT_TAG platformIndependent
    PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/extLib/loggingLib
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    UPDATE_COMMAND ""
) 

ExternalProject_Add(
    SimpleEvents
    GIT_REPOSITORY https://github.com/lbuchman/SimpleEvents.git
    GIT_TAG master
    PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/extLib/SimpleEvents
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    UPDATE_COMMAND ""
) 

 ExternalProject_Add(
    TaskScheduler
    GIT_REPOSITORY https://github.com/lbuchman/TaskScheduler.git
    GIT_TAG main
    PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/extLib/TaskScheduler
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    UPDATE_COMMAND ""
) 

ExternalProject_Add(
    SRXL2
    GIT_REPOSITORY https://github.com/lbuchman/SRXL2.git
    GIT_TAG master
    PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/extLib/SRXL2
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    UPDATE_COMMAND ""
) 
ExternalProject_Get_Property(SRXL2 source_dir)
include_directories(${source_dir})
file(GLOB_RECURSE libs_source_cpp_files ${source_dir}/*.cpp)
ExternalProject_Get_Property(TaskScheduler source_dir)
include_directories(${source_dir}/src)
file(GLOB_RECURSE libs_source_cpp_files1 ${source_dir}/*.cpp)
list(APPEND libs_source_cpp_files " ${libs_source_cpp_files1}")
ExternalProject_Get_Property(SimpleEvents source_dir)
include_directories(${source_dir})
file(GLOB_RECURSE libs_source_cpp_files1 ${source_dir}/*.cpp)
list(APPEND libs_source_cpp_files " ${libs_source_cpp_files1}")
ExternalProject_Get_Property(loggingLib source_dir)
include_directories(${source_dir})
file(GLOB_RECURSE libs_source_cpp_files1  ${source_dir}/*.cpp)
list(APPEND libs_source_cpp_files " ${libs_source_cpp_files1}")
ExternalProject_Get_Property(CmdArduino source_dir)
include_directories(${source_dir})
file(GLOB_RECURSE libs_source_cpp_files1 ${source_dir}/*.cpp)


list(APPEND libs_source_cpp_files " ${libs_source_cpp_files1}")
message(STATUS ${libs_source_cpp_files})

file(GLOB_RECURSE app_source_cpp_files ./src/*.cpp)
# platform ctest_read_custom_files
file(GLOB_RECURSE app_platform_cpp_files ./linux/*.cpp ./teensy/*.cpp)

file(GLOB_RECURSE lib_cpp_files ./extLib/*.cpp)
file(GLOB_RECURSE lib_source_h_files ./extLib/*.h)

#message(FATAL_ERROR ${app_source_cpp_files})

add_executable(${PROJECT_NAME} ${app_source_h_files} ${app_source_cpp_files}  ${app_source_h_files} ${app_platform_cpp_files} ${lib_cpp_files} ${lib_source_h_files} ${target_source_cpp_files} ${target_source_c_files} ${target_source_h_files})

IF(NOT ${CMAKE_CROSSCOMPILING} MATCHES "1")
target_link_libraries(${PROJECT_NAME}
     m
     pthread
)
ELSE()
target_link_libraries(${PROJECT_NAME}
     ${MATH_LIB}
     stdc++ 
     ${teensyStaticLib}
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


set_target_properties(${PROJECT_NAME}
    PROPERTIES
        CXX_STANDARD 14
        CXX_STANDARD_REQUIRED YES
        CXX_EXTENSIONS YES
)

target_compile_options(${PROJECT_NAME} PUBLIC "$<$<CONFIG:DEBUG>:${MY_DEBUG_OPTIONS}>")
target_compile_options(${PROJECT_NAME} PUBLIC "$<$<CONFIG:RELEASE>:${MY_RELEASE_OPTIONS}>")
