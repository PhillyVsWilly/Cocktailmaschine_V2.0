#ifndef _MODULE_4_POURING_H_
#define _MODULE_4_POURING_H_

#include "Sensors.h"
#include "Evaluation.h"
#include "Actuators.h"

/**ÄNDERN: Zustande aus Ablaufdiagramm**/
//REFERENCE ist der Zustand der Referenzfahrt und darf nicht herausgenommen werden !!
enum statesPour {REFERENCE_POUR, ACTIVE_POUR, GLASS_IN_STATION_POUR, POURING, INACTIVE_POUR};

void vInit_Module_4_Pouring(Module_State_4_Pouring_t*, State_General_t*);
void vEvaluate_Module_4_Pouring(InputValues_t, Module_State_4_Pouring_t*, OutputValues_t*);
void vSwitchStatePour(Module_State_4_Pouring_t*, int);

#endif
