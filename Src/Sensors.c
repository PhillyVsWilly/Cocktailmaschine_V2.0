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

//Read Module 1:
#if MODULE_1_USE_VIRTUAL_INPUT
	DPRINT_WARNING("Module 1: Using virtual Input");
	updateVirtualInput_Module_1(ptrInput);
#else
	vReadSensorValues_Module_1();
#endif

//Read Module 2:
#if MODULE_2_USE_VIRTUAL_INPUT
	DPRINT_WARNING("Module 2: Using virtual Input");
	updateVirtualInput_Module_2(ptrInput);
#else
	vReadSensorValues_Module_2();
#endif

//Read Module 3
#if MODULE_3_USE_VIRTUAL_INPUT
	DPRINT_WARNING("Module 3: Using virtual Input");
	updateVirtualInput_Module_3(ptrInput);
#else
	vReadSensorValues_Module_3();
#endif

//Read Module 4
#if MODULE_4_USE_VIRTUAL_INPUT
	DPRINT_WARNING("Module 4: Using virtual Input");
	updateVirtualInput_Module_4(ptrInput);
#else
	vReadSensorValues_Module_4();
#endif

//Read Module 5
#if MODULE_5_USE_VIRTUAL_INPUT
	DPRINT_WARNING("Module 5: Using virtual Input");
	updateVirtualInput_Module_5(ptrInput);
#else
	vReadSensorValues_Module_5();
#endif

//Read Module 6
#if MODULE_6_USE_VIRTUAL_INPUT
	DPRINT_WARNING("Module 6: Using virtual Input");
	updateVirtualInput_Module_6(ptrInput);
#else
	vReadSensorValues_Module_6();
#endif

//Read Module 7
#if MODULE_7_USE_VIRTUAL_INPUT
	DPRINT_WARNING("Module 7: Using virtual Input");
	updateVirtualInput_Module_7(ptrInput);
#else
	vReadSensorValues_Module_7();
#endif

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
