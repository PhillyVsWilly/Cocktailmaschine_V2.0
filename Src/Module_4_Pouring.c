#include "Module_4_Pouring.h"
#include "Evaluation.h"
#include "Debug.h"
#include "Module_common.h"
#include "task.h"
#include "project_conf.h"


#undef DEBUG_ENABLED
#define DEBUG_ENABLED 1
#include "Debug.h"

//Hier steht die Modulnummer. Sie wird im Code nicht hart gecodet, sondern nur hier eingetragen
//Im Code wird sie dann mit dem Namen MODULE_NUMBER verwendet
#define MODULE_NUMBER 4
#define EMPTY_WEIGHT_THRESH 100
#define GLASS_WEIGHT 100
#define FILL_ERROR 5 //Fluid amount that gets filled in when the bottle is moved in the up position


/** @brief Initialisierung des Teilmoduls
 *
 * Hier werden die Anfangswerte der Zustände und der Variablen eingestellt. Die Funktion wird nur ein einziges
 * Mal beim Systemstart ausgeführt
**/
void vInit_Module_4_Pouring(Module_State_4_Pouring_t* state, State_General_t* ptrGeneralState)
{
	//Nicht dern, muss so sein!
	state->state = REFERENCE_POUR;
	state->ptrGeneralState = ptrGeneralState;
	list_new(&state->drinkList);
	ptrGeneralState->CritFlags[MODULE_NUMBER -1] = 0;
	ptrGeneralState->ErrFlags[MODULE_NUMBER -1] = 0;
	ptrGeneralState->WarnFlags[MODULE_NUMBER -1] = 0;

	// Hier knen jetzt noch - falls nig - Startwerte für die anderen Zustandsvariablen gegeben werden
}

/** @brief Prüfe nach allgmeinen Fehlern
 *Diese Funktion muss bei jedem Zyklus ausgeführt werden. In ihr werden Systemzustände überprüft, die unabhängig vom
 *Ablauf des Systems sind.
 *Bsp: Beim Transportmodul: Keine Hand in der Maschine
 *Beim Eis: ﾜberlaufbecken nicht voll
 *Deshalb wird ihr auch nicht der Systemzustand übergeben
 **/
static void vCheckForGeneralErrors(InputValues_t input)
{
	if (input.Pouring.doors_open == SWITCH_OPEN)
	{
		ThrowError(MODULE_NUMBER,DOOR_OPEN);
	}

}

/** @brief Zentrale Ablaufsteuerung des Moduls
 *
 * Dieses Modul wird in jedem Zyklus aufgerufen und steuert das Modul
 * In ihr wird zuerst vCheckForErrors aufgerufen um zuerst nach allgemeinen Fehlern zu suchen
 * Danach wird in Abhängigkeit des Modulzustands state->state und des Betriebszustands *(state->ptrGeneralState) eine
 * oder mehrere bestimmte Aktionen ausgeführt und deren Verlauf überwacht
 * Hier knen über ThrowError() auch weitere Fehler geworfen werden.
 * Soll der Modulzustand gewechselt werden, wird die vSwitchStatePour() Funktion benutzt. Diese prüft die generelle Zul舖sigkeit
 * (falls nig) des Zustandswechsels und schreibt einen Debug-Print.
 **/
