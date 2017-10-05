#include "Evaluation.h"
#include "Module_common.h"
#include "Module_1_Transportation.h"
#include "Module_2_Gravity.h"
#include "Module_3_Pumping.h"
#include "Module_4_Pouring.h"
#include "Module_5_Sensors.h"
#include "Module_6_Handling.h"
#include "Module_7_Ice.h"

void vInitModules(SystemState_t* pState)
{
	vInit_Module_common(pState);
	vInit_Module_1_Transport(&pState->Transportation,&pState->General);
	vInit_Module_2_Gravity(&pState->Gravity,&pState->General);
	vInit_Module_3_Pumping(&pState->Pumping,&pState->General);
	vInit_Module_4_Pouring(&pState->Pouring,&pState->General);
	vInit_Module_5_Sensors(&pState->Sensors,&pState->General);
	//vInit_Module_6_Handling(&pState->Handling,&pState->General); Not used
	vInit_Module_7_Ice(&pState->Ice,&pState->General);

}

void vEvaluate(InputValues_t input, SystemState_t* state, OutputValues_t* output)
{
		if (input.Button == 0)
		{
			output->LED2 = 1;
		} else {
			output->LED2 = 0;
		}
		
		vEvaluate_Module_1_Transportation(input, &(state->Transportation), output);
		vEvaluate_Module_2_Gravity(input, &(state->Gravity), output);
		vEvaluate_Module_3_Pumping(input, &(state->Pumping), output);
		vEvaluate_Module_4_Pouring(input, &(state->Pouring), output);
		vEvaluate_Module_5_Sensors(input, &(state->Sensors), output);
		vEvaluate_Module_6_Handling(input,&(state->Handling), output);
		vEvaluate_Module_7_Ice(input, &(state->Ice), output);
		
}
