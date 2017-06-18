#ifndef SENSORS_H_ 
#define SENSORS_H_ 

#include <stdint.h>
#include "stm32f3xx_hal.h"

typedef unsigned char uint_8; 

typedef struct {
	int placeholder;
} Module_6_Handling_Input_t;

typedef struct {
	int safety[10]; //Contains the Sensor Inputs of the safety and service sensors
} Module_5_Sensors_Input_t;

typedef struct {
	uint8_t start;
	float desired_weight;
	uint_8 position_up;
	uint_8 position_down;
	float weight;
} Module_4_Pouring_Input_t;

typedef struct {
	int placeholder;
} Module_3_Pumping_Input_t;

typedef struct {
	uint_8 lightbarrier_stations[4];
} Module_1_Transportation_Input_t;

//Gravity_Input stores the Input values for the Gravity Station
typedef struct {
	int placeholder;
} Module_2_Gravity_Input_t;

typedef struct {
	int placeholder;
}Module_7_Ice_Input_t;

typedef struct {
	int placeholder;
} Module_x_Name_t;
//Define the Type where all input values will be stored later.
//Same idea as in Evaluation.h, a struct for each module to keep the oversight.
typedef struct {
	uint_8 Button;
	
	Module_1_Transportation_Input_t Transportation;
	Module_2_Gravity_Input_t Gravity;
	Module_3_Pumping_Input_t Pumping;
	Module_4_Pouring_Input_t Pouring;
	Module_5_Sensors_Input_t Sensors;
	Module_6_Handling_Input_t Handling;
	Module_7_Ice_Input_t Ice;
	Module_x_Name_t Module_x_Name;
	
		
} InputValues_t;



void vReadSensorValues(InputValues_t*);
void vReadSensorValues_Module_1(void);
void vReadSensorValues_Module_2(void);
void vReadSensorValues_Module_3(void);
void vReadSensorValues_Module_4(void);
void vReadSensorValues_Module_5(void);
void vReadSensorValues_Module_6(void);
void vReadSensorValues_Module_7(void);

void vReadAnalogPin(GPIO_TypeDef*, int);

#endif 

