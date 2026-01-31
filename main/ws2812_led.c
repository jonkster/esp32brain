#include "driver/gpio.h"
#include "common.h"
#include "led_strip.h"
#include "ws2812_led.h"

static bool ready;
static led_strip_rmt_config_t rmtConfig;
static led_strip_config_t     stripConfig;
static led_strip_handle_t     ledStrip;

void colourLedOff(void)
{
	if (ready) {
		ESP_ERROR_CHECK(led_strip_clear(ledStrip));
	}
}

void colourSetup(void)
{
	led_strip_rmt_config_t rmt_config = {
		.clk_src = RMT_CLK_SRC_DEFAULT,
		.resolution_hz = 0,     // use default
		.mem_block_symbols = 0, // use default
		.flags = { .with_dma = false }
	};

	led_strip_config_t strip_config = {
		.strip_gpio_num = COLOUR_LED_GPIO,
		.max_leds = 1,
		.led_model = LED_MODEL_WS2812,
		.color_component_format = LED_STRIP_COLOR_COMPONENT_FMT_GRB,
		.flags = { .invert_out = false }
	};
	rmtConfig = rmt_config;
	stripConfig = strip_config;

	ESP_ERROR_CHECK(led_strip_new_rmt_device(&stripConfig, &rmtConfig, &ledStrip));
	ready = true;
}

void colourLed(uint8_t r, uint8_t g, uint8_t b)
{
	if (ready) {
		ESP_ERROR_CHECK(led_strip_del(ledStrip));
		ESP_ERROR_CHECK(led_strip_new_rmt_device(&stripConfig, &rmtConfig, &ledStrip));

		ESP_ERROR_CHECK(led_strip_set_pixel(ledStrip, 0, r, g, b));
		ESP_ERROR_CHECK(led_strip_refresh(ledStrip));
	}
}

bool flashCallback(void)
{
	static bool flashOn = false;
	static int flashCount = 0;

	if (flashOn) {
		colourLed(ALERT_COLOURS_1);
	} else {
		colourLed(ALERT_COLOURS_2);
	}
	flashOn = ! flashOn;
	if (flashCount++ > 30) {
		flashCount = 0;
		colourLed(NORMAL_COLOURS);
		return false;
	}
	return true;
}

void flashColourLed(void)
{
	if (ready) {
		colourLed(ALERT_COLOURS_2);
		while (flashCallback()) {
			vTaskDelay( 100 / portTICK_PERIOD_MS);
		}
	}
}








