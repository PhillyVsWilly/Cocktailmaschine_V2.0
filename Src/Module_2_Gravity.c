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
	state->state = REFERENCE;
	state->ptrGeneralState = ptrGeneralState;
	list_new(&(state->Gravity.drinkList),sizeof(int), NULL);

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

void vEvaluate_Module_2_Gravity(InputValues_t input, SystemState_t* state, OutputValues_t* output)
{
			//Ändern des Status auf Basis des Gesamtmaschinenzustand
			if (state->ptrGeneralState->operation_mode == stop)
			{
				vSwitchState(state, INACTIVE);
			}

			//Ausführen von Funktionen basierend auf dem Zustand
			switch (state->Gravity.state){
				case INACTIVE:
					output->Gravity.move_baum = 0;
					output->Gravity.move_platform = 0;
					if (state->General.operation_mode != stop) {
						vSwitchState(state->Gravity, REFERENCE);
					}
					break;
				case REFERENCE:
					//Drives everything to its reference point
					DPRINT_MESSAGE("I'm in State %d\n", state.Gravity->state);
					if (output->Gravity.platform_Position_Down == TRUE) {
						output->Gravity.move_platform = 0;
						vSwitchState(state->Gravity, ACTIVE);
					} else {
						output->Gravity.move_platform = -1; //Plattform soll herunterfahren
					}
					break;
				case ACTIVE:
					//Do something
					DPRINT_MESSAGE("I'm in State %d\n", state.Gravity->state);
					if (list_get(state->Gravity.drinkList, 0) != 0 && state->Gravity.treeInPosition == false) {
						DPRINT_MESSAGE("Gravity: Getränk ist in Liste, fahre Baum auf passende Position");
						fillDrinkList(list_get(state.Gravity.drinkList, 0));
						//TODO Datenstruktur für Getränke festlegen
						vSwitchState(state.Gravity, MOVING_TREE);
					}
					if (input.Gravity.information_sensor > EMPTY_WEIGHT) {
						state->Sensors.modules_finished[MODULE_NUMBER - 2] = 0;
						vSwitchState(state.Gravity, GLASS_IN_STATION);
					}
					break;
				case MOVING_TREE: //Moving the Tree to the next position
					DPRINT_MESSAGE("I'm in State %d\n", state.Gravity->state);
					if (input.Gravity.position_baum != state->Gravity.currentDrinkList[0]) {
					output->Gravity.move_baum = 1; //TODO Wert bestimmen zum bewegen
					} else {
						state->Gravity.treeInPosition = TRUE;
						vSwitchState(state.Gravity, ACTIVE);
					}

					break;
				case GLASS_IN_STATION:
					DPRINT_MESSAGE("I'm in State %d\n", state.Gravity->state);
					bool finished = TRUE;
					for (int i = 0; i < 8; i++) {
						if (state->Gravity.currentDrinkList[i][1] != 0) {
							finished = FALSE;
							break;
						}
					}
					if (finished == TRUE) {
						vSwitchState(state.Gravity, FILLED_GLASS);
						break;
					}
					if (state.Gravity.currentDrinkList[0][1] == 0) {
						vShuffleDrinks(state.Gravity.currentDrinkList);
					} else {
						vSwitchState(state.Gravity, MOVE_PLATTFORM);
					}

					break;
				case MOVE_PLATTFORM:
					DPRINT_MESSAGE("I'm in State %d\n", state.Gravity->state);
					if (input.Gravity.platform == FALSE) {
						output->Gravity.move_platform = 1; //TODO Wert
					}
					if (input.Gravity.platform == TRUE && state->Gravity.startTicket == 0) {
						state->Gravity.startTicket = xTaskGetTickCount();
					}
					if (input.Gravity.platform == TRUE && state->Gravity.startTicket >= xTaskGetTickCount() + 1000) {
						output->Gravity.move_platform = -1; //TODO Wert
					}
					if (input.Gravity.platform == FALSE && state->Gravity.startTicket != 0) {
						output->Gravity.move_platform = 0;
						state.Gravity.currentDrinkList[0][1]--;
						vSwitchState(state.Gravity, GLASS_IN_STATION);
					}
					break;
				case FILLED_GLASS:
					state->Gravity.finished = TRUE;
					state->Sensors.modules_finished [MODULE_NUMBER - 2] = 1;
					if (input.Gravity.information_sensor = EMPTY_WEIGHT) {
						list_head(state->Gravity.drinkList, NULL, TRUE);
						vSwitchState(state.Gravity, ACTIVE);
					}
					break;
				default:
					break;
				}
				return;
}

void vSwitchState(Module_State_2_Gravity_t* state, int state_new)
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
void vFillDrinkList(listNode *node) {

}

//Wenn ein Drink fertig ist wird der nächste an die Spitze des Arrays gebracht
void vShuffleDrinks(uint_8 list[][]) {
	for (int i = 0; i < 7; i++) {
		list[i][0] = list[i + 1][0];
		list[i][1] = list[i + 1][1];
	}
	list[7][1] = 0;
}
//vHilfsfuntion2() {  }
