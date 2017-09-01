#include "Actuators.h"
#include "stm32f7xx_hal.h"
#include "stm32f7xx_nucleo_144.h"

extern TIM_HandleTypeDef htim2, htim3, htim4;

//Rampen in V/s (Steuerspannung)
#define TRANSPORT_RAMP_UP 1
#define TRANSPORT_RAMP_DOWN 1
#define GRAVITY_PLATFORM_RAMP_UP 1
#define GRAVITY_PLATFORM_RAMP_DOWN 1
#define GRAVITY_TREE_RAMP_UP 1
#define GRAVITY_TREE_RAMP_DOWN 1
#define PUMPING_CHOOSER_RAMP_UP 1
#define PUMPING_CHOOSER_RAMP_DOWN 1
#define PUMPING_PUMP_RAMP_UP 1
#define PUMPING_PUMP_RAMP_DOWN 1
#define POURING_RAMP_UP 1
#define POURING_RAMP_DOWN 1
#define ICE_RAMP_UP 1
#define ICE_RAMP_DOWN 1

#define MOTOR_ID_TRANSPORT 0
#define MOTOR_ID_GRAVITY_PLATFORM 1
#define MOTOR_ID_GRAVITY_TREE 2
#define MOTOR_ID_PUMPING_CHOOSER 3
#define MOTOR_ID_PUMPING_PUMP 4
#define MOTOR_ID_POURING 5
#define MOTOR_ID_ICE 6

#define COUNT_MOTORS 7

TIM_OC_InitTypeDef sConfigPWM[COUNT_MOTORS];


/*
 * TODO Benedikt: Hier bitte deine Hilfsvariablen (speichern alter Werte ...)
 */


void vInitActuatorValues(OutputValues_t* output_values)
{
}

void vWriteActuatorValues(OutputValues_t output)
{
	output.Ice.motor = 1;
	/*if(output.Ice.motor != 0)
	{*/
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, GPIO_PIN_SET);
		/*
		setPWM(MOTOR_ID_ICE, 100);
	}
	else
	{
		setPWM(MOTOR_ID_ICE,0);
	}
	if (output.Ice.pwm = 100)
	{
		output.Ice.pwm = 0;
	}
	else
	{*/
		output.Ice.pwm = 50;
	//}
		setPWM(MOTOR_ID_ICE, output.Ice.pwm);

}

void vAssignMotorValues(OutputValues_t* output)
{

	/*
	 * TODO Benedikt berechne die richtige PWM Werte für alle 7 Motoren
	 * Du kannst die untenstehende Funktion getPWMValue() zur Hilfe nehmen
	 */

	//output->Transport.pwm = ...;
	//output->Gravity.pwm = ...;
	//usw.


	return;
}

/** Gibt den neuen Wert für die PWM zurück
 *
 *  Die Funktion prüft zuerst, ob der Sollwert des Motors und der Istwert der PWM übereinstimmen.
 *  Wenn nicht, wird der Wert der PWM mit der angegebenen Steigung in Richtung des Sollwerts verändert
 *
 *  @param desired Der aktuelle Sollwert des Motors in -1/0/1
 *  @param last_pwm Der vorhergehende PWM-Wert
 *  @param ascend Die Steigung der steigenden Flanke
 *  @param descend Die Steigung der fallenden Flanke
 *
 *  @return Der PWM-Wert zwischen 0V und 3V als Zahl zwischen 0 (0V) und 256 (3V)
 **/
int getPWMValue(desired, last_pwm, ascend, descend)
{
	int pwm;
	/*
	 * TODO
	 */
	return pwm;
}

void setPWM(int MotorID, uint16_t value)
{
    TIM_OC_InitTypeDef sConfigOC;

    sConfigPWM[MotorID].OCMode = TIM_OCMODE_PWM1;
    sConfigPWM[MotorID].Pulse = value;
    sConfigPWM[MotorID].OCPolarity = TIM_OCPOLARITY_HIGH;
    sConfigPWM[MotorID].OCFastMode = TIM_OCFAST_ENABLE;
    switch (MotorID)
    {
    case MOTOR_ID_TRANSPORT:
    	HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigPWM[MotorID], TIM_CHANNEL_1);
    	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
    	break;
    case MOTOR_ID_GRAVITY_PLATFORM:
    	HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigPWM[MotorID], TIM_CHANNEL_3);
    	HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
    	break;
    case MOTOR_ID_GRAVITY_TREE:
    	HAL_TIM_PWM_ConfigChannel(&htim3,&sConfigPWM[MotorID], TIM_CHANNEL_1);
    	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
    	break;
    case MOTOR_ID_POURING:
    	HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigPWM[MotorID], TIM_CHANNEL_2);
    	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);
    	break;
    case MOTOR_ID_PUMPING_CHOOSER:
    	HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigPWM[MotorID], TIM_CHANNEL_3);
    	HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3);
    	break;
    case MOTOR_ID_PUMPING_PUMP:
    	HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigPWM[MotorID], TIM_CHANNEL_1);
    	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
    	break;
    case MOTOR_ID_ICE:
    	HAL_TIM_PWM_ConfigChannel(&htim4, &sConfigPWM[MotorID], TIM_CHANNEL_2);
    	HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
    	break;
    default:
    	break;
    }
}
