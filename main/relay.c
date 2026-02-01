#include <stdio.h>
#include "driver/gpio.h"
#include "common.h"
#include "relay.h"

bool relayState[] = { false, false, false, false };

void initRelays(void)
{
	for (uint8_t i = 0; i < RELAY_COUNT; i++) {
		esp_rom_gpio_pad_select_gpio(RELAYS[i]);
		gpio_set_direction(RELAYS[i], GPIO_MODE_OUTPUT);
		relayState[i] = false;
	}
}

int relayDance(int argc, char** argv)
{
	for (uint8_t i = 0; i < RELAY_COUNT; i++) {
		relaySet(i, 1);
		vTaskDelay( 500 / portTICK_PERIOD_MS);
	}
	for (uint8_t i = 0; i < RELAY_COUNT; i++) {
		relaySet(i, 0);
		vTaskDelay( 500 / portTICK_PERIOD_MS);
	}
	return 0;
}

void relaySet(const uint8_t relayIdx, const bool onoff)
{
	const bool currentState = relayState[relayIdx];
	if (onoff != currentState) {
		gpio_set_level(RELAYS[relayIdx], 1);
		vTaskDelay( 100 / portTICK_PERIOD_MS);
		gpio_set_level(RELAYS[relayIdx], 0);
		relayState[relayIdx] = onoff;
	}
}


