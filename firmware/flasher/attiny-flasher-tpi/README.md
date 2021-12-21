# ATtiny Flasher TPI flashing firmware

![Open Source Hardware](/doc/images/open-source-hardware-logo.png)
![Open Source Software](/doc/images/open-source-software-logo.png)

## Introduction

This is TPI flashing firmware code for [ATtiny Flasher](https://sonocotta.com/attiny-flasher/).

This is Proof-of-Concept code that allows to establish basic communication with ATtiny chips using TPI protocol.

This is a work in progress and not yet ready to be used directly with an IDE, but rather as a standalone tool.

Note that TPI protocol physical layer uses resistor network to tie togeter MISO and MOSI pins, to allow bi-directional communication using single pin. Therefore it is disabled by default on ATtiny Flasher. However it can be enabled by shorting missing resistor. This way Flasher stays fully functional, but MISO and MOSI pins will mix communication. 

## Links

- [Crowd Supply campaign](https://www.crowdsupply.com/sonocotta/attiny-flasher)
- [ATtiny Flasher - Product site](https://sonocotta.com/attiny-flasher/)
- [ATtiny Flasher Kit - Product site](https://sonocotta.com/attiny-flasher-kit/)
