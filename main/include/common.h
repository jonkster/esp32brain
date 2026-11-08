#ifndef _COMMON_H_
#define _COMMON_H_

#include <stdbool.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#define UNAME "Brain (c)2026 Jon Kelly"


#define ARRAY_LENGTH(x) (sizeof(x) / sizeof((x)[0]))
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

#endif
