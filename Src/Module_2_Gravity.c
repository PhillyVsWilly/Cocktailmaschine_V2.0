#include "Module_common.h"

#include "Module_2_Gravity.h"
#include "Evaluation.h"
#include "Actuators.h"
#include "FIFO_Queue.h"

#undef DEBUG_ENABLED
#define DEBUG_ENABLED 1
#include "Debug.h"

#define MODULE_NUMBER 2
#define EMPTY_WEIGHT 15 //TODO Gewicht eines leeren Schlittens (muss noch gewogen werden)

void vInit_Module_1_Transport(Module_State_2_Gravity_t* state, State_General_t* ptrGeneralState)
{
	//Nicht ändern, muss so sein!
	state->state = REFERENCE_GRAV;
	state->ptrGeneralState = ptrGeneralState;
	//list_new(&(state->Gravity.drinkList),sizeof(int), NULL);

	// Hier können jetzt noch - falls nötig - Startwerte für die anderen Zustandsvariablen gegeben werden
}

int vCheckForGeneralErrors(InputValues_t input)
{

	if(input.Module_x_Name.placeholder > 10.0)
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

			//Ausführen von Funktionen basierend auf dem Zustand
			switch (state->state){
				case INACTIVE_GRAV:
					output->Gravity.move_baum = 0;
					output->Gravity.move_platform = 0;
					if (state->ptrGeneralState != stop) {
						vSwitchStateGrav(state, REFERENCE_GRAV);
					}
					break;
				case REFERENCE_GRAV:
					//Drives everything to its reference point
					DPRINT_MESSAGE("I'm in State %d\n", state->state);
					if (input.Gravity.sensor_down == TRUE) {
						output->Gravity.move_platform = 0;
						vSwitchStateGrav(state, IDLE_GRAV);
					} else {
						output->Gravity.move_platform = -1; //Plattform soll herunterfahren
					}
					break;
				case IDLE_GRAV:
					//Do something
					DPRINT_MESSAGE("I'm in State %d\n", state->state);
//					if (list_get(state->drinkList, 0) != 0 && state->treeInPosition == FALSE) {
//						DPRINT_MESSAGE("Gravity: Getränk ist in Liste, fahre Baum auf passende Position");
//						fillDrinkList(list_get(state->drinkList, 0));
//						//TODO Datenstruktur für Getränke festlegen
//						vSwitchStateGrav(state, MOVING_TREE);
//					}
					if (input.Gravity.weight_sensor > EMPTY_WEIGHT) {
						input.Sensors.modules_finished[MODULE_NUMBER - 2] = 0;
						vSwitchStateGrav(state, GLASS_IN_STATION);
					}
					break;
				case MOVING_TREE: //Moving the Tree to the next position
					DPRINT_MESSAGE("I'm in State %d\n", state->state);
					if (input.Gravity.position_tree != state->currentDrinkList[0]) {
					output->Gravity.move_baum = 1; //TODO Wert bestimmen zum bewegen
					} else {
						state->treeInPosition = TRUE;
						vSwitchStateGrav(state, IDLE_GRAV);
					}

					break;
				case GLASS_IN_STATION:
					DPRINT_MESSAGE("I'm in State %d\n", state->state);
					bool finished = TRUE;
					for (int i = 0; i < 8; i++) {
						if (state->currentDrinkList[i][1] != 0) {
							finished = FALSE;
							break;
						}
					}
					if (finished == TRUE) {
						vSwitchStateGrav(state, FILLED_GLASS);
						break;
					}
					if (state->currentDrinkList[0][1] == 0) {
						vShuffleDrinks(state->currentDrinkList);
					} else {
						vSwitchStateGrav(state, MOVE_PLATTFORM);
					}

					break;
				case MOVE_PLATTFORM:
					DPRINT_MESSAGE("I'm in State %d\n", state->state);
					if (input.Gravity.sensor_down == TRUE) {
						output->Gravity.move_platform = 1; //TODO Wert
					}
					if (input.Gravity.sensor_up == TRUE && state->startTicket == 0) {
						state->startTicket = xTaskGetTickCount();
					}
					if (input.Gravity.sensor_up == TRUE && state->startTicket >= xTaskGetTickCount() + 1000) {
						output->Gravity.move_platform = -1; //TODO Wert
					}
					if (input.Gravity.sensor_down == FALSE && state->startTicket != 0) {
						output->Gravity.move_platform = 0;
						state->currentDrinkList[0][1]--;
						vSwitchStateGrav(state, GLASS_IN_STATION);
					}
					break;
				case FILLED_GLASS:
					state->finished = TRUE;
					//state->Sensors.modules_finished [MODULE_NUMBER - 2] = 1;
					if (input.Gravity.weight_sensor = EMPTY_WEIGHT) {
						//list_head(state->drinkList, NULL, TRUE);
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

//Wenn ein Drink fertig ist wird der nächste an die Spitze des Arrays gebracht
void vShuffleDrinks(uint_8 list[8][2]) {
	for (int i = 0; i < 7; i++) {
		list[i][0] = list[i + 1][0];
		list[i][1] = list[i + 1][1];
	}
	list[7][1] = 0;
}
//vHilfsfuntion2() {  }
