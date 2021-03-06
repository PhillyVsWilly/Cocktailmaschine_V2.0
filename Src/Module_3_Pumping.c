#include "Module_common.h"
#include "Module_3_Pumping.h"
#include "task.h"

#undef DEBUG_ENABLED
#define DEBUG_ENABLED 1
#include "Debug.h"

//Hier steht die Modulnummer. Sie wird im Code nicht hart gecodet, sondern nur hier eingetragen
//Im Code wird sie dann mit dem Namen MODULE_NUMBER verwendet
#define MODULE_NUMBER 3

#define EMPTY_WEIGHT 200

//TODO Pumping Timeouts einstellen
#define TIMEOUT_VALVE_ADJUSTING 5000
#define TIMEOUT_PUMP_ACTIVE 5000

#define MAX_CHOOSE_MOTOR_SPEED 25


/** @brief Initialisierung des Teilmoduls
 *
 * Hier werden die Anfangswerte der Zustände und der Variablen eingestellt. Die Funktion wird nur ein einziges
 * Mal beim Systemstart ausgeführt
**/
void vInit_Module_3_Pumping(Module_State_3_Pumping_t* state, State_General_t* ptrGeneralState)
{
	//Nicht ändern, muss so sein!
	state->state = REFERENCE_PUMP;
	state->ptrGeneralState = ptrGeneralState;
	list_new(&state->drinkList);

	// Hier knen jetzt noch - falls nötig - Startwerte für die anderen Zustandsvariablen gegeben werden
}

/** @brief Prüfe nach allgmeinen Fehlern
 *Diese Funktion muss bei jedem Zyklus ausgeführt werden. In ihr werden Systemzustände überprüft, die unabhängig vom
 *Ablauf des Systems sind.
 *Bsp: Beim Transportmodul: Keine Hand in der Maschine
 *Beim Eis: ﾜberlaufbecken nicht voll
 *Deshalb wird ihr auch nicht der Systemzustand übergeben
 **/
static int vCheckForGeneralErrors(InputValues_t input)
{
	/*if(input.Pumping.doors_open == TRUE){
		ThrowError(3, DOOR_OPEN);
		return DOOR_OPEN;
	}*/
	

	return -1;
}
	
/** @brief Zentrale Ablaufsteuerung des Moduls
 *
 * Dieses Modul wird in jedem Zyklus aufgerufen und steuert das Modul
 * In ihr wird zuerst vCheckForErrors aufgerufen um zuerst nach allgemeinen Fehlern zu suchen
 * Danach wird in Abhängigkeit des Modulzustands state->state und des Betriebszustands *(state->ptrGeneralState) eine
 * oder mehrere bestimmte Aktionen ausgeführt und deren Verlauf überwacht
 * Hier knen über ThrowError() auch weitere Fehler geworfen werden.
 * Soll der Modulzustand gewechselt werden, wird die vSwitchStatePump() Funktion benutzt. Diese prüft die generelle Zul舖sigkeit
 * (falls nötig) des Zustandswechsels und schreibt einen Debug-Print.
 **/
