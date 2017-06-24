#include "Module_common.h"
#include "Module_3_Pumping.h"

#include "Sensors.h"
#include "Evaluation.h"
#include "Actuators.h"

#undef DEBUG_ENABLED
#define DEBUG_ENABLED 1
#include "Debug.h"

//Hier steht die Modulnummer. Sie wird im Code nicht hart gecodet, sondern nur hier eingetragen
//Im Code wird sie dann mit dem Namen MODULE_NUMBER verwendet
#define MODULE_NUMBER 3


/** @brief Initialisierung des Teilmoduls
 *
 * Hier werden die Anfangswerte der Zustände und der Variablen eingestellt. Die Funktion wird nur ein einziges
 * Mal beim Systemstart ausgeführt
**/
void vInit_Module_3_Pumping(Module_State_3_Pumping_t* state, State_General_t* ptrGeneralState)
{
	//Nicht ändern, muss so sein!
	state->state = REFERENCE;
	state->ptrGeneralState = ptrGeneralState;

	// Hier können jetzt noch - falls nötig - Startwerte für die anderen Zustandsvariablen gegeben werden
}

/** @brief Prüfe nach allgmeinen Fehlern
 *Diese Funktion muss bei jedem Zyklus ausgeführt werden. In ihr werden Systemzustände überprüft, die unabhängig vom
 *Ablauf des Systems sind.
 *Bsp: Beim Transportmodul: Keine Hand in der Maschine
 *Beim Eis: Überlaufbecken nicht voll
 *Deshalb wird ihr auch nicht der Systemzustand übergeben
 **/
int vCheckForGeneralErrors(InputValues_t input)
{
	//TODO: implementiere Fehlererkennung
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
 * oder mehrere bestimmte Aktionen ausgeführt und deren Verlauf überwacht
 * Hier können über ThrowError() auch weitere Fehler geworfen werden.
 * Soll der Modulzustand gewechselt werden, wird die vSwitchState() Funktion benutzt. Diese prüft die generelle Zulässigkeit
 * (falls nötig) des Zustandswechsels und schreibt einen Debug-Print.
 **/
void vEvaluate_Module_3_Pumping(InputValues_t input, Module_State_3_Pumping_t* state, OutputValues_t* output)
{
	//Ändern des Status auf Basis des Gesamtmaschinenzustand
	if (&state->ptrGeneralState == stop)
	{
		vSwitchState(state, INACTIVE_PUMP);
	}
	
	//Ausführen von Funktionen basierend auf dem Zustand
	switch (state->state){
		case INACTIVE:
			//stops the Motors and
			output -> Pumping.pump = 0;
			output -> Pumping.choose_motor = 0;

			DPRINT_MESSAGE("I'm in State %d\n", state->state);
			if(&state->ptrGeneralState != stop)
			{
				vSwitchState(state, REFERENCE_PUMP);
			}
			break;
		case REFERENCE_PUMP:
			output -> Pumping.pump = 0;
			DPRINT_MESSAGE("I'm in State %d\n", state-> state);
			if(input ->Pumping.valve_position == -1){
				output -> Pumping.choose_motor = -1;
			}
			else{
				output -> Pumping.choose_motor = 0;
				state -> valveInTransit = FALSE;
				vSwitchState(state, ACTIVE);

			}
			DPRINT_MESSAGE("The pump output is %d\nThe choose valve output is %d\n",output ->Pumping.pump,output ->Pumping.choose_motor);

			break;
		case ACTIVE:

			DPRINT_MESSAGE("I'm in State %d\n",state ->state);


			if(state->glassInStation && state->drinkQueue.front != NULL){
				vSwitchState(state, VALVE_ADJUSTING);
				input.Sensors.modules_finished[3] = 0;
			}

			break;

		case VALVE_ADJUSTING:
			if(input.Pumping.valve_position < state.drinkQueue.front[1] && !state.valveInTransit ){
				output -> Pumping.choose_motor = 1;
				state->valveInTransit = TRUE;
			}
			if(input.Pumping.valve_position > state.drinkQueue.front[1] && !state.valveInTransit ){
							output -> Pumping.choose_motor = -1;
							state->valveInTransit = TRUE;
						}
			if(input.Pumping.valve_position = state.drinkQueue.front[1] ){
				output -> Pumping.choose_motor = 0;
				state->valveInTransit = FALSE;
				vSwitchState(state, PUMP_ACTIVE);
			}
		case PUMP_ACTIVE:
			if(input.Pumping.weight_glass < state->drinkQueue.front[2]){
				output -> Pumping.pump = 1;
			}
			else{
				output -> Pumping.pump = 0;
				bool cont = (1 == state->drinkQueue.front[3]);
				Dequeue(state->drinkQueue);
				if (cont){
					vSwitchState(state,FILLED_GLASS);
				}
				else{
					vSwitchState(state,ACTIVE);
				}
			}
		case FILLED_GLASS:
			if(input.Pouring.weight == 0){
				vSwitchState(state,ACTIVE);
			}
			input.Sensors.modules_finished[3] = 1;

		default:
			break;
		}
	
		return;

}

void vSwitchState(Module_State_3_Pumping_t* state, int state_new)
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


