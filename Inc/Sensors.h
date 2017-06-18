#ifndef SENSORS_H_ 
#define SENSORS_H_ 

#include <stdint.h>

typedef unsigned char uint_8; 

typedef struct {
	int placeholder;
} Handling_Input_t;

typedef struct {
	int safety[10]; //Contains the Sensor Inputs of the safety and service sensors
} Sensors_Input_t;

typedef struct {
	uint8_t start;
	float desired_weight;
	uint_8 position_up;
	uint_8 position_down;
	float weight;
} Pouring_Input_t;

typedef struct {
	int placeholder;
} Pumping_Input_t;

typedef struct {
	uint_8 lightbarrier_stations[4];
} Transportation_Input_t;

//Gravity_Input stores the Input values for the Gravity Station
typedef struct {
	int placeholder;
} Gravity_Input_t;

typedef struct {
	int placeholder;
}Ice_Input_t;

typedef struct {
	int placeholder;
} Module_x_Name_t;
//Define the Type where all input values will be stored later.
//Same idea as in Evaluation.h, a struct for each modul to keep the oversight.
typedef struct {
	uint_8 Button;
	
	Gravity_Input_t Gravity; 
	Transportation_Input_t Transportation;
	Pumping_Input_t Pumping;
	Pouring_Input_t Pouring;
	Sensors_Input_t Sensors;
	Handling_Input_t Handling;
	Ice_Input_t Ice;
	Module_x_Name_t Module_x_Name;
	
		
} InputValues_t;



void vReadSensorValues(InputValues_t*);

#endif 

