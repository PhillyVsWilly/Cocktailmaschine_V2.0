#ifndef _MODULE_1_TRANSPORTATION_H_
#define _MODULE_1_TRANSPORTATION_H_

#include "Sensors.h"
#include "Evaluation.h"
#include "Actuators.h"

/**�NDERN: Zustande aus Ablaufdiagramm**/
//REFERENCE ist der Zustand der Referenzfahrt und darf nicht herausgenommen werden !!
enum states {REFERENCE, ACTIVE, WAITING, INACTIVE};

void vInit_Module_1_Transport(Module_State_1_Transport_t*, State_General_t*);
void vEvaluate_Module_1_Transportation(InputValues_t, SystemState_t*, OutputValues_t*);
void vSwitchState(Module_State_1_Transport_t*, int);


#endif
