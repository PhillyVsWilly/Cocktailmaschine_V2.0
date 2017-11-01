#include "Module_7_Ice.h"
#include "Evaluation.h"
#include "Module_common.h"
#include "project_conf.h"
#include "task.h"

#include "stm32f7xx_hal.h"


#include "Sensors.h"
#include "Evaluation.h"
#include "Actuators.h"


#undef DEBUG_ENABLED
#define DEBUG_ENABLED 1
#include "Debug.h"

#define MODULE_NUMBER 7

/** @brief Initialisierung des Teilmoduls
 *
 * Hier werden die Anfangswerte der Zustäde und der Variablen eingestellt. Die Funktion wird nur ein einziges
 * Mal beim Systemstart ausgeführt
**/
void vInit_Module_7_Ice(Module_State_7_Ice_t* state, State_General_t* ptrGeneralState)
{
	//Nicht ädern, muss so sein!
	state->state = ACTIVE_ICE;
	state->ptrGeneralState = ptrGeneralState;
	list_new(&state->drinkList);
	state->glassInStation = FALSE;
	ptrGeneralState->CritFlags[MODULE_NUMBER -1] = 0;
	ptrGeneralState->ErrFlags[MODULE_NUMBER -1] = 0;
	ptrGeneralState->WarnFlags[MODULE_NUMBER -1] = 0;
	// Hier könen jetzt noch - falls nötig - Startwerte für die anderen Zustandsvariablen gegeben werden

}

/** @brief Prüfe nach allgmeinen Fehlern
 *Diese Funktion muss bei jedem Zyklus ausgeführt werden. In ihr werden Systemzustäde überprüft, die unabhägig vom
 *Ablauf des Systems sind.
 *Bsp: Beim Transportmodul: Keine Hand in der Maschine
 *Beim Eis: Überlaufbecken nicht voll
 *Deshalb wird ihr auch nicht der Systemzustand übergeben
 **/
static void vCheckForGeneralErrors(InputValues_t input)
{

	if(input.Ice.doors_open == FALSE){
			ThrowError(MODULE_NUMBER, DOOR_OPEN);
		}

	//Warnungen; hier wird kein Fehler geworfen
	//Der Zustand muss nicht in Fehler wechseln
	if(input.Ice.light_barrier_enough_crushed_ice == FALSE){
		ThrowWarning(MODULE_NUMBER, ICE_1_EMPTY);
	} else {
		ClearWarning(MODULE_NUMBER, ICE_1_EMPTY);
	}

	if(input.Ice.light_barrier_enough_cube_ice == FALSE){
			ThrowWarning(MODULE_NUMBER, ICE_2_EMPTY);
		} else {
			ClearWarning(MODULE_NUMBER, ICE_2_EMPTY);
		}

	//TODO Timeout: Wenn kein Gewicht hinzu kommt und eine Warnung wegen kein Eis kommt: Error

}

/** @brief Zentrale Ablaufsteuerung des Moduls
 *
 * Dieses Modul wird in jedem Zyklus aufgerufen und steuert das Modul
 * In ihr wird zuerst vCheckForErrors aufgerufen um zuerst nach allgemeinen Fehlern zu suchen
 * Danach wird in Abhägigkeit des Modulzustands state->state und des Betriebszustands *(state->ptrGeneralState) eine
 * oder mehrere bestimmte Aktionen ausgeführt und deren Verlauf überwacht
 * Hier können über ThrowError(), ThrowErrorCritical() oder ThrowWarning() auch weitere Fehler geworfen werden.
 * Soll der Modulzustand gewechselt werden, wird die vSwitchStateIce() Funktion benutzt. Diese prüft die generelle Zul舖sigkeit
 * (falls nötig) des Zustandswechsels und schreibt einen Debug-Print.
 **/
