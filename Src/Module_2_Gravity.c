#include "Module_common.h"
#include "FreeRTOS.h"

#include "Module_2_Gravity.h"
#include "Evaluation.h"
#include "Actuators.h"
#include "FIFO_Queue.h"

#undef DEBUG_ENABLED
#define DEBUG_ENABLED 1
#include "Debug.h"

#define MODULE_NUMBER 2
#define EMPTY_WEIGHT 15 //TODO Gewicht eines leeren Schlittens (muss noch gewogen werden)

void vInit_Module_2_Gravity(Module_State_2_Gravity_t* state, State_General_t* ptrGeneralState)
{
	//Nicht ändern, muss so sein!
	state->state = REFERENCE_GRAV;
	state->ptrGeneralState = ptrGeneralState;
	list_new(state->drinkList);
	state->currentNode = NULL;
	state->glassInStation = FALSE;

	// Hier können jetzt noch - falls nötig - Startwerte für die anderen Zustandsvariablen gegeben werden
}

static int vCheckForGeneralErrors(InputValues_t input)
{

	if(input.Module_x_Name.placeholder > 10.0) //TODO
	{
		//ThrowError ist die zentrale "Fehlerverwaltung". An sie werden alle Fehler übergeben, die geworfen werden sollen
		ThrowError(MODULE_NUMBER, MOTOR1_NOT_MOVING);

		//Gibt den aktuell geworfenen Fehler aus
		return MOTOR1_NOT_MOVING;
	}

	return -1;
}

