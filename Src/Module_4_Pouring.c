#include "Module_4_Pouring.h"
#include "Evaluation.h"
#include "Debug.h"
#include "LinkedList.h"


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
 * Hier werden die Anfangswerte der Zust�nde und der Variablen eingestellt. Die Funktion wird nur ein einziges
 * Mal beim Systemstart ausgef�hrt
**/
void vInit_Module_4_Pouring(Module_State_x_Name_t* state, State_General_t* ptrGeneralState)
{
	//Nicht �ndern, muss so sein!
	state->state = REFERENCE;
	state->ptrGeneralState = ptrGeneralState;

	// Hier k�nnen jetzt noch - falls n�tig - Startwerte f�r die anderen Zustandsvariablen gegeben werden
}

/** @brief Pr�fe nach allgmeinen Fehlern
 *Diese Funktion muss bei jedem Zyklus ausgef�hrt werden. In ihr werden Systemzust�nde �berpr�ft, die unabh�ngig vom
 *Ablauf des Systems sind.
 *Bsp: Beim Transportmodul: Keine Hand in der Maschine
 *Beim Eis: �berlaufbecken nicht voll
 *Deshalb wird ihr auch nicht der Systemzustand �bergeben
 **/
int vCheckForGeneralErrors(InputValues_t input)
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
 * Danach wird in Abh�ngigkeit des Modulzustands state->state und des Betriebszustands *(state->ptrGeneralState) eine
 * oder mehrere bestimmte Aktionen ausgef�hrt und deren Verlauf �berwacht
 * Hier k�nnen �ber ThrowError() auch weitere Fehler geworfen werden.
 * Soll der Modulzustand gewechselt werden, wird die vSwitchState() Funktion benutzt. Diese pr�ft die generelle Zul�ssigkeit
 * (falls n�tig) des Zustandswechsels und schreibt einen Debug-Print.
 **/
void vEvaluate_Module_4_Pouring(InputValues_t input, SystemState_t* state, OutputValues_t* output)
{
	//�ndern des Status auf Basis des Gesamtmaschinenzustand
	if (state->General->operation_mode == stop)
	{
		vSwitchState(state, INACTIVE);
	}

	//Ausf�hren von Funktionen basierend auf dem Zustand
	switch (state->Pouring.state){
		case REFERENCE:
			//Do something
			DPRINT_MESSAGE("I'm in State %d\n", state->state);
			if (!input.Pouring.position_up) {
				output->Pouring.motor = 1;
			} else if (input.Pouring.position_up) {
				output->Pouring.motor = 0;
				vSwitchState(state->Pouring, ACTIVE);
			}
			break;
		case ACTIVE:
			//Do something
			DPRINT_MESSAGE("I'm in State %d\n", state->state);
			if (input.Pouring.weight > EMPTY_WEIGHT) {
				vSwitchState(state->Pouring, GLASS_IN_STATION);
			}
			break;
		case INACTIVE:
			//Do something
			DPRINT_MESSAGE("I'm in State %d\n", state->state);
			output->Pouring.motor = 0;
			if (state->General.operation_mode != stopp) {
				vSwitchState(state->Pouring, REFERENCE);
			}
			break;
		case GLASS_IN_STATION:
			               list_head(state->Pouring.drinkList,state->Pouring.currentNode, FALSE);
			if (state->Pouring->currentNode->data == 0 && input.Pouring.weight == EMPTY_WEIGHT) {
				list_head(state->Pouring.drinkList,state->Pouring.currentNode, TRUE);
				vSwitchState(state->Pouring, ACTIVE);
				break;
			}
			if (state->Pouring->currentNode->data != 0 && state->Pouring.drinkWeight == 0) {
				state->Pouring.drinkWeight = EMPTY_WEIGHT + GLASS_WEIGHT + state->Pouring->currentNode->data; //TODO Datenstruktur hier anpassen
			} else if (state->Pouring->currentNode->data != 0) {
				vSwitchState(state->Pouring, POURING);
			}

			break;

		case POURING:
			state->Sensors.modules_finished [MODULE_NUMBER - 2] = 0;
			if (input.Pouring.position_up == 1 && input.Pouring.weight < state->Pouring.drinkWeight) {
				output->Pouring.motor = 1; //TODO Motorwert
			}
			if (input.Pouring.position_down == 1) {
				output->Pouring.motor = 0;
			}
			if (input.Pouring.weight >= state->Pouring.drinkWeight - FILL_ERROR && input.Pouring.position_up != 1) {
				output->Pouring.motor = 1; //TODO Wert, Motor soll hochfahren
			}
			if (input.Pouring.weight >= state->Pouring.drinkWeight - FILL_ERROR && input.Pouring.position_up == 1) {
				state->Sensors.modules_finished [MODULE_NUMBER - 2] = 1;
				output->Pouring.motor = 0;
				vSwitchState(state->Pouring, ACTIVE);
				break;
			}
			break;
		default:
			break;
		}

		return;

}

void vSwitchState(Module_State_4_Pouring_t* state, int state_new)
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