void vEvaluate_Module_7_Ice(InputValues_t input, Module_State_7_Ice_t* state, OutputValues_t* output)
{



	//Ändern des Status auf Basis des Gesamtmaschinenzustand
	if (state->ptrGeneralState->operation_mode == stop)
	{
		vSwitchStateIce(state, INACTIVE_ICE);
	}

	//Ausführen von Funktionen basierend auf dem Zustand
	vCheckForGeneralErrors(input);
	if(state->ptrGeneralState->ErrFlags[MODULE_NUMBER-1] != 0 ||
			state->ptrGeneralState->CritFlags[MODULE_NUMBER-1] != 0)
	{
		vSwitchStateIce(state, INACTIVE_ICE);
	}

	DPRINT_MESSAGE("Warning Flags: %d\n", state->ptrGeneralState->WarnFlags[MODULE_NUMBER-1]);

	switch (state->state){

	/**
	 * Das Eismodul ist nicht bereit.
	 * Das ist der Fall, wenn das Modul einen Fehler oder eine Warnung hat.
	 * Außerdem ist es inaktiv, wenn der Systemzustand auf "stop" steht
	 */
	case INACTIVE_ICE:
		state->ptrGeneralState->modules_finished[MODULE_NUMBER-1] = 0;
		DPRINT_MESSAGE("Ice in State %d\n", state->state);
		output->Ice.motor = 0;

		if (!(state->ptrGeneralState->operation_mode == stop||
				state->ptrGeneralState->ErrFlags[MODULE_NUMBER-1] != 0 ||
				state->ptrGeneralState->CritFlags[MODULE_NUMBER-1] != 0 ||
				state->ptrGeneralState->WarnFlags[MODULE_NUMBER-1] != 0)) {
			vSwitchStateIce(state, ACTIVE_ICE);
		}
		break;

	/**
	 * Das Eismodul ist bereit für neue Befehle
	 */
	case ACTIVE_ICE:
		state->ptrGeneralState->modules_finished[MODULE_NUMBER-1] = 1;
		DPRINT_MESSAGE("Ice in State %d\n", state->state);
		list_head(&state->drinkList,&state->currentNode,FALSE);
		state->ptrGeneralState->modules_finished[6]=1;

		if(state->drinkList.logicalLength > 0)
		{
			if(state->ptrGeneralState->WarnFlags[MODULE_NUMBER-1] != 0)
			{
				vSwitchStateIce(state, INACTIVE_ICE);
			} else	if(input.Ice.weight > GLASS_WEIGHT*0.9 && state->currentNode->ingredient.amount != 0){
				vSwitchStateIce(state, FILL_ICE);
				state->glassInStation = TRUE;
			}

		}
		break;

	/**
	 * Das Modul arbeitet und füllt Eis in das Glas
	 */
	case FILL_ICE:
		state->ptrGeneralState->modules_finished[MODULE_NUMBER-1] = 0;
		if(xTaskGetTickCount() > (state->startTicket + TIMEOUT_ICE_FILL)){
			//TODO Fehler werfen
		}
		state->ptrGeneralState->modules_finished[6]=0;
		DPRINT_MESSAGE("Ice in State %d\n", state->state);
		list_head(&state->drinkList,&state->currentNode,FALSE);

		//Beginne, Eis zu fördern
		if((input.Ice.weight - GLASS_WEIGHT < state->currentNode->ingredient.amount) && state->glassInStation && (state->currentNode->ingredient.bottleID == 0)){
			output->Ice.motor = 90;
			break;
		}
		else if((input.Ice.weight - GLASS_WEIGHT < state->currentNode->ingredient.amount) && state->glassInStation && (state->currentNode->ingredient.bottleID == 1)){
			output->Ice.motor = -90;
			break;
		}
		else{
			//Stoppe Motor
			output->Ice.motor = 0;
			if(state->currentNode->ingredient.lastInstruction == TRUE){
				list_head(&state->drinkList, &state->currentNode, TRUE);
				vSwitchStateIce(state,FINISHED_ICE);
			}
			else
			{
				list_head(&state->drinkList, &state->currentNode, TRUE);
			}
		}
		break;

	/**
	 * Das Befüllen wurde abgeschlossen
	 * Das erfolgreiche Abschließen wird an das System gemeldet.
	 * Das Modul bleibt in diesem Zustand, bis das Glas nicht mehr im Modul ist
	 */
	case FINISHED_ICE:
		state->ptrGeneralState->modules_finished[MODULE_NUMBER-1] = 1;
		output->Ice.motor = 0;

		DPRINT_MESSAGE("Ice in State %d\n", state->state);
		state->ptrGeneralState->modules_finished[6]=1;
		if(input.Ice.weight < GLASS_WEIGHT * 0.75){
			vSwitchStateIce(state, ACTIVE_ICE);
		}

		break;
	
	default:
		DPRINT_MESSAGE("%s\n", "Here");
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






