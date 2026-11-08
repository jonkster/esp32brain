#ifndef _LED_H_
#define _LED_H_

#include <stdlib.h>

void setBlinkPeriod(int period);

void setAutoBlink(bool state);

void setMainLedOnOff(bool state);

void TaskMainLed(void *pvParameters);

#endif
