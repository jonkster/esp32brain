#include <stdio.h>
#include "common.h"
#include "terminal.h"
#include "led.h"


void app_main(void)
{
	xTaskCreate(TaskMainLed,
			"Task Flash" ,
			configMINIMAL_STACK_SIZE ,
			NULL,
			4 ,
			NULL
		   );


	xTaskCreate(TaskTerminal,
			"Task Terminal" ,
			16 * configMINIMAL_STACK_SIZE ,
			NULL,
			3 ,
			NULL
		   );

}
