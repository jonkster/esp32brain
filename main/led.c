#include <stdio.h>
#include "driver/gpio.h"
#include "common.h"
#include "led.h"

bool autoBlink = true;

int flashPeriod = CONFIG_LED_FLASH_PERIOD_MS;

void setBlinkPeriod(int period) {
	flashPeriod = period;
}

void setAutoBlink(bool state)
{
	autoBlink = state;
}

void setMainLedOnOff(bool state)
{
	gpio_set_level(CONFIG_BLINK_GPIO, !state);
}

void TaskMainLed(void *pvParameters) {
	(void)pvParameters;

	static bool ledState = true;

	esp_rom_gpio_pad_select_gpio(CONFIG_BLINK_GPIO);
	gpio_set_direction(CONFIG_BLINK_GPIO, GPIO_MODE_OUTPUT);

	for (;;) {
		if (autoBlink) {
			setMainLedOnOff(ledState);
			ledState = ! ledState;
		}
		vTaskDelay(flashPeriod / portTICK_PERIOD_MS);
	}
}



