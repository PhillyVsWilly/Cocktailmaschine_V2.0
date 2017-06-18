#ifndef EVALUATION_H_ 
#define EVALUATION_H_

#include "Sensors.h"
#include "Actuators.h"

#define TRUE 1
#define FALSE 0

typedef char bool;

enum operation_mode {startup, normal, repair, stop};

/*Type Definitions for System States*/
typedef struct{ 
	int operation_mode ;
	} State_General_t;

typedef struct{
	int placeholder;
	} Module_State_1_Transportation_t;

typedef struct{
	int placeholder;
	} Module_State_2_Gravity_t;

typedef struct{
	int placeholder;
	} Module_State_3_Pumping_t;

typedef struct{
	int placeholder;
	} Module_State_4_Pouring_t;

typedef struct{
	int placeholder;
	} Module_State_5_Sensors_t;

typedef struct{
	int placeholder;
	} Module_State_6_Handling_t;

typedef struct{
	int placeholder;
	}Module_State_7_Ice_t;

typedef struct{
	State_General_t* ptrGeneralState;
	int state;
	bool var_a;
	float var_b; 
	}Module_State_x_Name_t;
	

typedef struct{
	State_General_t General;
	Module_State_1_Transportation_t Transportation;
	Module_State_2_Gravity_t Gravity;
	Module_State_3_Pumping_t Pumping;
	Module_State_4_Pouring_t Pouring;
	Module_State_5_Sensors_t Sensors;
	Module_State_6_Handling_t Handling;
	Module_State_7_Ice_t Ice;
																									} SystemState_t;

/*End Type Definitions*/																									

																									
void vEvaluate(InputValues_t, SystemState_t*, OutputValues_t*);

#endif
