#ifndef _MAIN_H
#define _MAIN_H

#if defined(SW_SERIAL_ENABLE)
#include <SoftwareSerial.h>
SoftwareSerial *sserial;
#endif

#include "debug.h"

#ifdef OLED_ENABLE
  #include <Tiny4kOLED.h>
  // Screen settings
  #define SCREEN_WIDTH     128 
  #define SCREEN_HEIGHT     64 
  #define SCREEN_MAX_ROWS    8
  #define SCREEN_MAX_COLS   21
  #define SCREEN_ROW_HEIGHT 16
  #define OLED_RESET        -1
  SSD1306Device * display = &oled;
#endif

void serial_bridge_loop();
void pulse(int pin, int times);

#ifdef OLED_ENABLE
void logo_loop(SSD1306Device*);
void logo_show(SSD1306Device*);
#endif

#endif