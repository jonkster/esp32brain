#ifndef _RELAY_H_
#define _RELAY_H_

#include <stdlib.h>

#define RELAY_COUNT 4 
#define RELAYS      (uint16_t[]){GPIO_NUM_12, GPIO_NUM_14, GPIO_NUM_27, GPIO_NUM_26}

void initRelays(void);

int relayDance(int argc, char** argv);

void relaySet(const uint8_t relayIdx, const bool onoff);

#endif
