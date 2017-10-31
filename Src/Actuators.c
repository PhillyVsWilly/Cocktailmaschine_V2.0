#include "Actuators.h"
#include "stm32f7xx_hal.h"
#include "stm32f7xx_nucleo_144.h"
#include "math.h"

extern TIM_HandleTypeDef htim2, htim3, htim4;

//Rampen in V/s (Steuerspannung)
#define TRANSPORT_RAMP_UP 1
#define TRANSPORT_RAMP_DOWN -1
#define GRAVITY_PLATFORM_RAMP_UP 100
#define GRAVITY_PLATFORM_RAMP_DOWN -100
#define GRAVITY_TREE_RAMP_UP 1
#define GRAVITY_TREE_RAMP_DOWN -1
#define PUMPING_CHOOSER_RAMP_UP 30
#define PUMPING_CHOOSER_RAMP_DOWN -30
#define PUMPING_PUMP_RAMP_UP 100
#define PUMPING_PUMP_RAMP_DOWN -100
#define POURING_RAMP_UP 10
#define POURING_RAMP_DOWN -40
#define ICE_RAMP_UP 100
#define ICE_RAMP_DOWN -100

#define MOTOR_ID_TRANSPORT 0
#define MOTOR_ID_GRAVITY_PLATFORM 1
#define MOTOR_ID_GRAVITY_TREE 2
#define MOTOR_ID_PUMPING_CHOOSER 3
#define MOTOR_ID_PUMPING_PUMP 4
#define MOTOR_ID_POURING 5
#define MOTOR_ID_ICE 6

#define COUNT_MOTORS 7

TIM_OC_InitTypeDef sConfigPWM[COUNT_MOTORS];

void AuxPins(int*, int*, int);
int getPWMValue(int, int, int, int);
void setPWM(int, uint16_t);



void vInitActuatorValues(OutputValues_t* output_values)
{
}

void vWriteActuatorValues(OutputValues_t* output)
{
	vModule_1_ActuatorValues(&(output->Transport));
	vModule_2_ActuatorValues(&(output->Gravity));
	vModule_3_ActuatorValues(&(output->Pumping));
	vModule_4_ActuatorValues(&(output->Pouring));
	vModule_7_ActuatorValues(&(output->Ice));

}

void vModule_1_ActuatorValues(Transport_t* ptr_output)
{
	// Motor
	int a,b;
	if(ptr_output->startUp == 1 || ptr_output->windDown == 1)
	{
		ptr_output->motor= (ptr_output->windDown == 1) ? 0 : 100;
	}
	ptr_output->pwm = getPWMValue(ptr_output->motor, ptr_output->pwm, TRANSPORT_RAMP_UP, TRANSPORT_RAMP_DOWN);
	setPWM(MOTOR_ID_TRANSPORT, abs(ptr_output->pwm));
	AuxPins(&a, &b, ptr_output->pwm);
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_9, a);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, b);

}


void vModule_2_ActuatorValues(Gravity_t* ptr_output)
{
	// Motor Plattform
	int a,b;
	ptr_output->pwm_platform = getPWMValue(ptr_output->move_platform, ptr_output->pwm_platform, GRAVITY_PLATFORM_RAMP_UP, GRAVITY_PLATFORM_RAMP_DOWN);
	setPWM(MOTOR_ID_GRAVITY_PLATFORM, abs(ptr_output->pwm_platform));
	AuxPins(&a, &b, ptr_output->pwm_platform);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, a);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_15, b);

	// Motor Baum
	ptr_output->pwm_baum = getPWMValue(ptr_output->move_baum, ptr_output->pwm_baum, GRAVITY_TREE_RAMP_UP, GRAVITY_TREE_RAMP_DOWN);
	setPWM(MOTOR_ID_GRAVITY_TREE, abs(ptr_output->pwm_baum));
	AuxPins(&a, &b, ptr_output->pwm_baum);
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_14, a);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, b);

	printf("Motor Plattform %d\n", ptr_output->pwm_platform);
	printf("Motor Baum %d\n", a);

	/*a = ptr_output->move_baum >0;
	b = ptr_output->move_baum <0;
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_14, a);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, b);*/

}

