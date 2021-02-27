#Copyright (c) Microsoft. All rights reserved.
#Lic`sed under the MIT license. See LICENSE file in the project root for full license information.


#cmake -DRPI2=1 -DCMAKE_TOOLCHAIN_FILE=../arm-linux-gnueabihf.cmake ..

cmake_minimum_required(VERSION 3.0)
set(PROJECT_NAME SRXL2_SPY)

project (thruststand)

set(CMAKE_VERBOSE_MAKEFILE ON)


set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s -Os -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s -std=gnu++17 -Os -Wall")

set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_DEBUG}  -O0 -Wall -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_DEBUG}  -std=gnu++17 -O0 -g -Wall")



# Application files
file(GLOB_RECURSE app_source_cpp_files ./src/*.cpp)
file(GLOB_RECURSE app_source_h_files ./inc/*.h)
include_directories(./ ./inc)


add_executable(${PROJECT_NAME} ${app_source_h_files} ${app_source_cpp_files}  ${app_source_h_files} )

target_link_libraries(${PROJECT_NAME}
     m
     pthread
)

