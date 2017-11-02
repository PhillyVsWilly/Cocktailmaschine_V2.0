#include "Module_common.h"
#include "FreeRTOS.h"
#include "project_conf.h"

#include "Module_2_Gravity.h"
#include "Evaluation.h"
#include "Actuators.h"
#include "FIFO_Queue.h"

#undef DEBUG_ENABLED
#define DEBUG_ENABLED 1
#include "Debug.h"

#define MODULE_NUMBER 2
#define EMPTY_WEIGHT 15

#define MOTOR_SPEED_PLATFORM 50

void vInit_Module_2_Gravity(Module_State_2_Gravity_t* state,
		State_General_t* ptrGeneralState) {
	//Nicht ändern, muss so sein!
	state->state = REFERENCE_GRAV;
	state->ptrGeneralState = ptrGeneralState;
	list_new(&state->drinkList);
	state->currentNode = NULL;
	state->glassInStation = FALSE;
	state->ptrGeneralState->ErrFlags[MODULE_NUMBER - 1] = 0;
	state->ptrGeneralState->CritFlags[MODULE_NUMBER - 1] = 0;
	state->ptrGeneralState->WarnFlags[MODULE_NUMBER - 1] = 0;
	state->startTicket = 0;
	state->platform_been_up = 0;
	state->plat_start_time = 0;
	state->plat_up_time = 0;

}

static int vCheckForGeneralErrors(InputValues_t input) {
	/*if(input.Gravity.doors_open == SWITCH_OPEN)
	 {
	 ThrowError(MODULE_NUMBER, DOOR_OPEN);
	 }*/

}

