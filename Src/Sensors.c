#include "main.h"
#include "Sensors.h"
#include "stm32f7xx_hal.h"
#include "VirtualInput.h"
#include "project_conf.h"

#define DEBUG_ENABLED TRUE
#include "Debug.h"

#if DEBUG_ENABLED
void vPrintReadSensorValues(InputValues_t);
#endif

static InputValues_t* ptrInput;

extern ADC_HandleTypeDef hadc1, hadc3;

int ValueAnalogPin0;
int ValueAnalogPin1;
int ValueAnalogPin2;
int ValueAnalogPin3;
int ValueAnalogPin4;
int ValueAnalogPin5;
int ValueAnalogPin6;
int ValueAnalogPin7;
int ValueAnalogPin8;



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

#if DEBUG_ENABLED
	vPrintReadSensorValues(*ptrInput);
#endif

}


 void vReadSensorValues_Module_1()
{
	 //Read the Button State (Pin 13 at GPIO C)
    ptrInput->Button = HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13);
	//read, and save the inputs here
    ptrInput->Transportation.start
    ptrInput->Transportation.newGlas
    ptrInput->Transportation.inModule2
	ptrInput->Transportation.inModule3
	ptrInput->Transportation.inModule4
	ptrInput->Transportation.inModule7
	
	return;
}

 void vReadSensorValues_Module_2()
{
	ptrInput->Gravity.weight_sensor = ValueAnalogPin4 / FACTOR_BITS_TO_WEIGHT; //A4
	ptrInput->Gravity.doors_open = HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_3); // PE3 digital
	ptrInput->Gravity.sensor_up = HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_8); // PF8
	ptrInput->Gravity.sensor_down = HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_7); // PF7
	ptrInput->Gravity.button_fill_in = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15); // PB15
	ptrInput->Gravity.position_tree = ValueAnalogPin6 / FACTOR_BITS_TO_ANGLE; //A6
	
	return;
}

 void vReadSensorValues_Module_3()
{
	 ptrInput->Pumping.doors_open = HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_2); // PE2
	 ptrInput->Pumping.valve_position = ValueAnalogPin2 / FACTOR_BITS_TO_POSITION; // A2
	 ptrInput->Pumping.weight_glass = ValueAnalogPin1 / FACTOR_BITS_TO_WEIGHT; // A1
	return;
}

 void vReadSensorValues_Module_4()
{
	 ptrInput->Pouring.doors_openn = HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_4); // PE4
	 ptrInput->Pouring.position_up = HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_6); // PE6
	 ptrInput->Pouring.position_down = HAL_GPIO_ReadPin(GPIOE,GPIO_PIN_5); // PE5
	 ptrInput->Pouring.weight = ValueAnalogPin3 / FACTOR_BITS_TO_WEIGHT; // A3
	return;
}

 void vReadSensorValues_Module_5()
{
	 ptrInput->Sensors.start_doors_open = HAL_GPIO_ReadPin(GPIOD,GPIO_PIN_0); // PD0
	 ptrInput->Sensors.start_light_barrier = HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_2); // PF2
	 ptrInput->Sensors.end_doors_open = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_9); // PB9
	 ptrInput->Sensors.end_light_barrier = HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_15); // PA15
	 ptrInput->Sensors.end_button_glass_present = HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_12); // PB12
	return;
}

 void vReadSensorValues_Module_6()
{
	 ptrInput->Ice.weight = ValueAnalogPin0 / FACTOR_BITS_TO_WEIGHT; // A0
	 ptrInput->Ice.doors_open = HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_1); // PF1
	 ptrInput->Ice.light_barrier_enough_cube_ice = HAL_GPIO_ReadPin(GPIOF,GPIO_PIN_0); // PF0
	 // TODO Hier fehlt die Pinbelegung der zweiten Lichtschranke -> nicht vermerkt in Sensors.txt
	 ptrInput->Ice.light_barrier_enough_crushed_ice = HAL_GPIO_ReadPin(GPIOX,GPIO_PIN_XX); // PXX
	 
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
 		
 		//A0: Channel 0
 	}
 	else if(port == GPIOF && pin == 5)
 	{
 		adc = hadc3;
 		
 		//F5: Channel 15
 		// Wie wird parallel ausgelesen? 
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


#if DEBUG_ENABLED
void vPrintReadSensorValues(InputValues_t input)
{
	printf("====================\n%s\n==============\n", "Sensor Values");
	printf("Gravity Weight: %g\nDoor Open: %d\nSensorUp: %d\nSensorDown: %d\nButton Fill: %d\nPosition Tree: %d\n",
			input->Gravity.weight_sensor, input->Gravity.doors_open, input->Gravity.sensor_up,
			input->Gravity.sensor_down, input->Gravity.button_fill_in);
	printf("Door Open: %d\nValve Position: %d\nWeightGlass: %g\n",
			input->Pumping.doors_open, input->Pumping.valve_position, input->Pumping.weight_glass);
	printf("Door Open: %d\nPosition Up: %d\nPosition Down: %d\nWeight: %g\n",
			input->Pouring.doors_open, input->Pouring.position_up, input->Pouring.position_down,
			input->Pouring.weight);
	printf("Start Door Open: %d\nStart Light Barrier: %d\nEnd Door Open: %d\nEnd Light Barrier: %d\nButton glass present: %d\n",
			input->Sensors.start_doors_open, input->Sensors.start_light_barrier, input->Sensors.end_doors_open,
			input->Sensors.end_light_barrier, input->Sensor.send_button_glass_present);

	printf("Weight: %g\nDoor open: %d\nLight Barrier enough cube ice: %d\nLight Barrier enough crushed ice: %d\n",
			input->Ice.weight, input->Ice.doors_open,
			input->Ice.light_barrier_enough_cube_ice, input->Ice.light_barrier_enough_crushed_ice);




}
#endif
