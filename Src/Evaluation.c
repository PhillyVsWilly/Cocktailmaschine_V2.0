#include "Evaluation.h"
#include "Module_common.h"
#include "Module_1_Transportation.h"
#include "Module_2_Gravity.h"
#include "Module_3_Pumping.h"
#include "Module_4_Pouring.h"
#include "Module_5_Sensors.h"
#include "Module_6_Handling.h"
#include "Module_7_Ice.h"

#undef DEBUG_ENABLED
#define DEBUG_ENABLED 0
#include "Debug.h"

static SystemState_t* ptr_System_State;

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

	ptr_System_State = pState;

	//TODO: Cocktail-Definitionen
		//Sex on the beach
		ingredient_t COCKTAIL1_1;
		COCKTAIL1_1.bottleID = 1;
		COCKTAIL1_1.amount = 60;
		COCKTAIL1_1.lastInstruction = TRUE;

		ingredient_t COCKTAIL1_2;
		COCKTAIL1_1.bottleID = 1;//Aprikose
		COCKTAIL1_1.amount = 1;
		COCKTAIL1_1.lastInstruction = FALSE;

		ingredient_t COCKTAIL1_3;
		COCKTAIL1_1.bottleID = 4;//wodka
		COCKTAIL1_1.amount = 1;
		COCKTAIL1_1.lastInstruction = TRUE;

		ingredient_t COCKTAIL1_4;
		COCKTAIL1_1.bottleID = 2;//Ananas
		COCKTAIL1_1.amount = 80;
		COCKTAIL1_1.lastInstruction = FALSE;

		ingredient_t COCKTAIL1_5;
		COCKTAIL1_1.bottleID = 4;//Cranberry
		COCKTAIL1_1.amount = 40;
		COCKTAIL1_1.lastInstruction = FALSE;

		ingredient_t COCKTAIL1_6;
		COCKTAIL1_1.bottleID = 7;//Orange
		COCKTAIL1_1.amount = 40;
		COCKTAIL1_1.lastInstruction = TRUE;

		ingredient_t COCKTAIL1_7;
		COCKTAIL1_1.bottleID = 0;//leeres Element
		COCKTAIL1_1.amount = 0;
		COCKTAIL1_1.lastInstruction = TRUE;


		//Hurricane
		ingredient_t COCKTAIL2_1;
		COCKTAIL2_1.bottleID = 1;
		COCKTAIL2_1.amount = 40;
		COCKTAIL2_1.lastInstruction = TRUE;

		ingredient_t COCKTAIL2_2;
		COCKTAIL2_1.bottleID = 3;//Rum
		COCKTAIL2_1.amount = 2;
		COCKTAIL2_1.lastInstruction = TRUE;

		ingredient_t COCKTAIL2_3;
		COCKTAIL2_1.bottleID = 1;//Zitrone
		COCKTAIL2_1.amount = 20;
		COCKTAIL2_1.lastInstruction = FALSE;

		ingredient_t COCKTAIL2_4;
		COCKTAIL2_1.bottleID = 7;//Orange
		COCKTAIL2_1.amount = 40;
		COCKTAIL2_1.lastInstruction = FALSE;

		ingredient_t COCKTAIL2_5;
		COCKTAIL2_1.bottleID = 3;//Maracuja
		COCKTAIL2_1.amount = 80;
		COCKTAIL2_1.lastInstruction = TRUE;

		ingredient_t COCKTAIL2_6;
		COCKTAIL2_1.bottleID = 1;//Grenadine
		COCKTAIL2_1.amount = 10;
		COCKTAIL2_1.lastInstruction = TRUE;

		//Tequilla Sunrise
		ingredient_t COCKTAIL3_1;
		COCKTAIL3_1.bottleID = 1;
		COCKTAIL3_1.amount = 60;
		COCKTAIL3_1.lastInstruction = TRUE;

		ingredient_t COCKTAIL3_2;
		COCKTAIL3_1.bottleID = 5;//Tequilla
		COCKTAIL3_1.amount = 1;
		COCKTAIL3_1.lastInstruction = TRUE;

		ingredient_t COCKTAIL3_3;
		COCKTAIL3_1.bottleID = 7;//Orange
		COCKTAIL3_1.amount = 100;
		COCKTAIL3_1.lastInstruction = TRUE;

		ingredient_t COCKTAIL3_4;
		COCKTAIL3_1.bottleID = 1;//Grenadine
		COCKTAIL3_1.amount = 15;
		COCKTAIL3_1.lastInstruction = TRUE;


		IceIngArray[0] = COCKTAIL1_1;
		IceIngArray[1] = COCKTAIL2_1;
		IceIngArray[2] = COCKTAIL3_1;

		GravIngArray[0] = COCKTAIL1_2;
		GravIngArray[1] = COCKTAIL1_3;
		GravIngArray[2] = COCKTAIL2_2;
		GravIngArray[3] = COCKTAIL3_2;

		PumpIngArray[0] = COCKTAIL1_4;
		PumpIngArray[1] = COCKTAIL1_5;
		PumpIngArray[2] = COCKTAIL1_6;
		PumpIngArray[3] = COCKTAIL2_3;
		PumpIngArray[4] = COCKTAIL2_4;
		PumpIngArray[5] = COCKTAIL2_5;
		PumpIngArray[6] = COCKTAIL3_3;

		PourIngArray[0] = COCKTAIL1_7;
		PourIngArray[1] = COCKTAIL2_6;
		PourIngArray[2] = COCKTAIL3_4;

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
		//vEvaluate_Module_2_Gravity(input, &(state->Gravity), output);
		//vEvaluate_Module_3_Pumping(input, &(state->Pumping), output);
		//vEvaluate_Module_4_Pouring(input, &(state->Pouring), output);
		vEvaluate_Module_5_Sensors(input, &(state->Sensors), output);
		//vEvaluate_Module_6_Handling(input,&(state->Handling), output);
		//vEvaluate_Module_7_Ice(input, &(state->Ice), output);
		
		DPRINT_MESSAGE("Glass Count: %d\n", state->General.glassCount);
		DPRINT_MESSAGE("Finished Sensors: %d\n", state->General.modules_finished[4]);
		DPRINT_MESSAGE("Finished Ice: %d\n", state->General.modules_finished[6]);
		DPRINT_MESSAGE("Finished Rondell: %d\n", state->General.modules_finished[1]);
		DPRINT_MESSAGE("Finished Pumping: %d\n", state->General.modules_finished[2]);
		DPRINT_MESSAGE("Finished Pouring: %d\n", state->General.modules_finished[3]);



}