void vEvaluate_Module_2_Gravity(InputValues_t input,
		Module_State_2_Gravity_t* state, OutputValues_t* output) {
	//Ändern des Status auf Basis des Gesamtmaschinenzustand
	/*if (state->ptrGeneralState->operation_mode == stop)
	 {
	 vSwitchStateGrav(state, INACTIVE_GRAV);
	 }

	 vCheckForGeneralErrors(input);
	 if(state->ptrGeneralState->ErrFlags[MODULE_NUMBER-1] != 0 ||
	 state->ptrGeneralState->CritFlags[MODULE_NUMBER-1] != 0)
	 {
	 vSwitchStateGrav(state, INACTIVE_GRAV);
	 }*/

	//Ausführen von Funktionen basierend auf dem Zustand
	switch (state->state) {
	case INACTIVE_GRAV:
		state->ptrGeneralState->modules_finished[MODULE_NUMBER - 1] = 0;
		output->Gravity.move_baum = 50;
		output->Gravity.move_platform = 0;

		if (state->ptrGeneralState->operation_mode != stop
				&& state->ptrGeneralState->ErrFlags[MODULE_NUMBER - 1] == 0
				&& state->ptrGeneralState->CritFlags[MODULE_NUMBER - 1] == 0) {
			vSwitchStateGrav(state, REFERENCE_GRAV);
		}
		break;

	case REFERENCE_GRAV:
		state->ptrGeneralState->modules_finished[MODULE_NUMBER - 1] = 0;
		//Drives everything to its reference point
		DPRINT_MESSAGE("I'm in State %d\n", state->state);
		if (input.Gravity.sensor_down == BTN_PRESSED) {
			output->Gravity.move_platform = 0;
			vSwitchStateGrav(state, IDLE_GRAV);
			state->ptrGeneralState->modules_finished[MODULE_NUMBER - 1] = 1;
		} else {
			output->Gravity.move_platform = -MOTOR_SPEED_PLATFORM; //Plattform soll herunterfahren
		}
		break;
	case IDLE_GRAV:
		state->ptrGeneralState->modules_finished[MODULE_NUMBER - 1] = 1;
		//Do something
		if (state->ptrGeneralState->glassCount > 0) {
			state->plat_start_time = xTaskGetTickCount();
			vSwitchStateGrav(state, PARTY_PLAT);
		}
		/*DPRINT_MESSAGE("I'm in State %d\n", state->state);
		 if (input.Gravity.weight_sensor > EMPTY_WEIGHT) {
		 state->drinkWeight = input.Gravity.weight_sensor;
		 state->glassInStation = TRUE;
		 vSwitchStateGrav(state, GLASS_IN_STATION);
		 break;
		 }
		 list_head(&state->drinkList, &state->currentNode, FALSE);
		 if (state-> currentNode != NULL && !state->treeInPosition && state->glassInStation) {
		 if (state->currentNode->ingredient.bottleID != -1) {
		 vSwitchStateGrav(state, MOVING_TREE);
		 break;
		 }
		 }*/

		break;
	case MOVING_TREE: //Moving the Tree to the next position
		state->ptrGeneralState->modules_finished[MODULE_NUMBER - 1] = 0;
		DPRINT_MESSAGE("I'm in State %d\n", state->state);
		if (input.Gravity.position_tree
				!= state->currentNode->ingredient.bottleID) {
			output->Gravity.move_baum = 50; //TODO Motorgeschwindigkeit einstellen
		}

		if (input.Gravity.position_tree
				== state->currentNode->ingredient.bottleID) {
			output->Gravity.move_baum = 50;
			state->treeInPosition = TRUE;
			if (input.Gravity.button_fill_in == BTN_PRESSED) {
				output->Gravity.move_baum = 0;
				vSwitchStateGrav(state, MOVE_PLATTFORM);
			}
			break;
		}
		/*if (input.Gravity.weight_sensor > EMPTY_WEIGHT) {
		 output->Gravity.move_baum = 50;

		 if (state->glassInStation == FALSE) {
		 vSwitchStateGrav(state, IDLE_GRAV);
		 } else if (state->glassInStation == TRUE) {
		 vSwitchStateGrav(state, GLASS_IN_STATION);
		 }
		 }*/
		break;
	case GLASS_IN_STATION:
		state->ptrGeneralState->modules_finished[MODULE_NUMBER - 1] = 1;
		DPRINT_MESSAGE("I'm in State %d\n", state->state);
		list_head(&state->drinkList, &state->currentNode, FALSE);

		if (input.Gravity.weight_sensor < EMPTY_WEIGHT) {
			vSwitchStateGrav(state, IDLE_GRAV);
			state->glassInStation = FALSE;
		}

		DPRINT_MESSAGE("Amount: %d\n", state->currentNode->ingredient.amount);

		if (state->currentNode != NULL) {

			if (state->currentNode->ingredient.amount > 0) {
				if (input.Gravity.position_tree
						!= state->currentNode->ingredient.bottleID|| !input.Gravity.button_fill_in == BTN_PRESSED) {
					state->ptrGeneralState->modules_finished[MODULE_NUMBER - 1] =
							0;
					vSwitchStateGrav(state, MOVING_TREE);
				} else {
					state->ptrGeneralState->modules_finished[MODULE_NUMBER - 1] =
							0;
					vSwitchStateGrav(state, MOVE_PLATTFORM);
				}
			}
		}
		break;
	case MOVE_PLATTFORM:
		state->ptrGeneralState->modules_finished[MODULE_NUMBER - 1] = 0;
		DPRINT_MESSAGE("I'm in State %d\n", state->state);

		if (!input.Gravity.sensor_up == BTN_PRESSED
				&& !state->platform_been_up) {
			output->Gravity.move_platform = MOTOR_SPEED_PLATFORM; //TODO Motorgeschwindigkeit einstellen
		}
		if (input.Gravity.sensor_up == BTN_PRESSED) {
			state->platform_been_up = 1;
		}
		if (state->platform_been_up == TRUE
				&& !input.Gravity.sensor_down == BTN_PRESSED) {
			output->Gravity.move_platform = -MOTOR_SPEED_PLATFORM; //TODO Motorgeschwindigkeit einstellen
		}
		if (input.Gravity.sensor_down == BTN_PRESSED
				&& state->platform_been_up) {
			output->Gravity.move_platform = 0;
			state->currentNode->ingredient.amount--;
			state->platform_been_up = 0;
			if (state->currentNode->ingredient.amount > 0) {
				vSwitchStateGrav(state, MOVE_PLATTFORM);
			} else {
				vSwitchStateGrav(state, FILLED_GLASS);
			}

		}
		break;
	case FILLED_GLASS:
		state->ptrGeneralState->modules_finished[MODULE_NUMBER - 1] = 1;
		if (input.Gravity.weight_sensor == 0) {
			list_head(&state->drinkList, &state->currentNode, TRUE);
			state->glassInStation = FALSE;
			vSwitchStateGrav(state, IDLE_GRAV);
		}
		break;

	case PARTY_PLAT:

		if (input.Gravity.sensor_up != BTN_PRESSED && !state->platform_been_up) {
			state->platform_been_up = 0;
			output->Gravity.move_platform = MOTOR_SPEED_PLATFORM;
		}
		if (input.Gravity.sensor_up == BTN_PRESSED ) {
			state->platform_been_up = 1;
			output->Gravity.move_platform = -MOTOR_SPEED_PLATFORM;
			state->plat_up_time = xTaskGetTickCount();
		} else if (state->platform_been_up == 1) {
			output->Gravity.move_platform = -MOTOR_SPEED_PLATFORM;
		} else if (state->platform_been_up == 1 && input.Gravity.sensor_down == BTN_PRESSED ) {
			output->Gravity.move_platform = MOTOR_SPEED_PLATFORM;

			vSwitchStateGrav(state, PARTY_TREE_1);

		}
		break;

	case PARTY_TREE_1:
		output->Gravity.move_platform = 0;
		if (input.Gravity.position_tree != 5) {
			output->Gravity.move_baum = 50; //TODO Motorgeschwindigkeit einstellen
		}

		if (input.Gravity.position_tree == 5) {
			output->Gravity.move_baum = 50;
			state->treeInPosition = TRUE;
			if (input.Gravity.button_fill_in == BTN_PRESSED) {
				output->Gravity.move_baum = 0;
				vSwitchStateGrav(state, PARTY_TREE_2);
			}

		}
		break;

	case PARTY_TREE_2:
		output->Gravity.move_platform =  0;
		if (input.Gravity.position_tree != 4) {
			output->Gravity.move_baum = -50; //TODO Motorgeschwindigkeit einstellen
		}

		if (input.Gravity.position_tree == 4) {
			output->Gravity.move_baum = -50;
			state->treeInPosition = TRUE;
			if (input.Gravity.button_fill_in == BTN_PRESSED) {
				output->Gravity.move_baum = 0;
				state->plat_start_time = xTaskGetTickCount();
				vSwitchStateGrav(state, PARTY_PLAT);
			}

		}
		break;

	default:
		break;
	}
	return;
}

void vSwitchStateGrav(Module_State_2_Gravity_t* state, int state_new) {
	//Hier kommt alles rein, was bei jedem(!) Zustandswechsel passieren soll
	DPRINT_MESSAGE("Switching states from State %d to State %d\r\n",
			state->state, state_new);

	//Das hier sollte passieren, sonst wird der Zustand nicht gewechselt
	state->state = state_new;

	return;
}

/*****************************************************************/
/*****					HILFSFUNKTIONEN															******/
/* Hier komme alle Codeabschnitte hin, die mehr als 10-20 Zeilen */
/* besitzen, damit vEvaluate nicht zu aufgeblasen wird					 */
/*****************************************************************/

