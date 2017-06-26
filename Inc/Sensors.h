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
	bool inModule5; //Glas in Modul 5
} Module_1_Transportation_Input_t;

//Gravity_Input stores the Input values for the Gravity Station
typedef struct {
	bool start;
	int position_tree; // English word for this? Well, tree :D
	float weight_sensor;
    linked_list *drinkList;
	bool sensor_up;
	bool sensor_down;
} Module_2_Gravity_Input_t;

typedef struct {
	bool start;
	int valve_position;//1-8 oder -1 wenn sich das ventil zwischen 2 tastern befindet
	float weight_glass;
} Module_3_Pumping_Input_t;

typedef struct {
	uint8_t start;
	uint_8 position_up;
	uint_8 position_down;
	linked_list *drinkList;
	float weight;
} Module_4_Pouring_Input_t;

typedef struct {
	int safety_push_buttons[10]; //Contains the Sensor Inputs of the safety and service sensors
	bool right_glass;
	int modules_finished[7]; // Contains Modul's signals: Moved to State, because the modules can't fill this in here
} Module_5_Sensors_Input_t;

// Welche Infos kommen? -> Muss noch vervollständigt werden.
typedef struct {
	uint8_t flaschen_ID;
	float desired_weight;
} Module_6_Handling_Input_t;

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

