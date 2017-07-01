#include "Module_4_Pouring.h"
#include "Evaluation.h"
#include "Debug.h"
#include "Module_common.h"
//#include "FIFI_Queue.h"


#undef DEBUG_ENABLED
#define DEBUG_ENABLED 1
#include "Debug.h"

//Hier steht die Modulnummer. Sie wird im Code nicht hart gecodet, sondern nur hier eingetragen
//Im Code wird sie dann mit dem Namen MODULE_NUMBER verwendet
#define MODULE_NUMBER 4
#define EMPTY_WEIGHT 15
#define GLASS_WEIGHT 15
#define FILL_ERROR 5 //Fluid amount that gets filled in when the bottle is moved in the up position


/** @brief Initialisierung des Teilmoduls
 *
 * Hier werden die Anfangswerte der Zust舅de und der Variablen eingestellt. Die Funktion wird nur ein einziges
 * Mal beim Systemstart ausgef�hrt
**/
void vInit_Module_4_Pouring(Module_State_4_Pouring_t* state, State_General_t* ptrGeneralState)
{
	//Nicht 舅dern, muss so sein!
	state->state = REFERENCE_POUR;
	state->ptrGeneralState = ptrGeneralState;
	//list_new(input.Pouring.drinkList, sizeof(int[4]));
	state->currentNode = NULL;
	list_new(state->drinkList);

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
 * Soll der Modulzustand gewechselt werden, wird die vSwitchStatePour() Funktion benutzt. Diese pr�ft die generelle Zul舖sigkeit
 * (falls nig) des Zustandswechsels und schreibt einen Debug-Print.
 **/
void vEvaluate_Module_4_Pouring(InputValues_t input, Module_State_4_Pouring_t* state, OutputValues_t* output)
{
	//ﾄndern des Status auf Basis des Gesamtmaschinenzustand
	if (state->ptrGeneralState->operation_mode == stop)
	{
		vSwitchStatePour(state, INACTIVE_POUR);
	}

	//Ausf�hren von Funktionen basierend auf dem Zustand
	switch (state->state){
		case REFERENCE_POUR:
			//Do something
			DPRINT_MESSAGE("I'm in State %d\n", state->state);
			if (!input.Pouring.position_up) {
				output->Pouring.motor = 1;
			} else if (input.Pouring.position_up) {
				output->Pouring.motor = 0;
				vSwitchStatePour(state, ACTIVE_POUR);
			}
			break;
		case ACTIVE_POUR:
			//Do something
			DPRINT_MESSAGE("I'm in State %d\n", state->state);
			if (input.Pouring.weight > EMPTY_WEIGHT) {
				state->drinkWeight = input.Pouring.weight;
				vSwitchStatePour(state, GLASS_IN_STATION_POUR);
			}
			break;
		case INACTIVE_POUR:
			//Do something
			DPRINT_MESSAGE("I'm in State %d\n", state->state);
			output->Pouring.motor = 0;
			if (state->ptrGeneralState->operation_mode != stop) {
				vSwitchStatePour(state, REFERENCE_POUR);
			}
			break;
		case GLASS_IN_STATION_POUR:
			list_head(state->drinkList, state->currentNode, FALSE);
			if (state->currentNode == NULL && input.Pouring.weight < EMPTY_WEIGHT) {
				vSwitchStatePour(state, ACTIVE_POUR);
				break;
			}
			//Preventing NullPointer Exception
			if (state->currentNode != NULL) {
				if (state->currentNode->ingredient.bottleID == 0 && input.Pouring.weight == 0) {
					vSwitchStatePour(state, ACTIVE_POUR);
					break;
				}
				if (state->currentNode->ingredient.bottleID == 0 && input.Pouring.weight >= EMPTY_WEIGHT) {
					break;
				}
				if (state->currentNode->ingredient.bottleID != 0) {
					state->ptrGeneralState->modules_finished[MODULE_NUMBER - 1] = 0;
					vSwitchStatePour(state, POURING);
				}
			}
			break;
		case POURING:
			if (input.Pouring.position_up == 1 && input.Pouring.weight < state->drinkWeight + state->currentNode->ingredient.amount + FILL_ERROR) {
				output->Pouring.motor = -1; //TODO Motorwert
			}
			if (input.Pouring.position_down == 1) {
				output->Pouring.motor = 0;
			}
			if (input.Pouring.weight >= state->drinkWeight + state->currentNode->ingredient.amount + FILL_ERROR && input.Pouring.position_up != 1) {
				output->Pouring.motor = 1; //TODO Wert, Motor soll hochfahren
			}
			if (input.Pouring.weight >= state->drinkWeight + state->currentNode->ingredient.amount + FILL_ERROR && input.Pouring.position_up == 1) {
				state->ptrGeneralState->modules_finished[MODULE_NUMBER - 1] = 1;
				output->Pouring.motor = 0;
				list_head(state->drinkList, state->currentNode, TRUE);
				vSwitchStatePour(state, ACTIVE_POUR);
				break;
			}
			break;
		default:
			break;
		}

		return;

}

void vSwitchStatePour(Module_State_4_Pouring_t* state, int state_new)
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


