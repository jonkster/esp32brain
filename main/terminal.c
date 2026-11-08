#include <stdio.h>
#include <string.h>
#include <fcntl.h>

#include "esp_console.h"
#include "driver/uart.h"
#include "driver/uart_vfs.h"
#include "driver/usb_serial_jtag.h"
#include "driver/uart_vfs.h"
#include "linenoise/linenoise.h"
#include "argtable3/argtable3.h"

#include "common.h"
#include "commands.h"
#include "terminal.h"

/*
 * We warn if a secondary serial console is enabled. A secondary serial console is always output-only and
 * hence not very useful for interactive console applications. If you encounter this warning, consider disabling
 * the secondary serial console in menuconfig unless you know what you are doing.
 */
#if SOC_USB_SERIAL_JTAG_SUPPORTED
#if !CONFIG_ESP_CONSOLE_SECONDARY_NONE
#warning "A secondary serial console is not useful when using the console component. Please disable it in menuconfig."
#endif
#endif

#define PROMPT_STR			ANSI_BOLD_YELLOW ANSI_BG_BLUE CONFIG_IDF_TARGET "-brain>" ANSI_RESET " "

void registerCommands(void)
{
	esp_console_register_help_command();

	const esp_console_cmd_t commands[] = {
		{
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
    /* Drain stdout before reconfiguring it */
    fflush(stdout);
    fsync(fileno(stdout));
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);

#if defined(CONFIG_ESP_CONSOLE_UART_DEFAULT) || defined(CONFIG_ESP_CONSOLE_UART_CUSTOM)
	ESP_ERROR_CHECK(uart_driver_install(CONFIG_ESP_CONSOLE_UART_NUM, 256, 0, 0, NULL, 0));
	uart_vfs_dev_use_driver(CONFIG_ESP_CONSOLE_UART_NUM);
	uart_vfs_dev_port_set_rx_line_endings(CONFIG_ESP_CONSOLE_UART_NUM, ESP_LINE_ENDINGS_CR);
	uart_vfs_dev_port_set_tx_line_endings(CONFIG_ESP_CONSOLE_UART_NUM, ESP_LINE_ENDINGS_CRLF);


#elif defined(CONFIG_ESP_CONSOLE_USB_SERIAL_JTAG)
    /* Minicom, screen, idf_monitor send CR when ENTER key is pressed */
    usb_serial_jtag_vfs_set_rx_line_endings(ESP_LINE_ENDINGS_CR);
    /* Move the caret to the beginning of the next line on '\n' */
    usb_serial_jtag_vfs_set_tx_line_endings(ESP_LINE_ENDINGS_CRLF);

    /* Enable blocking mode on stdin and stdout */
    fcntl(fileno(stdout), F_SETFL, 0);
    fcntl(fileno(stdin), F_SETFL, 0);

    usb_serial_jtag_driver_config_t jtag_config = {
        .tx_buffer_size = 256,
        .rx_buffer_size = 256,
    };

    /* Install USB-SERIAL-JTAG driver for interrupt-driven reads and writes */
    ESP_ERROR_CHECK( usb_serial_jtag_driver_install(&jtag_config));

    /* Tell vfs to use usb-serial-jtag driver */
    usb_serial_jtag_vfs_use_driver();

#else
#error Unsupported console type
#endif

	esp_console_config_t consoleConfig = {
		.max_cmdline_args = CONFIG_CONSOLE_MAX_CMDLINE_ARGS,
		.max_cmdline_length = CONFIG_CONSOLE_MAX_COMMAND_LINE_LENGTH,
		.hint_color = atoi(ANSI_CYAN)
	};
	ESP_ERROR_CHECK( esp_console_init(&consoleConfig) );
}


void TaskTerminal(void *pvParameters) {
	(void)pvParameters;

	printf(ANSI_BG_GREEN ANSI_BLACK "\n------------------------------------\n");
	uname(0, NULL);
	version(0, NULL);
	printf("------------------------------------\n");
	printf(ANSI_BOLD_WHITE "Ctrl+] to exit monitor" "\n");
	printf("type help for help" ANSI_RESET "\n\n");

	initialiseConsole();

	registerCommands();

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
					printf("Internal Error: %s    (input='%s')\n", esp_err_to_name(err), line);
				}
			}
			linenoiseFree(line);
		}
		vTaskDelay(500 / portTICK_PERIOD_MS);
	}
}



