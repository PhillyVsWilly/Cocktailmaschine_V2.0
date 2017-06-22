#ifndef _TEMPLATE_MODULE_X_NAME_H_
#define _TEMPLATE_MODULE_X_NAME_H_

#include "Evaluation.h"

/**ÄNDERN: Zustande aus Ablaufdiagramm**/
//REFERENCE ist der Zustand der Referenzfahrt und darf nicht herausgenommen werden !!
enum states {REFERENCE, STATE1, STATE2, STATE3};

void vInit_Module_x_Name(Module_State_x_Name_t*, State_General_t*);
void vEvaluate_Module_x_Name(InputValues_t, Module_State_x_Name_t*, OutputValues_t*);
void vSwitchState(Module_State_x_Name_t*, int);



#endif