void vEvaluate_Module_2_Gravity(InputValues_t input, Module_State_2_Gravity_t* state, OutputValues_t* output)
{
			//Ändern des Status auf Basis des Gesamtmaschinenzustand
			if (state->ptrGeneralState->operation_mode == stop)
			{
				vSwitchStateGrav(state, INACTIVE_GRAV);
			}
			if (input.Gravity.doors_open) {
				vSwitchStateGrav(state, INACTIVE_GRAV);
				state->ptrGeneralState->modules_finished[MODULE_NUMBER - 1] = 0;
			}

			//Ausführen von Funktionen basierend auf dem Zustand
			switch (state->state){
				case INACTIVE_GRAV:
					output->Gravity.move_baum = 0;
					output->Gravity.move_platform = 0;
					if (state->ptrGeneralState->operation_mode != stop) {
						vSwitchStateGrav(state, REFERENCE_GRAV);
					}
					break;
				case REFERENCE_GRAV:
					//Drives everything to its reference point
					DPRINT_MESSAGE("I'm in State %d\n", state->state);
					if (state->stateTicket + 5000 > xTaskGetTickCount()) { //TODO Wert anpassen
						ThrowError(2, BAD_POSITION);
						break;
					}
					if (input.Gravity.sensor_down == TRUE) {
						output->Gravity.move_platform = 0;
						vSwitchStateGrav(state, IDLE_GRAV);
						state->ptrGeneralState->modules_finished[MODULE_NUMBER - 1] = 1;
					} else {
						output->Gravity.move_platform = -1; //Plattform soll herunterfahren
					}
					break;
				case IDLE_GRAV:
					//Do something
					DPRINT_MESSAGE("I'm in State %d\n", state->state);
					if (input.Gravity.weight_sensor > EMPTY_WEIGHT) {
						state->drinkWeight = input.Gravity.weight_sensor;
						state->glassInStation = TRUE;
						vSwitchStateGrav(state, GLASS_IN_STATION);
						break;
					}
					list_head(state->drinkList, state->currentNode, FALSE);
					if (state-> currentNode != NULL) {
						if (state->currentNode->ingredient.bottleID != 0) {
							vSwitchStateGrav(state, MOVING_TREE);
							break;
						}
					}

					break;
				case MOVING_TREE: //Moving the Tree to the next position
					DPRINT_MESSAGE("I'm in State %d\n", state->state);
					if (state->stateTicket + 5000 > xTaskGetTickCount()) { //TODO Wert anpassen
											ThrowError(2, MOTOR1_NOT_MOVING);
											break;
					}
					if (input.Gravity.position_tree != state->currentNode->ingredient.bottleID) {
					output->Gravity.move_baum = 1; //TODO Wert bestimmen zum bewegen
					}
					if (input.Gravity.position_tree == state->currentNode->ingredient.bottleID) {
						output->Gravity.move_baum = 0;
						state->treeInPosition = TRUE;
						vSwitchStateGrav(state, IDLE_GRAV);
						break;
					}
					if (input.Gravity.weight_sensor > EMPTY_WEIGHT) {
						output->Gravity.move_baum = 0;
						if (state->glassInStation == FALSE) {
						vSwitchStateGrav(state, IDLE_GRAV);
						} else if (state->glassInStation == TRUE) {
							vSwitchStateGrav(state, GLASS_IN_STATION);
						}
					}
					break;
				case GLASS_IN_STATION:
					DPRINT_MESSAGE("I'm in State %d\n", state->state);
					if (state->stateTicket + 5000 > xTaskGetTickCount()) { //TODO Wert anpassen
											ThrowError(2, BAD_POSITION);
											break;
					}
					list_head(state->drinkList, state->currentNode, FALSE);
					if (state->currentNode == NULL && input.Gravity.weight_sensor < EMPTY_WEIGHT) {
						vSwitchStateGrav(state, IDLE_GRAV);
						state->glassInStation = FALSE;
						break;
					}
					if (state->currentNode != NULL) {
						if (state->currentNode->ingredient.bottleID == 0 && input.Gravity.weight_sensor >= EMPTY_WEIGHT ) {
							break;
						}
						if (state->currentNode->ingredient.bottleID == 0 && input.Gravity.weight_sensor < EMPTY_WEIGHT ) {
							vSwitchStateGrav(state, IDLE_GRAV);
							state->glassInStation = FALSE;
							break;
						}
						state->ptrGeneralState->modules_finished[MODULE_NUMBER - 1] = 0;
						if (input.Gravity.position_tree != state->currentNode->ingredient.bottleID) {
							vSwitchStateGrav(state, MOVING_TREE);
							break;
						}
						if (state->currentNode->ingredient.amount > 0) {
							vSwitchStateGrav(state, MOVE_PLATTFORM);
							break;
						}
						if (state->currentNode->ingredient.amount <= 0 && state->currentNode->ingredient.lastInstruction == TRUE) {
							state->ptrGeneralState->modules_finished[MODULE_NUMBER - 1] = 1;
							vSwitchStateGrav(state, FILLED_GLASS);
							break;
						}
						if (state->currentNode->ingredient.amount <= 0 && state->currentNode->ingredient.lastInstruction == FALSE) {
							list_head(state->drinkList, state->currentNode, TRUE);
							list_head(state->drinkList, state->currentNode, FALSE);
							vSwitchStateGrav(state, MOVING_TREE);
							break;
						}
					}
					break;
				case MOVE_PLATTFORM:
					DPRINT_MESSAGE("I'm in State %d\n", state->state);
					if (state->stateTicket + 5000 > xTaskGetTickCount()) { //TODO Wert anpassen
											ThrowError(2, BAD_POSITION);
											break;
					}
					if (input.Gravity.sensor_down == TRUE) {
						output->Gravity.move_platform = 1; //TODO Wert
					}
					if (input.Gravity.sensor_up == TRUE && state->startTicket == NULL) {
						state->startTicket = xTaskGetTickCount();
					}
					if (input.Gravity.sensor_up == TRUE && state->startTicket >= xTaskGetTickCount() + 1000) {
						output->Gravity.move_platform = -1; //TODO Wert
					}
					if (input.Gravity.sensor_down == FALSE && state->startTicket != 0) {
						output->Gravity.move_platform = 0;
						state->currentNode->ingredient.amount--;
						vSwitchStateGrav(state, GLASS_IN_STATION);
					}
					break;
				case FILLED_GLASS:
					if (input.Gravity.weight_sensor == 0) {
						list_head(state->drinkList, state->currentNode, TRUE);
						state->glassInStation = FALSE;
						vSwitchStateGrav(state, IDLE_GRAV);
					}
					break;
				default:
					break;
				}
				return;
}

void vSwitchStateGrav(Module_State_2_Gravity_t* state, int state_new)
{
	//Hier kommt alles rein, was bei jedem(!) Zustandswechsel passieren soll
	DPRINT_MESSAGE("Switching states from State %d to State %d\r\n", state->state, state_new);
	
	//Das hier sollte passieren, sonst wird der Zustand nicht gewechselt
	state->state = state_new;
	state->stateTicket = xTaskGetTickCount();
	
	return;
}

/*****************************************************************/
/*****					HILFSFUNKTIONEN															******/
/* Hier komme alle Codeabschnitte hin, die mehr als 10-20 Zeilen */
/* besitzen, damit vEvaluate nicht zu aufgeblasen wird					 */
/*****************************************************************/

//Function for filling the array  with the beverages for the current drink
//void vFillDrinkList(listNode *node) {
//
//}

//vHilfsfuntion2() {  }
