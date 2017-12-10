cmake_minimum_required(VERSION 3.7)
project(BedBotHardware)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        algorithm.c
        algorithm.h
        demo.c
        gpio.c
        init.sh
        MAX30102.c
        MAX30102.h
        mbed.h
        README.md
        tempSensor.c)

add_executable(BedBotHardware ${SOURCE_FILES})