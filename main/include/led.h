#ifndef _LED_H_
#define _LED_H_

#include <stdlib.h>

#define LED GPIO_NUM_2
#define LED_FLASH_PERIOD_MS 2000

void setBlinkPeriod(int period);

void setAutoBlink(bool state);

void setMainLedOnOff(bool state);

void TaskMainLed(void *pvParameters);

#endif
