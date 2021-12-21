# ATtiny Flasher firmware

![Open Source Hardware](/doc/images/open-source-hardware-logo.png)
![Open Source Software](/doc/images/open-source-software-logo.png)

## Introduction

This is firmware code for [ATtiny Flasher](https://sonocotta.com/attiny-flasher/).

It is based on Arduino Framework and currently cupports ISP flasing using STK500v1 protocol in both high voltage and low voltage modes.

I'm trying to keep support of all board revisions to simplify board development and testing, therefore list of build configurations is growing all the time. 

Each new revision of the board is adding some capabilities and more code required to support added features.

Code is shaped as Platformio project and can be built and flashed to multiple board revisions from the same code base.

## Basic structure

[main.cpp](src/main.cpp) is responsible for hardware initialisations and main programm loop. It owns host Serial communication, therefore also responsible for bi-directional Serial bridge logic between target MCU and the HOST.

Whenever Serial input is captured it is sent to [stk500.cpp](src/stk500.cpp) superclass, that has two implementations - low voltage and high voltage programmers. 

Having STK500 protocol implementation in [stk500.cpp](src/stk500.cpp), [isp.cpp] would implement target MCU communication layer in low-voltage programming mode, [hvsp.cpp](src/hvsp.cpp) in turn implements high-voltage programming mode. 

Switch between two is handled by main loop, which checking on physical switch digital input.

[boards.h](src/boards.h) has hardware definition for each revision, hiding away most of the pinout and feature richness behind macros.

[debug.h](src/debug.h) is a debugging toolset, that normally passes through debugged calls when project built normally. However when SERIAL_DEBUG_ENABLE flag is set, it will drop traces to software serial interface on pins (PB4, PB5), which can be captured with second serial monitor (first being the one connected to host using built in Serial adapter, since it is booked for STK communication and cannot be used for debugging)

## Links

- [Crowd Supply campaign](https://www.crowdsupply.com/sonocotta/attiny-flasher)
- [ATtiny Flasher - Product site](https://sonocotta.com/attiny-flasher/)
- [ATtiny Flasher Kit - Product site](https://sonocotta.com/attiny-flasher-kit/)
