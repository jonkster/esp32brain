#include <stdio.h>
#include "driver/gpio.h"
#include "common.h"
#include "led.h"

bool autoBlink = true;

int flashPeriod = LED_FLASH_PERIOD_MS;

void setBlinkPeriod(int period) {
	flashPeriod = period;
}

void setAutoBlink(bool state)
{
	autoBlink = state;
}

void setMainLedOnOff(bool state)
{
	gpio_set_level(LED, !state);
}

void TaskMainLed(void *pvParameters) {
	(void)pvParameters;

	static bool ledState = true;

	esp_rom_gpio_pad_select_gpio(LED);
	gpio_set_direction(LED, GPIO_MODE_OUTPUT);

	for (;;) {
		if (autoBlink) {
			setMainLedOnOff(ledState);
			ledState = ! ledState;
		}
		vTaskDelay(flashPeriod / portTICK_PERIOD_MS);
	}
}



