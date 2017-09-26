#include "Module_5_Sensors.h"
#include "Module_common.h"
#include "FreeRTOS.h"

#include "Sensors.h"
#include "Evaluation.h"
#include "Actuators.h"

#undef DEBUG_ENABLED
#define DEBUG_ENABLED 1
#include "Debug.h"

//Hier steht die Modulnummer. Sie wird im Code nicht hart gecodet, sondern nur hier eingetragen
//Im Code wird sie dann mit dem Namen MODULE_NUMBER verwendet
#define MODULE_NUMBER 5
#define WAIT_TIME 250


/** @brief Initialisierung des Teilmoduls
 *
 * Hier werden die Anfangswerte der Zust舅de und der Variablen eingestellt. Die Funktion wird nur ein einziges
 * Mal beim Systemstart ausgef�hrt
**/
void vInit_Module_5_Sensors(Module_State_5_Sensors_t* state, State_General_t* ptrGeneralState)
{
	//Nicht 舅dern, muss so sein!
	state->state = REFERENCE_SENS;
	state->ptrGeneralState = ptrGeneralState;
	state->glassCount = 0;
	state->lightBarrierStart = 0;
	state->lightBarrierEnd = 0;
	state->startTicket = 0;

	// Hier knen jetzt noch - falls nig - Startwerte f�r die anderen Zustandsvariablen gegeben werden
}

/** @brief Pr�fe nach allgmeinen Fehlern
 *Diese Funktion muss bei jedem Zyklus ausgef�hrt werden. In ihr werden Systemzust舅de �berpr�ft, die unabh舅gig vom
 *Ablauf des Systems sind.
 *Bsp: Beim Transportmodul: Keine Hand in der Maschine
 *Beim Eis: ﾜberlaufbecken nicht voll
 *Deshalb wird ihr auch nicht der Systemzustand �bergeben
 **/
int vCheckForGeneralErrorsSens(InputValues_t input)
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
 * Soll der Modulzustand gewechselt werden, wird die vSwitchState() Funktion benutzt. Diese pr�ft die generelle Zul舖sigkeit
 * (falls nig) des Zustandswechsels und schreibt einen Debug-Print.
 **/
void vEvaluate_Module_5_Sensors(InputValues_t input, Module_State_5_Sensors_t* state, OutputValues_t* output)
{
	//ﾄndern des Status auf Basis des Gesamtmaschinenzustand
	if (state->ptrGeneralState->operation_mode == stop)
	{
		vSwitchStateSens(state, STOP_SENS);
	}
	if (state->glassCount < state->ptrGeneralState->glassCount) {
		vSwitchStateSens(state,NEW_GLAS);
		state->startTicket = xTaskGetTickCount();
		state->ptrGeneralState->modules_finished[MODULE_NUMBER -1] = 0;
	}
	
	//Ausf�hren von Funktionen basierend auf dem Zustand
	switch (state->state){
		case STOP_SENS:
		if (safetyCheck(input)) {
			state->ptrGeneralState->operation_mode = startup;
		}
		break;
		case GLAS_AT_END:
			//Do something
			DPRINT_MESSAGE("I'm in State %d\n", state->state);
			if (safetyCheck(input) && input.Sensors.end_button_glass_present == FALSE) {
				vSwitchStateSens(state, ACTIVE_SENS);
				state->ptrGeneralState->modules_finished[MODULE_NUMBER - 1] = 1;
				break;
		case REFERENCE_SENS:
			//Do something
			DPRINT_MESSAGE("I'm in State %d\n", state->state);
			state->ptrGeneralState->operation_mode = startup;
			state->ptrGeneralState->modules_finished[MODULE_NUMBER -1] = 1;
			vSwitchStateSens(state, ACTIVE_SENS);
			break;
		case ACTIVE_SENS:
			//Do something
			DPRINT_MESSAGE("I'm in State %d\n", state->state);
			if (input.Sensors.end_button_glass_present == TRUE) {
				vSwitchStateSens(state, GLAS_AT_END);
				state->ptrGeneralState->modules_finished[MODULE_NUMBER - 1] = 0;
				break;
			}
			break;
		case NEW_GLAS:
			if (input.Sensors.start_light_barrier == TRUE && state->lightBarrierStart == 0) {
				state->lightBarrierStart = xTaskGetTickCount();
			}
			if (input.Sensors.start_light_barrier == FALSE && state->lightBarrierStart != 0) {
				state->lightBarrierEnd = xTaskGetTickCount();
			}
			if (state->startTicket + 5000 >= xTaskGetTickCount() && state->lightBarrierEnd >= state->lightBarrierStart + WAIT_TIME) {
				state->ptrGeneralState->modules_finished[MODULE_NUMBER -1] = 1;
				vSwitchStateSens(state, ACTIVE_SENS);
			}
			break;
		default:
			break;
		}
	
		return;
}

void vSwitchStateSens(Module_State_5_Sensors_t* state, int state_new)
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

bool safetyCheck(InputValues_t input) {
	if(input.Sensors.end_doors_open == TRUE || input.Sensors.end_light_barrier == FALSE
			|| input.Sensors.start_doors_open == TRUE || input.Sensors.start_light_barrier == FALSE) {
		return FALSE;
	}
	return TRUE;
}
}
//vHilfsfuntion2() {  }



