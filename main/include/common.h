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


#define ANSI_BLACK "\x1b[30m"
#define ANSI_RED "\x1b[31m"
#define ANSI_GREEN "\x1b[32m"
#define ANSI_YELLOW "\x1b[33m"
#define ANSI_BLUE "\x1b[34m"
#define ANSI_MAGENTA "\x1b[35m"
#define ANSI_CYAN "\x1b[36m"
#define ANSI_WHITE "\x1b[37m"
#define ANSI_RESET "\x1b[0m"
#define ANSI_BG_BLACK "\x1b[40m"
#define ANSI_BG_RED "\x1b[41m"
#define ANSI_BG_GREEN "\x1b[42m"
#define ANSI_BG_YELLOW "\x1b[43m"
#define ANSI_BG_BLUE "\x1b[44m"
#define ANSI_BG_PURPLE "\x1b[45m"
#define ANSI_BG_CYAN "\x1b[46m"
#define ANSI_BG_WHITE "\x1b[47m"

#define ANSI_BOLD_BLACK "\x1b[1;90m"
#define ANSI_BOLD_RED "\x1b[1;91m"
#define ANSI_BOLD_GREEN "\x1b[1;92m"
#define ANSI_BOLD_YELLOW "\x1b[1;93m"
#define ANSI_BOLD_BLUE "\x1b[1;94m"
#define ANSI_BOLD_PURPLE "\x1b[1;95m"
#define ANSI_BOLD_CYAN "\x1b[1;96m"
#define ANSI_BOLD_WHITE "\x1b[1;97m"

#endif
