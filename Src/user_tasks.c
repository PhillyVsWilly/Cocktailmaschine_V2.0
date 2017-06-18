/*
 * threads.c
 *
 *  Created on: 18.06.2017
 *      Author: Philipp
 */

#include "user_tasks.h"
#include "main.h"

#include "stm32f3xx_hal.h"
#include "cmsis_os.h"

#include "FreeRTOS.h"
#include "task.h"

#include "Sensors.h"
#include "Evaluation.h"
#include "Actuators.h"



#define DEBUG_ENABLED TRUE
#include "Debug.h"

extern InputValues_t Input_Storage;
extern OutputValues_t Output_Storage;
extern SystemState_t System_State;


/***********************/
//Tasks
/***********************/
/* Once every 1000ms, the following Code is executed:
* Read all Sensor Values (in this case: The blue Button)
*Based on the Sensor Values: Calculate the output values
*Write the output values
*The starting time is stored in the beginning. vTaskDelayUntil() then waits until it is 1000ms after the stored time to make the
*task ready again.*/

void vMainCycle(void const* pvParameters)
{
	  /* -3- Toggle IOs in an infinite loop */
  while (1)
  {
		DPRINT_MESSAGE("Main Cycle Start\n");
		DPRINT_WARNING("Warning\n");

		//Store Tick Count
		TickType_t xLastWakeTime;
		xLastWakeTime = xTaskGetTickCount();

		//Sensoren lesen
		vReadSensorValues(&Input_Storage);

		//Berechnen
		vEvaluate(Input_Storage,&System_State, &Output_Storage);

		//Ausgabe schreiben
		vWriteActuatorValues(Output_Storage);

		//Delay until next Second
		vTaskDelayUntil(&xLastWakeTime, pdMS_TO_TICKS(1000));

		/************************************/
		//Delete later
		/*HAL_ADC_Start(&HS_ADC_Conf);
		  //Wait for the end of conversion
			if (HAL_ADC_PollForConversion(&HS_ADC_Conf, 10) != HAL_TIMEOUT)
		{
			// Get the converted value of regular channel
			int KeyConvertedValue = HAL_ADC_GetValue(&HS_ADC_Conf);
			DPRINT_MESSAGE("ADC Value: %d\r\n", KeyConvertedValue);
		}

		else
		{
			DPRINT_MESSAGE("ADC Timeout");
		}
*/




  }
}

