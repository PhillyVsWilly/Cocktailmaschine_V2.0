#include "Module_5_Sensors.h"
#include "Module_common.h"
#include "FreeRTOS.h"

#include "Sensors.h"
#include "Evaluation.h"
#include "Actuators.h"
#include "project_conf.h"
#include "task.h"

#undef DEBUG_ENABLED
#define DEBUG_ENABLED 0
#include "Debug.h"

//Hier steht die Modulnummer. Sie wird im Code nicht hart gecodet, sondern nur hier eingetragen
//Im Code wird sie dann mit dem Namen MODULE_NUMBER verwendet
#define MODULE_NUMBER 5
#define START_END_GLASS_PRESENT_WAIT 250

void vSwitchState(Module_State_5_Sensors_t*, int );
void vSwitchStateSensEnd(Module_State_5_Sensors_t*, int );


/** @brief Initialisierung des Teilmoduls
 *
 * Hier werden die Anfangswerte der Zustände und der Variablen eingestellt. Die Funktion wird nur ein einziges
 * Mal beim Systemstart ausgeführt
**/
void vInit_Module_5_Sensors(Module_State_5_Sensors_t* state, State_General_t* ptrGeneralState)
{
	//Nicht ändern, muss so sein!
	state->state = IDLE_SENS;
	state->state_end = IDLE_SENS;
	state->ptrGeneralState = ptrGeneralState;

	state->startTime = 0;

	state->aux_btn_cock_1 =BTN_RELEASED;
	state->aux_btn_cock_2 =BTN_RELEASED;
	state->aux_btn_cock_3 =BTN_RELEASED;
	state->aux_btn_start_stop =BTN_RELEASED;
	state->aux_end = BTN_RELEASED;


	ptrGeneralState->WarnFlags[MODULE_NUMBER-1] = 0;
	ptrGeneralState->ErrFlags[MODULE_NUMBER-1] = 0;
	ptrGeneralState->CritFlags[MODULE_NUMBER-1] = 0;

	// Hier knen jetzt noch - falls nötig - Startwerte für die anderen Zustandsvariablen gegeben werden
}

/** @brief Prüfe nach allgmeinen Fehlern
 *Diese Funktion muss bei jedem Zyklus ausgeführt werden. In ihr werden Systemzustände überprüft, die unabhängig vom
 *Ablauf des Systems sind.
 *Bsp: Beim Transportmodul: Keine Hand in der Maschine
 *Beim Eis: ﾜberlaufbecken nicht voll
 *Deshalb wird ihr auch nicht der Systemzustand übergeben
 **/
void vCheckForGeneralErrorsSens(InputValues_t input)
{
	
	/*if(input.Sensors.end_doors_open || input.Sensors.start_doors_open)
	{
		ThrowError(MODULE_NUMBER, DOOR_OPEN);
	}*/
}
	
/** @brief Zentrale Ablaufsteuerung des Moduls
 *
 * Dieses Modul wird in jedem Zyklus aufgerufen und steuert das Modul
 * In ihr wird zuerst vCheckForErrors aufgerufen um zuerst nach allgemeinen Fehlern zu suchen
 * Danach wird in Abhängigkeit des Modulzustands state->state und des Betriebszustands *(state->ptrGeneralState) eine
 * oder mehrere bestimmte Aktionen ausgeführt und deren Verlauf überwacht
 * Hier knen über ThrowError() auch weitere Fehler geworfen werden.
 * Soll der Modulzustand gewechselt werden, wird die vSwitchState() Funktion benutzt. Diese prüft die generelle Zul舖sigkeit
 * (falls nötig) des Zustandswechsels und schreibt einen Debug-Print.
 **/
