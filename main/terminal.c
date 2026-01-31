#include <stdio.h>
#include <string.h>

#include "esp_console.h"
#include "driver/uart.h"
#include "driver/uart_vfs.h"
#include "linenoise/linenoise.h"
#include "argtable3/argtable3.h"

#include "common.h"
#include "commands.h"
#include "terminal.h"
#include "ws2812_led.h"

#define PROMPT_STR			ANSI_BOLD_YELLOW ANSI_BG_BLUE CONFIG_IDF_TARGET "-brain>" ANSI_RESET " "
#define CONSOLE_MAX_CMDLINE_ARGS	8
#define CONSOLE_MAX_CMDLINE_LENGTH	256

void registerCommands(void)
{
	esp_console_register_help_command();

	const esp_console_cmd_t commands[] = {
		{
			.command = "alert",
			.help = "Flash the alert LED",
			.hint = NULL,
			.func = &alertLed,
		}, {
			.command = "ps",
			.help = "Get information about running tasks",
			.hint = NULL,
			.func = &tasksInfo,
		}, {
			.command = "reboot",
			.help = "Reboot system",
			.hint = NULL,
			.func = &reboot,
		}, {
			.command = "system",
			.help = "Display system information",
			.hint = NULL,
			.func = &systemStats,
		}, {
			.command = "uname",
				.help = "Identify System ",
				.hint = NULL,
				.func = &uname,
		}, {
			.command = "version",
				.help = "Get version",
				.hint = NULL,
				.func = &version,
		}
	};

	for (int i = 0; i < ARRAY_LENGTH(commands); i++) {
		ESP_ERROR_CHECK( esp_console_cmd_register(&commands[i]) );
	}
	return;
}

void initialiseConsole(void)
{
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);
	ESP_ERROR_CHECK(uart_driver_install(CONFIG_ESP_CONSOLE_UART_NUM, 256, 0, 0, NULL, 0));
	uart_vfs_dev_use_driver(CONFIG_ESP_CONSOLE_UART_NUM);
	uart_vfs_dev_port_set_rx_line_endings(CONFIG_ESP_CONSOLE_UART_NUM, ESP_LINE_ENDINGS_CR);
	uart_vfs_dev_port_set_tx_line_endings(CONFIG_ESP_CONSOLE_UART_NUM, ESP_LINE_ENDINGS_CRLF);

	esp_console_config_t consoleConfig = {
		.max_cmdline_args = CONSOLE_MAX_CMDLINE_ARGS,
		.max_cmdline_length = CONSOLE_MAX_CMDLINE_LENGTH,
		.hint_color = atoi(ANSI_CYAN)
	};
	ESP_ERROR_CHECK( esp_console_init(&consoleConfig) );
}


void TaskTerminal(void *pvParameters) {
	(void)pvParameters;

	uname(0, NULL);
	version(0, NULL);

	initialiseConsole();
	registerCommands();
	printf("Ctrl+] to exit monitor\n");

	for (;;) {

		char* line = linenoise(PROMPT_STR);
		if (line != NULL) {
			if (strlen(line) > 0) {

				linenoiseHistoryAdd(line);
				int ret;
				esp_err_t err = esp_console_run(line, &ret);

				if (err == ESP_ERR_NOT_FOUND) {
					printf("Unrecognized command\n");
				} else if (err == ESP_ERR_INVALID_ARG) {

				} else if (err == ESP_OK && ret != ESP_OK) {
					printf("Command returned non-zero error code: 0x%x (%s)\n", ret, esp_err_to_name(ret));
				} else if (err != ESP_OK) {
					printf("Internal error: %s\n", esp_err_to_name(err));
				}
			}
			linenoiseFree(line);
		}
		vTaskDelay(500 / portTICK_PERIOD_MS);

	}
}



