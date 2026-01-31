#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "esp_app_desc.h"
#include "esp_chip_info.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_app_desc.h"
#include "esp_flash.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_console.h"
#include "esp_vfs_cdcacm.h"

#include "common.h"
#include "commands.h"
#include "led.h"
#include "ws2812_led.h"

static const char* TAG = "commands";


int alertLed(int argc, char **argv)
{
	flashColourLed();
	return 0;
}

int reboot(int argc, char **argv)
{
	esp_restart();
	return 0;
}

int setRelay(int argc, char **argv)
{
	if (argc < 2) {
		printf("no relay specified: useage: setRelay nn <0|1>\n");
		return 1;
	}
	if (argc < 3) {
		printf("no relay value specified: useage: setRelay nn <0|1>\n");
		return 1;
	}
	const char* relayNum = argv[1];
	const char* relayVal = argv[2];
	printf("setRelay #%s to %s, not implemented yet\n", relayNum, relayVal);
	return 1;
}

int getRelay(int argc, char **argv)
{
	if (argc < 2) {
		printf("no relay specified: useage: getRelay nnn\n");
		return 1;
	}
	const char* relayNum = argv[1];
	printf("getRelay #%s, not implemented yet\n", relayNum);
	return 1;
}

int systemStats(int argc, char **argv)
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

int tasksInfo(int argc, char **argv)
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


int uname(int argc, char **argv)
{
	printf(UNAME "\n");
	return 0;
}

int version(int argc, char **argv)
{
	const esp_app_desc_t *appInfo = esp_app_get_description();
	printf("Project\n");
	printf(" Project Name: %s\n", appInfo->project_name);
	printf(" Version: %s\n", appInfo->version);
	printf(" Compile time: %s %s\n", appInfo->time, appInfo->date);
	printf(" IDF Version: %s\n", appInfo->idf_ver);
	return 0;
}


