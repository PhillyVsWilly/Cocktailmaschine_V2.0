#include "Module_common.h"
#include "Module_3_Pumping.h"

#undef DEBUG_ENABLED
#define DEBUG_ENABLED 1
#include "Debug.h"

//Hier steht die Modulnummer. Sie wird im Code nicht hart gecodet, sondern nur hier eingetragen
//Im Code wird sie dann mit dem Namen MODULE_NUMBER verwendet
#define MODULE_NUMBER 3


/** @brief Initialisierung des Teilmoduls
 *
 * Hier werden die Anfangswerte der Zust舅de und der Variablen eingestellt. Die Funktion wird nur ein einziges
 * Mal beim Systemstart ausgef�hrt
**/
void vInit_Module_3_Pumping(Module_State_3_Pumping_t* state, State_General_t* ptrGeneralState)
{
	//Nicht 舅dern, muss so sein!
	state->state = REFERENCE_PUMP;
	state->ptrGeneralState = ptrGeneralState;

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
	//TODO: implementiere Fehlererkennung
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
 * Soll der Modulzustand gewechselt werden, wird die vSwitchStatePump() Funktion benutzt. Diese pr�ft die generelle Zul舖sigkeit
 * (falls nig) des Zustandswechsels und schreibt einen Debug-Print.
 **/
void vEvaluate_Module_3_Pumping(InputValues_t input, Module_State_3_Pumping_t* state, OutputValues_t* output)
{
	//ﾄndern des Status auf Basis des Gesamtmaschinenzustand
	if (state->ptrGeneralState->operation_mode == stop)
	{
		vSwitchStatePump(state, INACTIVE_PUMP);
	}
	//Ausf�hren von Funktionen basierend auf dem Zustand
	switch (state->state){
		case INACTIVE_PUMP:
			//stops the Motors and
			output -> Pumping.pump = 0;
			output -> Pumping.choose_motor = 0;

			DPRINT_MESSAGE("I'm in State %d\n", state->state);
			if(state->ptrGeneralState->operation_mode!= stop)
			{
				vSwitchStatePump(state, REFERENCE_PUMP);
			}
			break;
		case REFERENCE_PUMP:
			output -> Pumping.pump = 0;
			DPRINT_MESSAGE("I'm in State %d\n", state-> state);
			if(input.Pumping.valve_position == -1){
				output -> Pumping.choose_motor = -1;
			}
			else{
				output -> Pumping.choose_motor = 0;
				state -> valveInTransit = FALSE;
				vSwitchStatePump(state, ACTIVE);

			}
			DPRINT_MESSAGE("The pump output is %d\nThe choose valve output is %d\n",output ->Pumping.pump,output ->Pumping.choose_motor);

			break;
		case ACTIVE:

			DPRINT_MESSAGE("I'm in State %d\n",state ->state);

			listNode *ls_head;
			//TODO list_head(input.Pumping.drinkList, ls_head, FALSE);

			if(state->glassInStation && ls_head != NULL){
				vSwitchStatePump(state, VALVE_ADJUSTING);
				//TODO input.Sensors.modules_finished[3] = 0;
			}

			break;

		case VALVE_ADJUSTING:

			//TODO list_head(input.Pumping.drinkList, ls_head, FALSE);
			if(input.Pumping.valve_position < ls_head->data[0] && !state->valveInTransit ){
				output -> Pumping.choose_motor = 1;
				state->valveInTransit = TRUE;
			}
			if(input.Pumping.valve_position > ls_head->data[0] && !state->valveInTransit ){
							output -> Pumping.choose_motor = -1;
							state->valveInTransit = TRUE;
						}
			if(input.Pumping.valve_position == ls_head->data[0] ){
				output -> Pumping.choose_motor = 0;
				state->valveInTransit = FALSE;
				vSwitchStatePump(state, PUMP_ACTIVE);
			}
		case PUMP_ACTIVE:

			//TODO list_head(input.Pumping.drinkList, ls_head, FALSE);
			if(input.Pumping.weight_glass < ls_head->data[1]){
				output -> Pumping.pump = 1;
			}
			else{
				output -> Pumping.pump = 0;
				bool cont = (1 == ls_head->data[2]);
				//TODO list_head(input.Pumping.drinkList, ls_head, TRUE);
				if (cont){
					vSwitchStatePump(state,FILLED_GLASS_PUMP);
				}
				else{
					vSwitchStatePump(state,ACTIVE);
				}
			}
		case FILLED_GLASS_PUMP:
			if(input.Pouring.weight == 0){
				vSwitchStatePump(state,ACTIVE);
			}
			//TODO input.Sensors.modules_finished[3] = 1;

		default:
			break;
		}
	
		return;

}

void vSwitchStatePump(Module_State_3_Pumping_t* state, int state_new)
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


