#include "Module_7_Ice.h"
#include "Evaluation.h"
#include "Module_common.h"


#include "Sensors.h"
#include "Evaluation.h"
#include "Actuators.h"

#undef DEBUG_ENABLED
#define DEBUG_ENABLED 1
#include "Debug.h"

//Hier steht die Modulnummer. Sie wird im Code nicht hart gecodet, sondern nur hier eingetragen
//Im Code wird sie dann mit dem Namen MODULE_NUMBER verwendet
#define MODULE_NUMBER 10


/** @brief Initialisierung des Teilmoduls
 *
 * Hier werden die Anfangswerte der Zust舅de und der Variablen eingestellt. Die Funktion wird nur ein einziges
 * Mal beim Systemstart ausgef�hrt
**/
void vInit_Module_7_Ice(Module_State_7_Ice_t* state, State_General_t* ptrGeneralState)
{
	//Nicht 舅dern, muss so sein!
	state->state = REFERENCE_ICE;
	state->ptrGeneralState = ptrGeneralState;
	list_new(state->drinkList);
	state->currentNode = NULL;
	state->glassInStation = FALSE;
	// Hier knen jetzt noch - falls nig - Startwerte f�r die anderen Zustandsvariablen gegeben werden
}

/** @brief Pr�fe nach allgmeinen Fehlern
 *Diese Funktion muss bei jedem Zyklus ausgef�hrt werden. In ihr werden Systemzust舅de �berpr�ft, die unabh舅gig vom
 *Ablauf des Systems sind.
 *Bsp: Beim Transportmodul: Keine Hand in der Maschine
 *Beim Eis: ﾜberlaufbecken nicht voll
 *Deshalb wird ihr auch nicht der Systemzustand �bergeben
 **/
static int vCheckForGeneralErrors(InputValues_t input)
{

	if(input.Module_x_Name.placeholder > 10.0)
	{
		//ThrowError ist die zentrale "Fehlerverwaltung". An sie werden alle Fehler �bergeben, die geworfen werden sollen
		ThrowError(MODULE_NUMBER, MOTOR1_NOT_MOVING);

		//Gibt den aktuell geworfenen Fehler aus
		return MOTOR1_NOT_MOVING;
	}

	return -1;
}

/** @brief Zentrale Ablaufsteuerung des Moduls
 *
 * Dieses Modul wird in jedem Zyklus aufgerufen und steuert das Modul
 * In ihr wird zuerst vCheckForErrors aufgerufen um zuerst nach allgemeinen Fehlern zu suchen
 * Danach wird in Abh舅gigkeit des Modulzustands state->state und des Betriebszustands *(state->ptrGeneralState) eine
 * oder mehrere bestimmte Aktionen ausgef�hrt und deren Verlauf �berwacht
 * Hier knen �ber ThrowError() auch weitere Fehler geworfen werden.
 * Soll der Modulzustand gewechselt werden, wird die vSwitchStateIce() Funktion benutzt. Diese pr�ft die generelle Zul舖sigkeit
 * (falls nig) des Zustandswechsels und schreibt einen Debug-Print.
 **/
void vEvaluate_Module_7_Ice(InputValues_t input, Module_State_7_Ice_t* state, OutputValues_t* output)
{
	//listNode *ls_head = 0;[0] = Gewicht Eisw�rfel, [1] = Gewicht crushed eis

	//ﾄndern des Status auf Basis des Gesamtmaschinenzustand
	if (state->ptrGeneralState->operation_mode == stop)
	{
		vSwitchStateIce(state, INACTIVE_ICE);
	}

	//Ausf�hren von Funktionen basierend auf dem Zustand
	switch (state->state){
		case INACTIVE_ICE:
			//Do something
			DPRINT_MESSAGE("I'm in State %d\n", state->state);
			output->Ice.motor = 0;

			if (state->ptrGeneralState->operation_mode != stop)
				{
					vSwitchStateIce(state,REFERENCE_ICE);
				}
			break;



		case REFERENCE_ICE:
			//Do something
			DPRINT_MESSAGE("I'm in State %d\n", state->state);
			output->Ice.motor = 0;

			if(input.Ice.weight > 0){
				vSwitchStateIce(state,ACTIVE_ICE);
			}

			break;
		case ACTIVE_ICE:


			DPRINT_MESSAGE("I'm in State %d\n", state->state);
			list_head(state->drinkList,state->currentNode,FALSE);
			if(input.Ice.weight && state->currentNode->ingredient.amount != 0){
				vSwitchStateIce(state, FILL_ICE);
				state->glassInStation = TRUE;
				state->ptrGeneralState->modules_finished[7]=1;

			}
			break;

		case FILL_ICE:
			state->ptrGeneralState->modules_finished[7]=0;
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

			DPRINT_MESSAGE("I'm in State %d\n", state->state);
			state->ptrGeneralState->modules_finished[7]=1;
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
	
	return;
}

/*****************************************************************/
/*****					HILFSFUNKTIONEN															******/
/* Hier komme alle Codeabschnitte hin, die mehr als 10-20 Zeilen */
/* besitzen, damit vEvaluate nicht zu aufgeblasen wird					 */
/*****************************************************************/

//vHilfsfuntion1() {  }
//vHilfsfuntion2() {  }




