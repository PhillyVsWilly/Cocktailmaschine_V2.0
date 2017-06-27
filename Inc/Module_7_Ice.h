#ifndef _MODULE_7_ICE_H_
#define _MODULE_7_ICE_H_

#include "Sensors.h"
#include "Evaluation.h"
#include "Actuators.h"

/**ÄNDERN: Zustande aus Ablaufdiagramm**/
//REFERENCE ist der Zustand der Referenzfahrt und darf nicht herausgenommen werden !!
enum statesIce {REFERENCE_ICE,ACTIVE_ICE,INACTIVE_ICE, FILL_ICE};


void vInit_Module_7_Ice(Module_State_7_Ice_t*, State_General_t*);
void vEvaluate_Module_7_Ice(InputValues_t, Module_State_7_Ice_t*, OutputValues_t*);
void vSwitchState(Module_State_7_Ice_t*, int);

#endif