void vEvaluate_Module_4_Pouring(InputValues_t input, Module_State_4_Pouring_t* state, OutputValues_t* output)
{
	//ﾄndern des Status auf Basis des Gesamtmaschinenzustand
	if (state->ptrGeneralState->operation_mode == stop)
	{
		vSwitchStatePour(state, INACTIVE_POUR);
	}

	//Eingang überprüfen
	vCheckForGeneralErrors(input);
	if (state->ptrGeneralState->ErrFlags[MODULE_NUMBER-1] || state->ptrGeneralState->CritFlags[MODULE_NUMBER-1]) {
		vSwitchStatePour(state, INACTIVE_POUR);
	}


	DPRINT_MESSAGE("Pouring in State %d\n", state->state);
	//Ausführen von Funktionen basierend auf dem Zustand
	switch (state->state){

		case REFERENCE_POUR:
			state->ptrGeneralState->modules_finished[MODULE_NUMBER-1] = 0;
			//Fahre den Motor so lange hoch, bis der Positionstaster für die obere Endlage gedrückt wurde
			if (!(input.Pouring.position_up == BTN_PRESSED)) {
				output->Pouring.motor = 40;
			} else if (input.Pouring.position_up == BTN_PRESSED) {
				output->Pouring.motor = 0;
				vSwitchStatePour(state, ACTIVE_POUR);
			}
			break;
		case ACTIVE_POUR:
			//Do something
			state->ptrGeneralState->modules_finished[MODULE_NUMBER - 1] = 1;
			list_head(&state->drinkList, &state->currentNode, FALSE);

			if(state->currentNode != NULL)
			{
				if (input.Pouring.weight > EMPTY_WEIGHT_THRESH && state->currentNode->ingredient.amount > 0) {
					state->drinkWeight = input.Pouring.weight;
					vSwitchStatePour(state, POURING);
				}
			}
			break;
		case INACTIVE_POUR:
			//Do something
			state->ptrGeneralState->modules_finished[MODULE_NUMBER - 1] = 0;
			output->Pouring.motor = 0;
			if (state->ptrGeneralState->operation_mode != stop
					&& state->ptrGeneralState->ErrFlags[MODULE_NUMBER-1] == 0
					&& state->ptrGeneralState->ErrFlags[MODULE_NUMBER-1] == 0) {
				vSwitchStatePour(state, REFERENCE_POUR);
			}
			break;
		case POURING:
			state->ptrGeneralState->modules_finished[MODULE_NUMBER-1] = 0;
			if (input.Pouring.position_up == BTN_PRESSED && input.Pouring.weight < state->drinkWeight + state->currentNode->ingredient.amount + FILL_ERROR) {
				output->Pouring.motor = -40;
			}
			if (input.Pouring.position_down == BTN_PRESSED) {
				output->Pouring.motor = 0;
			}
			if (input.Pouring.weight >= state->drinkWeight + state->currentNode->ingredient.amount + FILL_ERROR && input.Pouring.position_up != BTN_PRESSED) {
				output->Pouring.motor = 40;
			}
			if (input.Pouring.weight >= state->drinkWeight + state->currentNode->ingredient.amount + FILL_ERROR && input.Pouring.position_up == BTN_PRESSED) {
				state->ptrGeneralState->modules_finished[MODULE_NUMBER - 1] = 1;
				output->Pouring.motor = 0;
				list_head(&state->drinkList, &state->currentNode, TRUE);
				vSwitchStatePour(state, FINISHED_POUR);
				break;
			}
			break;
		case FINISHED_POUR:
			state->ptrGeneralState->modules_finished[MODULE_NUMBER-1] = 1;
			output->Pouring.motor=0;
			if(input.Pouring.weight < EMPTY_WEIGHT_THRESH - 10)
			{
				vSwitchStatePour(state, ACTIVE_POUR);
			}
		default:
			break;
		}

		return;

}

void vSwitchStatePour(Module_State_4_Pouring_t* state, int state_new)
{
	//Hier kommt alles rein, was bei jedem(!) Zustandswechsel passieren soll
	DPRINT_MESSAGE("Switching states from State %d to State %d\r\n", state->state, state_new);

	state->startTicket = xTaskGetTickCount();

	//Das hier sollte passieren, sonst wird der Zustand nicht gewechselt
	state->state = state_new;

	return;
}

/*****************************************************************/
/*****					HILFSFUNKTIONEN															******/
/* Hier komme alle Codeabschnitte hin, die mehr als 10-20 Zeilen */
/* besitzen, damit vEvaluate nicht zu aufgeblasen wird					 */
/*****************************************************************/

//vHilfsfuntion1() {  }
//vHilfsfuntion2() {  }


