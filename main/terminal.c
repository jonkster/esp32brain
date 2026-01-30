#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"

#include "esp_app_desc.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_console.h"
#include "esp_vfs_cdcacm.h"

#include "driver/uart.h"
#include "driver/uart_vfs.h"
#include "linenoise/linenoise.h"
#include "argtable3/argtable3.h"

#include "common.h"
#include "terminal.h"

static const char* TAG = "console";
#define PROMPT_STR			ANSI_BOLD_YELLOW ANSI_BG_BLUE CONFIG_IDF_TARGET "-brain>" ANSI_RESET " "
#define CONSOLE_MAX_CMDLINE_ARGS	8
#define CONSOLE_MAX_CMDLINE_LENGTH	256


static int uname(int argc, char **argv)
{
	printf(UNAME "\n");
	return 0;
}

static int version(int argc, char **argv)
{
	const esp_app_desc_t *appInfo = esp_app_get_description();
	printf("Project\n");
	printf(" Project Name: %s\n", appInfo->project_name);
	printf(" Version: %s\n", appInfo->version);
	printf(" Compile time: %s %s\n", appInfo->time, appInfo->date);
	printf(" IDF Version: %s\n", appInfo->idf_ver);
	return 0;
}

static int systemStats(int argc, char **argv)
{
	esp_chip_info_t chip_info;
	uint32_t flash_size;
	esp_chip_info(&chip_info);
	uname(argc, argv);
    	printf("System\n");
	printf(" %s chip with %d CPU core(s)\n net: %s%s%s%s\n",
			CONFIG_IDF_TARGET,
			chip_info.cores,
			(chip_info.features & CHIP_FEATURE_WIFI_BGN) ? "WiFi" : "",
			(chip_info.features & CHIP_FEATURE_BT) ? ",BT" : "",
			(chip_info.features & CHIP_FEATURE_BLE) ? ",BLE" : "",
			(chip_info.features & CHIP_FEATURE_IEEE802154) ? ",802.15.4 (Zigbee/Thread)" : "");

	unsigned major_rev = chip_info.revision / 100;
	unsigned minor_rev = chip_info.revision % 100;
	printf(" silicon revision v%d.%d, ", major_rev, minor_rev);
	if(esp_flash_get_size(NULL, &flash_size) != ESP_OK) {
		printf("Get flash size failed");
		return 1;
	}

	printf(" %" PRIu32 "MB %s flash\n", flash_size / (uint32_t)(1024 * 1024),
			(chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

	printf(" minimum free heap size: %" PRIu32 " bytes\n", esp_get_minimum_free_heap_size());
    	printf(" free memory: %"PRIu32"\n", esp_get_free_heap_size());
	version(argc, argv);
    	return 0;
}

static int tasksInfo(int argc, char **argv)
{
    const size_t bytes_per_task = 40; /* see vTaskList description */
    char *task_list_buffer = malloc(uxTaskGetNumberOfTasks() * bytes_per_task);
    if (task_list_buffer == NULL) {
        ESP_LOGE(TAG, "failed to allocate buffer for vTaskList output");
        return 1;
    }
    fputs("Task Name\tStatus\tPrio\tHWM\tTask#\tAffinity\n", stdout);
    vTaskList(task_list_buffer);
    fputs(task_list_buffer, stdout);
    free(task_list_buffer);
    return 0;
}

static int reboot(int argc, char **argv)
{
	esp_restart();
	return 0;
}

void registerCommands(void)
{
	esp_console_register_help_command();

	const esp_console_cmd_t commands[] = {
		{
			.command = "system",
			.help = "Display system information",
			.hint = NULL,
			.func = &systemStats,
		}, {
			.command = "version",
			.help = "Get version",
			.hint = NULL,
			.func = &version,
		}, {
			.command = "uname",
			.help = "Identify System ",
			.hint = NULL,
			.func = &uname,
		}, {
			.command = "reboot",
			.help = "Reboot system",
			.hint = NULL,
			.func = &reboot,
		}, {
			.command = "tasks",
			.help = "Get information about running tasks",
			.hint = NULL,
			.func = &tasksInfo,
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



