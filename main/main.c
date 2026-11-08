#include <stdio.h>
#include "common.h"
#include "led.h"
#include "terminal.h"


void app_main(void)
{
	xTaskCreate(
			TaskMainLed,
			"Task Flash" ,
			configMINIMAL_STACK_SIZE ,
			NULL,
			4 ,
			NULL
		   );


	xTaskCreate(
			TaskTerminal,
			"Task Terminal" ,
			16 * configMINIMAL_STACK_SIZE ,
			NULL,
			3 ,
			NULL
		   );
}
