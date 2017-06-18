#include "Module_4_Pouring.h"
#include "Evaluation.h"
#include "Debug.h"

void vEvaluate_Module_4_Pouring(InputValues_t input, Module_State_4_Pouring_t* state, OutputValues_t* output)
{
	/* TODO: Anpassen auf neues Template
	float weight_difference = input.Pouring.desired_weight - input.Pouring.weight ;
	if (input.Pouring.start == 0 && state->placeholder == 0){
		DPRINT_MESSAGE("Pouring : Not active and no start signal\n");
		return;
	}
	if(input.Pouring.position_up == 1 && weight_difference > 0){
		DPRINT_MESSAGE("Pouring : starting to lower the bottle\n");
		output->placeholder_pouring = 1;//tell the servo to run/ keep running
		state->placeholder = 1;
	}
	
	if(input.Pouring.position_down == 1){
		DPRINT_MESSAGE("Pouring : stopping to lower the bottle\n");
		output->placeholder_pouring = 0; //stop the servo
	}
	
	if ( weight_difference <=0 && input.Pouring.position_up == 0 ){
		DPRINT_MESSAGE("Pouring : glass filled, raising the bottle\n");
		output->placeholder_pouring = -1;//run the servo in the other direction
	}
	
	if(input.Pouring.position_up == 1 && weight_difference <=0){
		DPRINT_MESSAGE("Pouring : glass filled, bottle raised\n");
		output->placeholder_pouring = 0;//stop the servo
		state ->placeholder = 0;
	}
	
	return;
	*/
	
}
