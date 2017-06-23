#ifndef _MODULE_2_GRAVITY_H_
#define _MODULE_2_GRAVITY_H_

#include "Sensors.h"
#include "Evaluation.h"
#include "Actuators.h"

/**�NDERN: Zustande aus Ablaufdiagramm**/
//REFERENCE ist der Zustand der Referenzfahrt und darf nicht herausgenommen werden !!
enum states {REFERENCE, MOVING_TREE, FILLED_GLASS, GLASS_IN_STATION, MOVE_PLATTFORM, INACTIVE};

void vInit_Module_2_Gravity(Module_State_2_Gravity_t*, State_General_t*);
void vEvaluate_Module_2_Gravity(InputValues_t, SystemState_t*, OutputValues_t*);
void vSwitchState(Module_State_2_Gravity_t*, int);
void vFillDrinkList(listNode *node);
void vShuffleDrinks(uint_8 list[][]);

#endif