void vEvaluate_Module_3_Pumping(InputValues_t input, Module_State_3_Pumping_t* state, OutputValues_t* output)
{
	vCheckForGeneralErrors(input);

	//ﾄndern des Status
	if (state->ptrGeneralState->operation_mode == stop ||
			state->ptrGeneralState->ErrFlags[MODULE_NUMBER-1] ||
			state->ptrGeneralState->CritFlags[MODULE_NUMBER-1])
	{
		vSwitchStatePump(state, INACTIVE_PUMP);
	}

	DPRINT_MESSAGE("I'm in State %d\n", state->state);

	//Ausführen von Funktionen basierend auf dem Zustand
	switch (state->state){
		case INACTIVE_PUMP:
			state->ptrGeneralState->modules_finished[MODULE_NUMBER-1] = 0;

			//stops the Motors and
			output -> Pumping.pump = 0;
			output -> Pumping.choose_motor = 0;


			if(state->ptrGeneralState->operation_mode!= stop
					&& !state->ptrGeneralState->ErrFlags[MODULE_NUMBER-1] &&
					!state->ptrGeneralState->CritFlags[MODULE_NUMBER-1])
			{
				vSwitchStatePump(state, REFERENCE_PUMP);
			}
			break;


		case REFERENCE_PUMP:

			output -> Pumping.pump = 0;
			state->ptrGeneralState->modules_finished[MODULE_NUMBER-1] = 0;
			if(input.Pumping.valve_position == -1){
				output -> Pumping.choose_motor = -MAX_CHOOSE_MOTOR_SPEED;
			}
			else{
				output -> Pumping.choose_motor = 0;
				vSwitchStatePump(state, ACTIVE);
				state->valveInTransit = FALSE;

			}

			//DPRINT_MESSAGE("The pump output is %d\nThe choose valve output is %d\n",output ->Pumping.pump,output ->Pumping.choose_motor);

			break;
		case ACTIVE:
			state->ptrGeneralState->modules_finished[MODULE_NUMBER-1] = 1;
			state->glassInStation = input.Pumping.weight_glass > EMPTY_WEIGHT;
			output->Pumping.choose_motor = 0;

			if(state->drinkList.logicalLength){
				list_head(&state->drinkList, &state->currentNode, FALSE);

				if(state->currentNode->ingredient.amount > 0 && state->glassInStation){
					if(!state->valveInPosition)
					{
						vSwitchStatePump(state, VALVE_ADJUSTING);
						state->ptrGeneralState->modules_finished[3]=0;
					}
					else
					{
						vSwitchStatePump(state, VALVE_ADJUSTING);
						state->drinkWeight = input.Pumping.weight_glass;
					}


				}
			}

			break;

		case VALVE_ADJUSTING:
			state->ptrGeneralState->modules_finished[MODULE_NUMBER-1] = 0;

			if(xTaskGetTickCount() > (state->startTicket + TIMEOUT_VALVE_ADJUSTING)){
						//TODO Fehler werfen
					}

			if(input.Pumping.valve_position < state->currentNode->ingredient.bottleID && input.Pumping.valve_position != -1){
				output -> Pumping.choose_motor = MAX_CHOOSE_MOTOR_SPEED;
				state->valveInTransit = TRUE;
			}
			if(input.Pumping.valve_position > state->currentNode->ingredient.bottleID && input.Pumping.valve_position != -1){
				output -> Pumping.choose_motor = -MAX_CHOOSE_MOTOR_SPEED;
				state->valveInTransit = TRUE;
			}
			//Wenn wir noch keine Richtung haben, nehmen wir -1, ansonsten belassen wir es bei der Richtung
			if(input.Pumping.valve_position == -1 && output->Pumping.choose_motor != 1)
			{
				output->Pumping.choose_motor = -MAX_CHOOSE_MOTOR_SPEED;
				state->valveInTransit = TRUE;

			}

			if(input.Pumping.valve_position == state->currentNode->ingredient.bottleID ){
				output -> Pumping.choose_motor = 0;
				state->valveInPosition = TRUE;
				state->valveInTransit = FALSE;
				state->drinkWeight = input.Pumping.weight_glass;
				vSwitchStatePump(state, PUMP_ACTIVE);
			}

			break;
		case PUMP_ACTIVE:
			state->ptrGeneralState->modules_finished[MODULE_NUMBER-1] = 0;
			if(xTaskGetTickCount() > (state->startTicket + TIMEOUT_PUMP_ACTIVE)){
						//TODO Fehler werfen
					}



			if(input.Pumping.weight_glass < state->currentNode->ingredient.amount + state->drinkWeight){
				output -> Pumping.pump = 40;
			}
			else{
				output -> Pumping.pump = 0;
				bool cont = !state->currentNode->ingredient.lastInstruction;
				list_head(&state->drinkList, &state->currentNode, TRUE);
				state->valveInPosition = FALSE;
				if (cont){
					vSwitchStatePump(state,ACTIVE);
				}
				else{
					vSwitchStatePump(state,FILLED_GLASS_PUMP);
				}
			}
			break;
		case FILLED_GLASS_PUMP:

			state->ptrGeneralState->modules_finished[MODULE_NUMBER-1]=1;
			if(input.Pumping.weight_glass < EMPTY_WEIGHT){
				vSwitchStatePump(state,ACTIVE);
			}
			break;


		default:
			break;
		}
	
		return;

}

void vSwitchStatePump(Module_State_3_Pumping_t* state, int state_new)
{
	//Hier kommt alles rein, was bei jedem(!) Zustandswechsel passieren soll
	DPRINT_MESSAGE("Switching states from State %d to State %d\r\n", state->state, state_new);
	
	//Das hier sollte passieren, sonst wird der Zustand nicht gewechselt
	state->state = state_new;
	state->startTicket = xTaskGetTickCount();
	return;
}

/*****************************************************************/
/*****					HILFSFUNKTIONEN															******/
/* Hier komme alle Codeabschnitte hin, die mehr als 10-20 Zeilen */
/* besitzen, damit vEvaluate nicht zu aufgeblasen wird					 */
/*****************************************************************/

//vHilfsfuntion1() {  }
//vHilfsfuntion2() {  }