void vEvaluate_Module_5_Sensors(InputValues_t input, Module_State_5_Sensors_t* state, OutputValues_t* output)
{
	//Ändern des Status auf Basis des Gesamtmaschinenzustand
	if (state->ptrGeneralState->operation_mode == stop)
	{
		vSwitchStateSens(state, INACTIVE_SENS);
	}

	if(state->ptrGeneralState->ErrFlags[MODULE_NUMBER-1] != 0 || state->ptrGeneralState->CritFlags[MODULE_NUMBER-1] != 0)
	{
		vSwitchStateSens(state, INACTIVE_SENS);
	}

	//Flanken
	bool flank_start_stop = (input.Sensors.start_stop_button==BTN_PRESSED && state->aux_btn_start_stop==BTN_RELEASED);
	bool flank_cocktail_1 = (input.Sensors.button_cocktail_1==BTN_PRESSED && state->aux_btn_cock_1==BTN_RELEASED);
	bool flank_cocktail_2 = (input.Sensors.button_cocktail_2==BTN_PRESSED && state->aux_btn_cock_2==BTN_RELEASED);
	bool flank_cocktail_3 = (input.Sensors.button_cocktail_3==BTN_PRESSED && state->aux_btn_cock_3==BTN_RELEASED);
	bool flank_end = (input.Sensors.end_accept==BTN_PRESSED && state->aux_end==BTN_RELEASED);

	state->aux_btn_start_stop = input.Sensors.start_stop_button;
	state->aux_btn_cock_1 = input.Sensors.button_cocktail_1;
	state->aux_btn_cock_2 = input.Sensors.button_cocktail_2;
	state->aux_btn_cock_3 = input.Sensors.button_cocktail_3;
	state->aux_end = input.Sensors.end_accept;
	//Ende Flanken
	
	//Ausführen von Funktionen basierend auf dem Zustand
	DPRINT_MESSAGE("Sensors in State: %d\n", state->state);
	switch (state->state){
		case INACTIVE_SENS:
			if (!(state->ptrGeneralState->operation_mode == stop) &&
					!(state->ptrGeneralState->ErrFlags[MODULE_NUMBER-1] != 0 || state->ptrGeneralState->CritFlags[MODULE_NUMBER-1])) {
					vSwitchStateSens(state, IDLE_SENS);
			}
			break;
		case IDLE_SENS:
			//Flankenerkennung
			state->finished_start = 1;
			if(flank_start_stop)
			{
				vSwitchStateSens(state,CHOOSE_COCKTAIL_SENS);
			}
			output->Sensors.led_cocktail_chosen = 0;
			break;


		case ACTIVE_SENS:
			state->finished_start = 0;

			DPRINT_MESSAGE("Flank 0: %d\n" , flank_start_stop);
			DPRINT_MESSAGE("Flank 1: %d\n" , flank_cocktail_1);
			DPRINT_MESSAGE("Flank 2: %d\n" , flank_cocktail_2);
			DPRINT_MESSAGE("Flank 3: %d\n" , flank_cocktail_3);
			if(flank_cocktail_1	|| flank_cocktail_2 || flank_cocktail_3 || flank_start_stop)
			{
				vSwitchStateSens(state,CHOOSE_COCKTAIL_SENS);
			}
			break;

		case CHOOSE_COCKTAIL_SENS:
			output->Sensors.led_cocktail_chosen = 1;
			state->finished_start =0;
				if(flank_cocktail_1)
				{
					addCocktail1();
					state->ptrGeneralState->glassCount++;
					DPRINT_MESSAGE("Chosen Cocktail %d\n" ,1);
					vSwitchStateSens(state,IDLE_SENS);

				} else if (flank_cocktail_2){
					state->ptrGeneralState->glassCount++;
					addCocktail2();
					DPRINT_MESSAGE("Chosen Cocktail %d\n" ,2);
					vSwitchStateSens(state,IDLE_SENS);

				} else if (flank_cocktail_3){
					addCocktail3();
					state->ptrGeneralState->glassCount++;
					DPRINT_MESSAGE("Chosen Cocktail %d\n" ,3);
					vSwitchStateSens(state,IDLE_SENS);

				} else if (flank_start_stop){
					DPRINT_MESSAGE("None %d\n" , 0);
					vSwitchStateSens(state,IDLE_SENS);
				}

			break;
		default:
			break;
		}
	

	//Endmodul
	DPRINT_MESSAGE("End in State: %d\n", state->state_end);
	switch(state->state_end)
	{
	case IDLE_SENS:
		state->finished_end=TRUE;
		if(input.Sensors.end_button_glass_present == BTN_PRESSED)
		{
			vSwitchStateSensEnd(state, ACTIVE_SENS);
			state->finished_end = FALSE;
		}
		break;
	case ACTIVE_SENS:
		state->finished_end = FALSE;
		output->Sensors.led_cocktail_chosen = 1;
		if(input.Sensors.end_button_glass_present == BTN_RELEASED && flank_end)
		{
			vSwitchStateSensEnd(state, IDLE_SENS);
			if(state->ptrGeneralState->glassCount > 0)
			{
				state->ptrGeneralState->glassCount--;
				state->finished_end = TRUE;
			}
		}

	}

	DPRINT_MESSAGE("Finished Start %d\n", state->finished_start);
	DPRINT_MESSAGE("Finished End %d\n", state->finished_end);
	state->ptrGeneralState->modules_finished[MODULE_NUMBER-1] = state->finished_end && state->finished_start;

	return;



}

void vSwitchStateSens(Module_State_5_Sensors_t* state, int state_new)
{
	//Hier kommt alles rein, was bei jedem(!) Zustandswechsel passieren soll
	DPRINT_MESSAGE("Switching states from State %d to State %d\r\n", state->state, state_new);
	
	//Das hier sollte passieren, sonst wird der Zustand nicht gewechselt
	state->state = state_new;
	
	state->startTime = xTaskGetTickCount();

	return;
}

void vSwitchStateSensEnd(Module_State_5_Sensors_t* state, int state_new)
{
	{
		//Hier kommt alles rein, was bei jedem(!) Zustandswechsel passieren soll
		DPRINT_MESSAGE("Switching END states from State %d to State %d\r\n", state->state, state_new);

		//Das hier sollte passieren, sonst wird der Zustand nicht gewechselt
		state->state_end = state_new;

		//state->startTime = xTaskGetTickCount();

		return;
	}
}

/*****************************************************************/
/*****					HILFSFUNKTIONEN															******/
/* Hier komme alle Codeabschnitte hin, die mehr als 10-20 Zeilen */
/* besitzen, damit vEvaluate nicht zu aufgeblasen wird					 */
/*****************************************************************/



//vHilfsfuntion2() {  }



