#include "Sensors.h"
#include "stm32f3xx_hal.h"

void vReadSensorValues (InputValues_t* input)
{
		//Read the Button State (Pin 13 at GPIO C)
    input->Button = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);
		//read, and save the inputs here
}
