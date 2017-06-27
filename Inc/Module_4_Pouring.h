#ifndef _MODULE_4_POURING_H_
#define _MODULE_4_POURING_H_

#include "Sensors.h"
#include "Evaluation.h"
#include "Actuators.h"

/**ÄNDERN: Zustande aus Ablaufdiagramm**/
//REFERENCE ist der Zustand der Referenzfahrt und darf nicht herausgenommen werden !!
enum states {REFERENCE, ACTIVE, GLASS_IN_STATION, POURING, INACTIVE};

void vInit_Module_4_Pouring(Module_State_4_Pouring_t*, State_General_t*);
void vEvaluate_Module_4_Pouring(InputValues_t, SystemState_t*, OutputValues_t*);
void vSwitchState(Module_State_4_Pouring_t*, int);

#endif