void vModule_3_ActuatorValues(Pumping_t* ptr_output)
{
	int a,b;
	// Wahl
	ptr_output->pwm_choose = getPWMValue(ptr_output->choose_motor, ptr_output->pwm_choose, PUMPING_CHOOSER_RAMP_UP, PUMPING_CHOOSER_RAMP_DOWN);
	setPWM(MOTOR_ID_PUMPING_CHOOSER, abs(ptr_output->pwm_choose));
	AuxPins(&a, &b, ptr_output->pwm_choose);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, a);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, b);

	// Pumpe
	ptr_output->pwm_pump = getPWMValue(ptr_output->pump, ptr_output->pwm_pump, PUMPING_PUMP_RAMP_UP, PUMPING_PUMP_RAMP_DOWN);
	setPWM(MOTOR_ID_PUMPING_PUMP, abs(ptr_output->pwm_pump));
	AuxPins(&a, &b, ptr_output->pwm_pump);
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_15, a);
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_14, b);


}

void vModule_4_ActuatorValues(Pouring_t* ptr_output)
{
	int a,b;
	// Motor
	ptr_output->pwm = getPWMValue(ptr_output->motor, ptr_output->pwm, POURING_RAMP_UP, POURING_RAMP_DOWN);
	setPWM(MOTOR_ID_POURING, abs(ptr_output->pwm));
	AuxPins(&a, &b, ptr_output->pwm);
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_12, a);
	HAL_GPIO_WritePin(GPIOF, GPIO_PIN_13, b);
}

void vModule_7_ActuatorValues(Ice_t* ptr_output)
{
	int a,b;
	// Motor Baum
	ptr_output->pwm = getPWMValue(ptr_output->motor, ptr_output->pwm, ICE_RAMP_UP, ICE_RAMP_DOWN);
	setPWM(MOTOR_ID_ICE, abs(ptr_output->pwm));
	AuxPins(&a, &b, ptr_output->pwm);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, a);
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, b);
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
 *  @return Der PWM-Wert zwischen 0V und 3V als Zahl zwischen 0 (0V) und 100 (3V)
 **/
int getPWMValue(int desired, int last_pwm, int ascend, int descend)
{
	int diff = desired - last_pwm;
	if(diff < descend)
	{
		return last_pwm + descend;
	}
	else if(diff >= descend && diff <= ascend)
	{
		return desired;
	}
	else
	{
		return last_pwm + ascend;
	}
}

void setPWM(int MotorID, uint16_t value)
{

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
    	HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigPWM[MotorID], TIM_CHANNEL_2);
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

void AuxPins(int* a, int* b, int pwm)
{
	if(pwm <0)
	{
		*a = 0;
		*b = 1;
	}
	else if(pwm == 0)
	{
		*a = 0;
		*b = 0;
	}
	else
	{
		*a = 1;
		*b = 0;
	}
}

void StepperOut(int* a, int* b, int dir)
{
	if(dir>0)
	{
		if(*a == 0 && *b == 0)
		{
			*a = 1;
			*b = 0;
		} else if (*a == 1 && *b == 0)
		{
			*a = 1;
			*b = 1;
		} else if (*a == 1 && *b == 1)
		{
			*a = 0;
			*b = 1;
		} else {
			*a = 0;
			*b = 0;
		}
	} else if ( dir < 0) {
		if(*a == 0 && *b == 0)
		{
			*a = 0;
			*b = 1;
		} else if (*a == 0 && *b == 1)
		{
			*a = 1;
			*b = 1;
		} else if (*a == 1 && *b == 1)
		{
			*a = 1;
			*b = 0;
		} else {
			*a = 0;
			*b = 0;
		}
	}
}
