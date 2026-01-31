#ifndef _WS2812_LED_H_
#define _WS2812_LED_H_

#include "common.h"

// 10MHz resolution, 1 tick = 0.1us (led strip needs a high resolution)
#define LED_STRIP_RMT_RES_HZ  (10 * 1000 * 1000)

#define COLOUR_LED_GPIO     GPIO_NUM_16

#define NORMAL_COLOURS   0, 30,  1
#define ALERT_COLOURS_1 10, 30, 10
#define ALERT_COLOURS_2 40,  0,  0


void colourSetup(void);
void colourLedOff(void);
void colourLed(uint8_t r, uint8_t g, uint8_t b);
void flashColourLed(void);


#endif