//TODO Add ingredients of cocktail 1 to the stations
void addCocktail1() {
list_append (&ptr_System_State->Ice.drinkList, IceIngArray[0]);
list_append (&ptr_System_State->Gravity.drinkList, GravIngArray[0]);
list_append (&ptr_System_State->Gravity.drinkList, GravIngArray[1]);
list_append (&ptr_System_State->Pumping.drinkList, PumpIngArray[0]);
list_append (&ptr_System_State->Pumping.drinkList, PumpIngArray[1]);
list_append (&ptr_System_State->Pumping.drinkList, PumpIngArray[2]);
list_append (&ptr_System_State->Pouring.drinkList, PourIngArray[0]);
}

//TODO Add ingredients of cocktail 2 to the stations
void addCocktail2() {
	list_append (&ptr_System_State->Ice.drinkList, IceIngArray[1]);
	list_append (&ptr_System_State->Gravity.drinkList, GravIngArray[2]);
	list_append (&ptr_System_State->Pumping.drinkList, PumpIngArray[3]);
	list_append (&ptr_System_State->Pumping.drinkList, PumpIngArray[4]);
	list_append (&ptr_System_State->Pumping.drinkList, PumpIngArray[5]);
	list_append (&ptr_System_State->Pouring.drinkList, PourIngArray[1]);

}

//TODO Add ingredients of cocktail 3 to the stations
void addCocktail3() {
	list_append (&ptr_System_State->Ice.drinkList, IceIngArray[2]);
	list_append (&ptr_System_State->Gravity.drinkList, GravIngArray[3]);
	list_append (&ptr_System_State->Pumping.drinkList, PumpIngArray[6]);
	list_append (&ptr_System_State->Pouring.drinkList, PourIngArray[2]);
}

