# ATtiny Flasher and ATtiny Flasher Kits project files

![Open Source Hardware](/doc/images/open-source-hardware-logo.png)
![Open Source Software](/doc/images/open-source-software-logo.png)

ATtiny Flasher is a handy device that allows you to use your favorite IDEs and debugging tools when developing for the ATtiny microcontroller. Enjoy the classic Arduino development experience while working with bare metal. Whether you’re a design engineer, a hobbyist, or someone who’s just getting started programming MCUs, ATtiny Flasher can help you get from a great idea to a great implementation—without having to port your code to its intended platform when you’re done

After implementing numerous projects using the wonderful ATtiny family of MCUs, we’ve come away with a number of ideas about how to simplify the process. Our goal is to make ATtiny chips as easy to work with as more "grown up" MCUs:
- We want a traditional serial terminal for debugging. Ideally, we want two—one via serial data transferred to the host over USB, and another via the onboard display.
- We want some GPIO LEDs to play with. In fact, why not have LEDs for all GPIOs?
- Speaking of GPIOs, we want to use the RESET pin as a GPIO too, so we'll need an on-board high-voltage programmer.
- In general, we want to upload code to breadboard-connected devices and see that code in action immediately.
- And finally, since our debugger will be connected to a PC via USB, it should handle 3.3 V and 5 V breadboard power rails as well

## ATTiny Flasher Features

- Flash ATtiny85, ATtiny45, ATtiny25, and ATtiny13 via breadboard or onboard header
- Flash the entire Atmega MCU family using the ISP header
- Flash using Arduino IDE, Platformio, or the IDE of your choice
- Two configurable power rails, either 3.3 V or 5 V
- Supports high-voltage programming
- Supports UPDI programming with dedicated header
- Supports TPI programming (after certain HW modifications)
- Six GPIO "weak" LEDs
- Three flash-status LEDs
- Onboard OLED screen connected to target MCU as a live terminal
- Streams serial data to the host PC as if it were connected directly to the target MCU. (Yes, serial on ATtiny. Even ATtiny13!)
- A buffer IC between target and host MCUs only connects the two while flashing
- Open hardware driven by open source software

## Educational ATtiny Project Kits

Having built a product that ticks all of those boxes, we wanted to put ATtiny through its paces, so we designed a collection of simple, affordable, breadboard-compatible project kits that are easy to understand, fun to build, satisfying to use, and well suited to extension and modification. There are five of them in all, and they are a great way to learn about ATtiny programming:

- Traffic Light: ATtiny85-driven "traffic light"
- Rubber Ducky: ATtiny85-driven "USB rubber ducky" tool capable of emulating keyboard & mouse devices
- IR Messenger: ATtiny85-driven IR remote transmitter & receiver
- Rainbow LED: ATtiny85-driven circle of addressable LEDs
- Digital Clock: ATtiny85-driven digital clock

## Hardware

Please find more information in the [hardware](/hardware) section

## Firmware

Please find more information in the [software](/hardware) section

## Where to buy

Shortly will be available to order from [Mouser](https://mouser.com).

Later on will be shared on [Tindie](https://www.tindie.com/) store.

## Links

- [Crowd Supply campaign](https://www.crowdsupply.com/sonocotta/attiny-flasher)
- [ATtiny Flasher - Product site](https://sonocotta.com/attiny-flasher/)
- [ATtiny Flasher Kit - Product site](https://sonocotta.com/attiny-flasher-kit/)
