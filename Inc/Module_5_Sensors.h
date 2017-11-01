#ifndef _MODULE_5_SENSORS_H_
#define _MODULE_5_SENSORS_H_

#include "Sensors.h"
#include "Evaluation.h"
#include "Actuators.h"

enum statesSens {INACTIVE_SENS, IDLE_SENS, ACTIVE_SENS,CHOOSE_COCKTAIL_SENS};

void vInit_Module_5_Sensors(Module_State_5_Sensors_t*, State_General_t*);
void vEvaluate_Module_5_Sensors(InputValues_t, Module_State_5_Sensors_t*, OutputValues_t*);
void vSwitchStateSens(Module_State_5_Sensors_t*, int);

bool safetyCheck(InputValues_t input);

#endif
