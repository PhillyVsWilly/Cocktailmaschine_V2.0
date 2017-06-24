#ifndef _MODULE_3_PUMPING_H_
#define _MODULE_3_PUMPING_H_

#include "Sensors.h"
#include "Evaluation.h"
#include "Actuators.h"

enum states {REFERENCE_PUMP ,ACTIVE, VALVE_ADJUSTING, PUMP_ACTIVE, FILLED_GLASS, INACTIVE_PUMP};

void vEvaluate_Module_3_Pumping(Module_3_Pumping_Input_t, SystemState_t*, OutputValues_t*);

void vInit_Module_3_Pumping(Module_State_3_Pumping_t*, State_General_t*);

void vEvaluate_3_Pumping(InputValues_t, Module_State_3_Pumping_t*, OutputValues_t*);

void vSwitchState(Module_State_3_Pumping_t*, int);

#endif
