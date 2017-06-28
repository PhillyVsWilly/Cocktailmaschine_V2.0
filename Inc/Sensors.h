#ifndef SENSORS_H_ 
#define SENSORS_H_ 

#include <stdint.h>
#include "stm32f7xx_hal.h"

#include "Linked_List.h"

typedef unsigned char uint_8; 
typedef char bool;


typedef struct {
	bool start;		//Band start
	bool newGlas; //TRUE if a new glas should be entered
	bool inModule2; //Glas in Modul 2
	bool inModule3; //Glas in Modul 3
	bool inModule4; //Glas in Modul 4
	bool inModule7; //Glas in Modul 7
} Module_1_Transportation_Input_t;

//Gravity_Input stores the Input values for the Gravity Station
typedef struct {
	float weight_sensor;
	bool doors_open;
	bool sensor_up;
	bool sensor_down;
	bool button_fill_in;
	int position_tree;
} Module_2_Gravity_Input_t;

typedef struct {
	bool doors_open;
	int valve_position;//1-8 oder -1 wenn sich das ventil zwischen 2 tastern befindet
	float weight_glass;
} Module_3_Pumping_Input_t;


typedef struct {
	bool doors_open;
	bool position_up;
	bool position_down;
	float weight;
} Module_4_Pouring_Input_t;

typedef struct {
	bool start_doors_open;
	bool start_light_barrier;
	bool end_doors_open;
	bool end_light_barrier;
	bool end_button_glass_present;
} Module_5_Sensors_Input_t;


typedef struct {
} Module_6_Handling_Input_t;

typedef struct {
	float weight;
	bool doors_open;
	bool light_barrier_enough_cube_ice;
	bool light_barrier_enough_crushed_ice;
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

