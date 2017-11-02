#ifndef EVALUATION_H_ 
#define EVALUATION_H_


#include "FIFO_Queue.h"
#include "Sensors.h"
#include "Actuators.h"
#include "FreeRTOS.h"
#include "Linked_List.h"


#define TRUE 1
#define FALSE 0

typedef char bool;

enum OperationMode_t{startup, normal, repair, stop};

ingredient_t IceIngArray[3];
ingredient_t GravIngArray[4];
ingredient_t PumpIngArray[7];
ingredient_t PourIngArray[3];

/*Type Definitions for System States*/
typedef struct{ 
	int operation_mode;
	int modules_finished[7];
	int safety_sensors[7];
	int WarnFlags[7];
	int ErrFlags[7];
	int CritFlags[7];
	int glassCount;
	} State_General_t;

typedef struct{
	State_General_t* ptrGeneralState;
	int state;
	bool stopForNewGlass; //True wenn das Band für x Sekunden warten soll damit ein neues Glas auf das Band gestellt werden kann
	bool transportCanStart; //bool set if all safety and service sensors are ok to signal Transport belt that it can start
	TickType_t startTicket;
	} Module_State_1_Transportation_t;

typedef struct{
	State_General_t* ptrGeneralState;
	int state;
	float drinkWeight;
	bool glassInStation; //Bool um festzustellen ob sich noch ein Glas in der Station befindet
	bool treeInPosition;
	linked_list drinkList;
    listNode *currentNode;
    TickType_t startTicket;
    bool platform_been_up;
    TickType_t plat_start_time;
    TickType_t plat_up_time;
	} Module_State_2_Gravity_t;

typedef struct{
	State_General_t* ptrGeneralState;
	int placeholder;
	int state;
	bool glassInStation;
	bool valveInTransit; //Bool that tracks if the valve is in transit
	bool valveInPosition;
	bool finished;//Getraenk fertig
	linked_list drinkList;
	listNode *currentNode;
	int drinkWeight;
	TickType_t startTicket;
	} Module_State_3_Pumping_t;

typedef struct{
	State_General_t *ptrGeneralState;
	int state;
	linked_list drinkList;
	listNode *currentNode;
	float drinkWeight;
	TickType_t startTicket;
	} Module_State_4_Pouring_t;

typedef struct{
	State_General_t* ptrGeneralState;
	int state;
	int state_end;

	bool finished_start;
	bool finished_end;
	TickType_t startTime;
	bool aux_btn_start_stop;
	bool aux_btn_cock_1;
	bool aux_btn_cock_2;
	bool aux_btn_cock_3;
	bool aux_end;

	} Module_State_5_Sensors_t;

typedef struct{
	State_General_t* ptrGeneralState;
	int state;
	} Module_State_6_Handling_t;

typedef struct{
	State_General_t* ptrGeneralState;
	int state;
	bool glassInStation;
	linked_list drinkList;
	listNode *currentNode;
	float drinkWeight;
	TickType_t startTicket;
	}Module_State_7_Ice_t;

typedef struct{
	State_General_t* ptrGeneralState;
	int state;
	linked_list drinkList;
	listNode *currentNode;
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

void addCocktail1();
void addCocktail2();
void addCocktail3();

#endif
