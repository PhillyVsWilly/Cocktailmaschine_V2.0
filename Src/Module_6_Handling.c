#include "Module_6_Handling.h"
#include "Evaluation.h"
#include "linked_list.h"

SystemState_t* ptr_system_state;

typedef struct {
	ingredient_t ingredient;
	int module;
}module_ingredient_t;

void vInit_Module_6_Handling(SystemState_t* SystemState)
{
	//Nicht ädern, muss so sein!
	ptr_system_state = SystemState;

	init_drink_lists();

	// Hier können jetzt noch - falls nötig - Startwerte für die anderen Zustandsvariablen gegeben werden
}


void vEvaluate_Module_6_Handling(InputValues_t input, Module_State_6_Handling_t* state, OutputValues_t* output)
{
	return;
}

void init_drink_lists()
{
	/*
	 * TODO Olaf
	 * Bitte hier die Drinklisten mit leeren Einträgen in folgender Reihenfolge vorinitialisieren:
	 * Eis: 1 Leerplatz
	 * Schwerkraft: 2 Leerplätze
	 * Pumpen: 3
	 * Einschenken: 4
	 * Die Listen bekommst du über ptr_system_state
	 * Z.B. list_append(ptr_system_state->Gravity.drink_list
	 */

	//Leerer Eintrag, kann wiederverwendet werden.
	ingredient_t empty;
	empty.amount = -1;
	empty.bottleID = 0;
	empty.lastInstruction = TRUE;

	//Hinzufügen eines leeren Eintrags, wiederholen für mehrere leere Einträge
	//list_append(*(ptr_system_state->Ice.drinkList), empty);
}


void packet_handler(char* ptr_packet_stack, int len_packet_stack)
{
	//TODO Olaf: packet handler
	/*Diese Funktion wird vom TCP-Prozess ausgeführt, wenn ein (oder mehrere) neue Pakete angekommen ist/sind.
	 *Die Funktion bekommt einen Pointer auf den Beginn des ersten Pakets und die Größe aller Pakete (also z.B. 150, wenn
	 *3 Pakete à 50 Byte angekommen sind).
	 *Auf Basis dieser Daten liest du bitte die Pakete aus und ruft je Pakettyp die entsprechende Funktion auf.
	 */
}


void packet_handler_type_1(char* ptr_packet)
{
	//TODO Olaf
	/*
	 * Diese Funktion behandelt die Pakete vom Pakettyp 1
	 * 1. Loope durch die 48 Byte (ab Byte 3 bis Byte 50) und lies die 12 Zutaten aus. Falls keine 12 Zutaten definiert sind,
	 * sind die restlichen Bytes = 0.
	 * Speichere sie in einem Array vom Typ module_ingredient_t[12] (Definition siehe ganz oben) ab.
	 * 2. Sortiere das Array nach Modulen
	 * 3. Hänge die Zutaten an die entsprechenden Listen an
	 * Entgegen meiner Aussage musst du bei Dopplungen keine leeren Einträge generieren. Die Implementierung hat hierfür
	 * die Variable lastInstruction bei dem Datentyp der Zutaten erstellt. Du kannst die doppelten Zutaten einfach nacheinander
	 * an die Liste anhängen, wobei du bei allen lastIntstruction=FALSE und nur beim letzen =TRUE machst.
	 * Prüfe einfach, ob die folgende Zutat die gleiche Modulnummer hat und setze lastInstruction dementsprechend
	 * 4. Hänge an jedes nicht benutzte Modul einen leeren Eintrag an.
	 *
	 * Das anhängen an die Listen geschieht mit der Funktion list_append, welche einen pointer auf den die Liste
	 * und einen Eintrag des Datentypes ingredient_t (siehe Linked_List.h) bekommt.
	 */
}


void packet_handler_type_2()
{
	//TODO Olaf
	/*
	 * Checke, wie du die Anzahl der aktuell in Bearbeitung und ausstehenden Cocktails aus den Drink-Listen auslesen kannst
	 *
	 */

	//Schnüren des Pakets (musst du nicht mehr verändern)
	/*ptr_packet = malloc(2+2*sizeof(int));
	*(ptr_packet+2) = inBearbeitung;
	*(ptr_packet+2+sizeof(int)) = imSpeicher;*/

	//TODO Philipp
	//packet an TCP weitergeben
}

void packet_handler_type_5()
{
	//TODO Olaf
	//Diese Pakete enthalten einen gewünschten Systemzustand.
	//Diesen kannst du mehr oder weniger (einfach bei allem mal kurz drüber nachdenken, wann wir es nicht machen sollten)
	//in system_state->General.operation_mode übernehmen.

}



