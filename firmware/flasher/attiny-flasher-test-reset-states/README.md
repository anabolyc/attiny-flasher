# ATtiny Flasher firmware

![Open Source Hardware](/doc/images/open-source-hardware-logo.png)
![Open Source Software](/doc/images/open-source-software-logo.png)

## Introduction

This is debugging firmware code for [ATtiny Flasher](https://sonocotta.com/attiny-flasher/).

It's purpose is to test different programming modes, and communication means. 

Board uses 74HC241 octal-buffer IC, where 
- 3 buffers are used for CLK, MISO, MOSI lines in low-voltage programing mode
- 4 buffers are used for SDI, SII, SCI, SDO lines in high-voltage programing mode
- reset is controlled via 3 dedicated pins
  - TINY_RES_MD switches between 5V and 12V voltage source
  - TINY_RES_EN switches between RESET HIGH and high-impedance state 
  - TINY_RES_PULL switches between RESET LOW and high-impedance state 

This formware will cycle through following modes
- Buffer is off, Reset is Z state - normal target MCU operation
- Buffer is on, Reset is LOW, LV pins blinking - low voltage programmer mode reset
- Buffer is on, Reset is HIGH, LV pins blinking - low voltage programmer mode
- Buffer is on, Reset is LOW, HV pins blinking - high voltage programmer mode reset
- Buffer is on, Reset is +12V, HV pins blinking - high voltage programmer mode

## Links

- [Crowd Supply campaign](https://www.crowdsupply.com/sonocotta/attiny-flasher)
- [ATtiny Flasher - Product site](https://sonocotta.com/attiny-flasher/)
- [ATtiny Flasher Kit - Product site](https://sonocotta.com/attiny-flasher-kit/)
