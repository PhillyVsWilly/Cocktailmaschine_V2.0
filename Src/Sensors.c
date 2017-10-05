#include "main.h"
#include "Sensors.h"
#include "stm32f7xx_hal.h"
#include "VirtualInput.h"
#include "project_conf.h"
#include "math.h"

#define DEBUG_ENABLED TRUE
#include "Debug.h"

#if DEBUG_ENABLED
void vPrintReadSensorValues(InputValues_t);
#endif

void vReadSensorValues (InputValues_t* ptrInput)
{

//Read Module 1:
#if MODULE_1_USE_VIRTUAL_INPUT
	DPRINT_WARNING("Module 1: Using virtual Input");
	updateVirtualInput_Module_1(ptrInput);
#else
	vReadSensorValues_Module_1(ptrInput);
#endif


//Read Module 2:
#if MODULE_2_USE_VIRTUAL_INPUT
	DPRINT_WARNING("Module 2: Using virtual Input");
	updateVirtualInput_Module_2(ptrInput);
#else
	vReadSensorValues_Module_2(ptrInput);
#endif


//Read Module 3
#if MODULE_3_USE_VIRTUAL_INPUT
	DPRINT_WARNING("Module 3: Using virtual Input");
	updateVirtualInput_Module_3(ptrInput);
#else
	vReadSensorValues_Module_3(ptrInput);
#endif


//Read Module 4
#if MODULE_4_USE_VIRTUAL_INPUT
	DPRINT_WARNING("Module 4: Using virtual Input");
	updateVirtualInput_Module_4(ptrInput);
#else
	vReadSensorValues_Module_4(ptrInput);
#endif

//Read Module 5
#if MODULE_5_USE_VIRTUAL_INPUT
	DPRINT_WARNING("Module 5: Using virtual Input");
	updateVirtualInput_Module_5(ptrInput);
#else
	vReadSensorValues_Module_5(ptrInput);
#endif

//Read Module 6
#if MODULE_6_USE_VIRTUAL_INPUT
	DPRINT_WARNING("Module 6: Using virtual Input");
	updateVirtualInput_Module_6(ptrInput);
#else
	vReadSensorValues_Module_6(ptrInput);
#endif

//Read Module 7
#if MODULE_7_USE_VIRTUAL_INPUT
	DPRINT_WARNING("Module 7: Using virtual Input");
	updateVirtualInput_Module_7(ptrInput);
#else
	vReadSensorValues_Module_7(ptrInput);
#endif

#if DEBUG_ENABLED
	vPrintReadSensorValues(*ptrInput);
#endif

}


 void vReadSensorValues_Module_1(InputValues_t* ptrInput)
{
	return;
}

 void vReadSensorValues_Module_2(InputValues_t* ptrInput)
{
	ptrInput->Gravity.weight_sensor = ADC3ConvertedValues[1] / FACTOR_BITS_TO_GRAMMS; //A4
	ptrInput->Gravity.doors_open = HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_3); // PE3 digital
	ptrInput->Gravity.sensor_up = HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_8); // PF8
	ptrInput->Gravity.sensor_down = HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_7); // PF7
	ptrInput->Gravity.button_fill_in = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15); // PB15
	ptrInput->Gravity.position_tree = ((int)ADC1ConvertedValues[1]) / FACTOR_BITS_TO_ANGLE; //A6 (PB1)
	
	return;
}

 void vReadSensorValues_Module_3(InputValues_t* ptrInput)
{
	 ptrInput->Pumping.doors_open = HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_2); // PE2
	 ptrInput->Pumping.valve_position = ((int)ADC3ConvertedValues[0]) / FACTOR_BITS_TO_POSITION; // A2
	 ptrInput->Pumping.weight_glass = ((int)ADC2ConvertedValues[0] - OFFSET_WEIGHT) / FACTOR_BITS_TO_GRAMMS; // A1
	return;
}

 void vReadSensorValues_Module_4(InputValues_t* ptrInput)
{
	 ptrInput->Pouring.doors_open = HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_4); // PE4
	 ptrInput->Pouring.position_up = HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_6); // PE6
	 ptrInput->Pouring.position_down = HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_5); // PE5
	 ptrInput->Pouring.weight = ((int)ADC3ConvertedValues[3] - OFFSET_WEIGHT) / FACTOR_BITS_TO_GRAMMS;// A3

	return;
}

 void vReadSensorValues_Module_5(InputValues_t* ptrInput)
{
	 ptrInput->Sensors.start_doors_open = HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_0); // PD0
	 ptrInput->Sensors.start_light_barrier = HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_2); // PF2
	 ptrInput->Sensors.end_doors_open = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_9); // PB9
	 ptrInput->Sensors.end_light_barrier = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15); // PA15
	 ptrInput->Sensors.end_button_glass_present = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12); // PB12
	return;
}

 void vReadSensorValues_Module_6(InputValues_t* ptrInput)
{
	 

	return;
}

 void vReadSensorValues_Module_7(InputValues_t* ptrInput)
{
	 	 ptrInput->Ice.weight = ((int)ADC1ConvertedValues[0] - OFFSET_WEIGHT)/FACTOR_BITS_TO_GRAMMS; // A0
	 	 ptrInput->Ice.doors_open = HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_1); // PF1
	 	 ptrInput->Ice.light_barrier_enough_cube_ice = HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_0); // PF0
	 	 ptrInput->Ice.light_barrier_enough_crushed_ice = HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_9); // PF9
	 return;
}




#if DEBUG_ENABLED
void vPrintReadSensorValues(InputValues_t input)
{
	/*printf("====================\n%s\n==============\n", "Sensor Values");
	printf("Gravity Modul: \nGravity Weight: %d\nDoor Open: %d\nSensorUp: %d\nSensorDown: %d\nButton Fill: %d\nPosition Tree: %d\n",
			(int)roundf(input.Gravity.weight_sensor), input.Gravity.doors_open, input.Gravity.sensor_up,
			input.Gravity.sensor_down, input.Gravity.button_fill_in);
	printf("Pump Modul: \nDoor Open: %d\nValve Position: %d\nWeightGlass: %d\n",
			input.Pumping.doors_open, input.Pumping.valve_position, (int)roundf(input.Pumping.weight_glass));*/
	printf("Pouring Modul: \nDoor Open: %d\nPosition Up: %d\nPosition Down: %d\nWeight: %d\n",
			input.Pouring.doors_open, input.Pouring.position_up, input.Pouring.position_down,
			(int)roundf(input.Pouring.weight));
	/*printf("Sensor Modul: \nStart Door Open: %d\nStart Light Barrier: %d\nEnd Door Open: %d\nEnd Light Barrier: %d\nButton glass present: %d\n",
			input.Sensors.start_doors_open, input.Sensors.start_light_barrier, input.Sensors.end_doors_open,
			input.Sensors.end_light_barrier, input.Sensors.end_button_glass_present);
	printf("Handling: Leer\n");
	printf("Gewicht Modul: \nWeight: %d\nDoor open: %d\nLight Barrier enough cube ice: %d\nLight Barrier enough crushed ice: %d\n",
			(int)roundf(input.Ice.weight), input.Ice.doors_open,
			input.Ice.light_barrier_enough_cube_ice, input.Ice.light_barrier_enough_crushed_ice);*/




}
#endif
