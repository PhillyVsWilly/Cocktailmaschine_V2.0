#include "Module_7_Ice.h"
#include "Evaluation.h"
#include "Module_common.h"
#include "project_conf.h"


#include "Sensors.h"
#include "Evaluation.h"
#include "Actuators.h"

#undef DEBUG_ENABLED
#define DEBUG_ENABLED 1
#include "Debug.h"

/** @brief Initialisierung des Teilmoduls
 *
 * Hier werden die Anfangswerte der Zustäde und der Variablen eingestellt. Die Funktion wird nur ein einziges
 * Mal beim Systemstart ausgeführt
**/
void vInit_Module_7_Ice(Module_State_7_Ice_t* state, State_General_t* ptrGeneralState)
{
	//Nicht ädern, muss so sein!
	state->state = REFERENCE_ICE;
	state->ptrGeneralState = ptrGeneralState;
	list_new(state->drinkList);
	state->currentNode = NULL;
	state->glassInStation = FALSE;
	// Hier könen jetzt noch - falls nötig - Startwerte für die anderen Zustandsvariablen gegeben werden

	//Hier alle Einstellungen, die für den Einzelmodultest notwendig sind
#if (SINGLE_MODULE_TEST == MODULE_ID_ICE)

#endif
}

/** @brief Prüfe nach allgmeinen Fehlern
 *Diese Funktion muss bei jedem Zyklus ausgeführt werden. In ihr werden Systemzustäde überprüft, die unabhägig vom
 *Ablauf des Systems sind.
 *Bsp: Beim Transportmodul: Keine Hand in der Maschine
 *Beim Eis: Überlaufbecken nicht voll
 *Deshalb wird ihr auch nicht der Systemzustand übergeben
 **/
static int vCheckForGeneralErrors(InputValues_t input)
{

	if(input.Ice.doors_open == TRUE){
			ThrowError(7, DOOR_OPEN);
			return DOOR_OPEN;

		}




	return -1;
}

/** @brief Zentrale Ablaufsteuerung des Moduls
 *
 * Dieses Modul wird in jedem Zyklus aufgerufen und steuert das Modul
 * In ihr wird zuerst vCheckForErrors aufgerufen um zuerst nach allgemeinen Fehlern zu suchen
 * Danach wird in Abhägigkeit des Modulzustands state->state und des Betriebszustands *(state->ptrGeneralState) eine
 * oder mehrere bestimmte Aktionen ausgeführt und deren Verlauf überwacht
 * Hier können über ThrowError() auch weitere Fehler geworfen werden.
 * Soll der Modulzustand gewechselt werden, wird die vSwitchStateIce() Funktion benutzt. Diese prüft die generelle Zul舖sigkeit
 * (falls nötig) des Zustandswechsels und schreibt einen Debug-Print.
 **/
void vEvaluate_Module_7_Ice(InputValues_t input, Module_State_7_Ice_t* state, OutputValues_t* output)
{
	//listNode *ls_head = 0;[0] = Gewicht Eiswürfel, [1] = Gewicht crushed eis

	//ﾄndern des Status auf Basis des Gesamtmaschinenzustand
	if (state->ptrGeneralState->operation_mode == stop)
	{
		vSwitchStateIce(state, INACTIVE_ICE);
	}
	if(input.Ice.light_barrier_enough_crushed_ice == FALSE){
		vSwitchStateIce(state, INACTIVE_ICE);
	}
	if(input.Ice.light_barrier_enough_cube_ice == FALSE){
			vSwitchStateIce(state, INACTIVE_ICE);
		}

	//TODO Fehlendes Eis melden

	//Ausführen von Funktionen basierend auf dem Zustand
	switch (state->state){
	case INACTIVE_ICE:
		DPRINT_MESSAGE("I'm in State %d\n", state->state);
		output->Ice.motor = 0;

		if (!(state->ptrGeneralState->operation_mode
				== stop|| vCheckForGeneralErrors(input)!= -1 || input.Ice.light_barrier_enough_crushed_ice == FALSE || input.Ice.light_barrier_enough_cube_ice == FALSE)) {
			vSwitchStateIce(state, REFERENCE_ICE);
		}
		break;


		/*
		 * Das Eismodul besitzt nicht wirklich eine Referenzposition
		 * Daher entfällt hier die Referenzfahrt. Der Status wird sofort zu aktiv geswitcht
		 * solange kein genereller Fehler besteht
		 */
		case REFERENCE_ICE:
			//Do something
			if(vCheckForGeneralErrors(input)!= -1){
							vSwitchStateIce(state, INACTIVE_ICE);
						}
			DPRINT_MESSAGE("I'm in State %d\n", state->state);
			output->Ice.motor = 0;

			if(input.Ice.weight > 0){
				vSwitchStateIce(state,ACTIVE_ICE);
			}

			break;
		case ACTIVE_ICE:
			if(vCheckForGeneralErrors(input)!= -1){
							vSwitchStateIce(state, INACTIVE_ICE);
						}

			DPRINT_MESSAGE("I'm in State %d\n", state->state);
			list_head(state->drinkList,state->currentNode,FALSE);
			if(input.Ice.weight && state->currentNode->ingredient.amount != 0){
				vSwitchStateIce(state, FILL_ICE);
				state->glassInStation = TRUE;
				state->ptrGeneralState->modules_finished[6]=1;

			}
			break;

		case FILL_ICE:
			if(vCheckForGeneralErrors(input)!= -1){
							vSwitchStateIce(state, INACTIVE_ICE);
						}
			if(xTaskGetTickType() > (state->startTicket + 5000)){
				//TODO überprüfen ob 5000 Ticks korrekt sind
				//TODO Fehler werfen
			}
			state->ptrGeneralState->modules_finished[6]=0;
			DPRINT_MESSAGE("I'm in State %d\n", state->state);
			list_head(state->drinkList,state->currentNode,FALSE);
			if((input.Ice.weight < state->currentNode->ingredient.amount) && state->glassInStation && (state->currentNode->ingredient.bottleID == 1)){
				output->Ice.motor = 1;
				break;
			}
			if((input.Ice.weight < state->currentNode->ingredient.amount) && state->glassInStation && (state->currentNode->ingredient.bottleID == -1)){
				output->Ice.motor = -1;
				break;
			}
			else{
				list_head(state->drinkList,state->currentNode,TRUE);
				if(state->currentNode->ingredient.lastInstruction == TRUE){
				vSwitchStateIce(state,FINISHED_ICE);
				}
			}
			break;

		case FINISHED_ICE:
			if(vCheckForGeneralErrors(input)!= -1){
							vSwitchStatePump(state, INACTIVE_ICE);
						}

			DPRINT_MESSAGE("I'm in State %d\n", state->state);
			state->ptrGeneralState->modules_finished[6]=1;
			if(input.Ice.weight == 0){
				vSwitchStateIce(state, ACTIVE_ICE);
			}

			break;



		}
	
		return;

}

void vSwitchStateIce(Module_State_7_Ice_t* state, int state_new)
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




