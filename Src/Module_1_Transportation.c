#include "Module_common.h"
#include "Module_1_Transportation.h"
#include "FreeRTOS.h"
#include "task.h"
#include "project_conf.h"

#include "Sensors.h"
#include "Evaluation.h"
#include "Actuators.h"

#undef DEBUG_ENABLED
#define DEBUG_ENABLED 1
#include "Debug.h"

#define MODULE_NUMBER MODULE_ID_TRANSPORT

/** @brief Initialisierung des Teilmoduls
 *
 * Hier werden die Anfangswerte der Zust舅de und der Variablen eingestellt. Die Funktion wird nur ein einziges
 * Mal beim Systemstart ausgefuehrt
 **/
void vInit_Module_1_Transport(Module_State_1_Transportation_t* state,
		State_General_t* ptrGeneralState) {
	//Nicht ndern, muss so sein!
	state->state = REFERENCE_TRANS;
	state->ptrGeneralState = ptrGeneralState;
	state->startTicket = 0;
	state->ptrGeneralState->glassCount = 0;

	//Statisch eingefügt für das Handlingmodul da es atm nichts macht um Fehler aufgrund fehlendem Eintrag zu verhindern.
	state->ptrGeneralState->modules_finished[6] = 1;

	// Hier können jetzt noch - falls nötig - Startwerte für die anderen Zustandsvariablen gegeben werden
}

/** @brief Prüfe nach allgmeinen Fehlern
 *Diese Funktion muss bei jedem Zyklus ausgeführt werden. In ihr werden Systemzustnde überprüft, die unabhngig vom
 *Ablauf des Systems sind.
 *Bsp: Beim Transportmodul: Keine Hand in der Maschine
 *Beim Eis: Überlaufbecken nicht voll
 *Deshalb wird ihr auch nicht der Systemzustand übergeben
 **/
static int vCheckForGeneralErrors(InputValues_t input) {

}

/** @brief Zentrale Ablaufsteuerung des Moduls
 *
 * Dieses Modul wird in jedem Zyklus aufgerufen und steuert das Modul
 * In ihr wird zuerst vCheckForErrors aufgerufen um zuerst nach allgemeinen Fehlern zu suchen
 * Danach wird in Abhngigkeit des Modulzustands state->state und des Betriebszustands *(state->ptrGeneralState) eine
 * oder mehrere bestimmte Aktionen ausgeführt und deren Verlaufüberwacht
 * Hier können über ThrowError() auch weitere Fehler geworfen werden.
 * Soll der Modulzustand gewechselt werden, wird die vSwitchStateTrans() Funktion benutzt. Diese prüft die generelle Zulssigkeit
 * (falls nötig) des Zustandswechsels und schreibt einen Debug-Print.
 **/
void vEvaluate_Module_1_Transportation(InputValues_t input,
		Module_State_1_Transportation_t* state, OutputValues_t* output) {
	//Ändern des Status auf Basis des Gesamtmaschinenzustand
	if (state->ptrGeneralState->operation_mode == stop) {
		vSwitchStateTrans(state, INACTIVE_TRANS);
	}

	bool finished;
	//Ausführen von Funktionen basierend auf dem Zustand
	switch (state->state) {
	case INACTIVE_TRANS:
		output->Transport.windDown = TRUE;
		if (state->ptrGeneralState->operation_mode != stop) {
			vSwitchStateTrans(state, STOPP_TRANS);
		}
		break;

	case ACTIVE_TRANS:

		output->Transport.startUp = TRUE;
		output->Transport.windDown = FALSE;
		finished = TRUE;
		for (int i = 0; i < 7; i++) {
			if (state->ptrGeneralState->modules_finished[i] == 0) {
				finished = FALSE;
			}

		}
		if (!finished) {
			output->Transport.windDown = TRUE;
			output->Transport.startUp = FALSE;
			vSwitchStateTrans(state, STOPP_TRANS);
			break;
		} else {
			vSwitchStateTrans(state, ACTIVE_TRANS);
		}

		DPRINT_MESSAGE("I'm in State %d\n", state->state);
		break;
	case STOPP_TRANS:

		output->Transport.startUp = FALSE;
		output->Transport.windDown = TRUE;
		finished = TRUE;
		for (int i = 0; i < 7; i++) {
			if (state->ptrGeneralState->modules_finished[i] == 0) {
				finished = FALSE;
			}
		}
		if (finished) {
			output->Transport.windDown = FALSE;
			output->Transport.startUp = TRUE;
			vSwitchStateTrans(state, ACTIVE_TRANS);
		} else {
			vSwitchStateTrans(state, STOPP_TRANS);
		}

		break;

	default:
		break;
	}

	return;
}

void vSwitchStateTrans(Module_State_1_Transportation_t* state, int state_new) {
//Hier kommt alles rein, was bei jedem(!) Zustandswechsel passieren soll
	DPRINT_MESSAGE("Switching states from State %d to State %d\r\n",
			state->state, state_new);

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

//vHilfsfuntion1() {  }
//vHilfsfuntion2() {  }
