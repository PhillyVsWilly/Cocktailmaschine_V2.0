#include "Actuators.h"

#include "stm32f7xx_nucleo_144.h"


void vInitActuatorValues(OutputValues_t* output_values)
{
	output_values->LED2 = 0;
}

void vWriteActuatorValues(OutputValues_t output)
{
	if(output.LED2 == 1){
		BSP_LED_On(LED2);
	} else { 
		BSP_LED_Off(LED2);
	}
}
