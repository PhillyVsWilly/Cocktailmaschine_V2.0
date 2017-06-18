#include "main.h"
#include "Sensors.h"
#include "stm32f3xx_hal.h"
#include "VirtualInput.h"

#define DEBUG_ENABLED TRUE
#include "Debug.h"

static InputValues_t* ptrInput;

extern ADC_HandleTypeDef hadc1, hadc3;

void vReadSensorValues (InputValues_t* input)
{
	ptrInput = input;

//Read Module 1
#if MODULE_1_USE_VIRTUAL_INPUT
	DPRINT_WARNING("Module 1: Using virtual Input");
	updateVirtualnput_Module_1(ptrInput);
#else
	vReadSensorValues_Module_1();
#endif

//TODO: Bitte für alle Module wiederholen!

}

 void vReadSensorValues_Module_1()
{
	//Read the Button State (Pin 13 at GPIO C)
    ptrInput->Button = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);
	//read, and save the inputs here
}

 void vReadSensorValues_Module_2()
{
	return;
}

 void vReadSensorValues_Module_3()
{
	return;
}

 void vReadSensorValues_Module_4()
{
	return;
}

 void vReadSensorValues_Module_5()
{
	return;
}

 void vReadSensorValues_Module_6()
{
	return;
}

 void vReadSensorValues_Module_7()
{
	return;
}


 /***************************************************/
 void vReadAnalogPin(GPIO_TypeDef* port, int pin)
 {
 	ADC_HandleTypeDef adc;

 	if(port == GPIOA)
 	{
 		adc = hadc1;
 	}
 	else
 	{
 		adc = hadc3;
 	}
 	//Delete later
 	HAL_ADC_Start(&adc);
 	  /* Wait for the end of conversion */
 		if (HAL_ADC_PollForConversion(&adc, 10) != HAL_TIMEOUT)
 	{
 		/* Get the converted value of regular channel */
 		int KeyConvertedValue = HAL_ADC_GetValue(&adc);
 		DPRINT_MESSAGE("ADC Value: %d\r\n", KeyConvertedValue);
 	}

 	else
 	{
 		DPRINT_MESSAGE("ADC Timeout");
 	}
 }
