#include "Module_common.h"
#include "Module_1_Transportation.h"
#include "FreeRTOS.h"

#include "Sensors.h"
#include "Evaluation.h"
#include "Actuators.h"

#undef DEBUG_ENABLED
#define DEBUG_ENABLED 1
#include "Debug.h"


#define MODULE_NUMBER 1


/** @brief Initialisierung des Teilmoduls
 *
 * Hier werden die Anfangswerte der Zust舅de und der Variablen eingestellt. Die Funktion wird nur ein einziges
 * Mal beim Systemstart ausgefuehrt
**/
void vInit_Module_1_Transport(Module_State_1_Transportation_t* state, State_General_t* ptrGeneralState)
{
	//Nicht ändern, muss so sein!
	state->state = REFERENCE_TRANS;
	state->ptrGeneralState = ptrGeneralState;
	state->startTicket = NULL;
	state->glassCount = 0;

	//Statisch eingefügt für das Handlingmodul da es atm nichts macht um Fehler aufgrund fehlendem Eintrag zu verhindern.
	state->ptrGeneralState->modules_finished[6] = 1;

	// Hier können jetzt noch - falls nötig - Startwerte für die anderen Zustandsvariablen gegeben werden
}

/** @brief Prüfe nach allgmeinen Fehlern
 *Diese Funktion muss bei jedem Zyklus ausgeführt werden. In ihr werden Systemzustände überprüft, die unabhängig vom
 *Ablauf des Systems sind.
 *Bsp: Beim Transportmodul: Keine Hand in der Maschine
 *Beim Eis: Überlaufbecken nicht voll
 *Deshalb wird ihr auch nicht der Systemzustand übergeben
 **/
static int vCheckForGeneralErrors(InputValues_t input)
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

/** @brief Zentrale Ablaufsteuerung des Moduls
 *
 * Dieses Modul wird in jedem Zyklus aufgerufen und steuert das Modul
 * In ihr wird zuerst vCheckForErrors aufgerufen um zuerst nach allgemeinen Fehlern zu suchen
 * Danach wird in Abhängigkeit des Modulzustands state->state und des Betriebszustands *(state->ptrGeneralState) eine
 * oder mehrere bestimmte Aktionen ausgeführt und deren Verlaufüberwacht
 * Hier knen �ber ThrowError() auch weitere Fehler geworfen werden.
 * Soll der Modulzustand gewechselt werden, wird die vSwitchStateTrans() Funktion benutzt. Diese pr�ft die generelle Zulässigkeit
 * (falls nötig) des Zustandswechsels und schreibt einen Debug-Print.
 **/
void vEvaluate_Module_1_Transportation(InputValues_t input, Module_State_1_Transportation_t* state, OutputValues_t* output)
{
	//Ändern des Status auf Basis des Gesamtmaschinenzustand
		if (state->ptrGeneralState->operation_mode == stop)
		{
			vSwitchStateTrans(state, INACTIVE_TRANS);
		}

		//Ausführen von Funktionen basierend auf dem Zustand
		switch (state->state){
			case INACTIVE_TRANS:
				output->Transport.fullStop = TRUE;
				if (state->ptrGeneralState->operation_mode != stop) {
					vSwitchStateTrans(state, REFERENCE_TRANS);
				}
				break;
			case REFERENCE_TRANS:
				//Checks if the belt should be stopped for a new Glass or if the belt can start and switches states
				DPRINT_MESSAGE("I'm in State %d\n", state->state);
				output->Transport.LED_Status = TRUE;
				for(int i = 0; i < 7; i++) {
					if(state->ptrGeneralState->modules_finished[i] == 0 || state->ptrGeneralState->safety_sensors[i] == 0) {
						break;
					}
				}
				output->Transport.startUp = TRUE;
				output.Transport.windDown = FALSE;
				vSwitchStateTrans(state, ACTIVE_TRANS);
				break;
			case ACTIVE_TRANS:
				//Do something
				for (int i = 0; i < 7; i++) {
					if(state->ptrGeneralState->safety_sensors[i] == 0) {
						output->Transport.startUp = FALSE;
						output->Transport.fullStop = TRUE;
						vSwitchStateTrans(state, STOPP_TRANS);
						break;
					}
				}
				for (int i = 0; i < 7; i++) {
					if(state->ptrGeneralState->modules_finished[i] == 0) {
						output->Transport.windDown = TRUE;
						output->Transport.startUp = FALSE;
						vSwitchStateTrans(state, STOPP_TRANS);
						break;
					}
				}

				DPRINT_MESSAGE("I'm in State %d\n", state->state);
				break;
			case STOPP_TRANS:
				if (input.Transportation.start == TRUE) {
					output->Transport.startUp = TRUE;
					output->Transport.windDown = FALSE;
					vSwitchStateTrans(state, ACTIVE_TRANS);
				}
				break;
			case WAITING_TRANS: //State for waiting for a new Glass.
				DPRINT_MESSAGE("I'm in State %d\n", state->state);
				if (state->startTicket == 0) {
					state->startTicket = xTaskGetTickCount();
					output->Transport.LED_Status = TRUE;
				}
				TickType_t currentTickets = xTaskGetTickCount();
				if (state->startTicket + 5000 <= currentTickets && state->transportCanStart == TRUE) {
					state->startTicket = 0;
					output->Transport.startUp = TRUE;
					output->Transport.windDown = FALSE;
					output->Transport.LED_Status = FALSE;
					input.Transportation.newGlas = FALSE;
					vSwitchStateTrans(state, ACTIVE_TRANS);
				}
				break;
			default:
				break;
			}

			return;
}

void vSwitchStateTrans(Module_State_1_Transportation_t* state, int state_new)
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

