#include "Module_6_Handling.h"
#include "Evaluation.h"
#include "linked_list.h"
#include <stdlib.h>

SystemState_t* ptr_system_state;

typedef struct {
	ingredient_t ingredient;
	int module;
}module_ingredient_t;

void vInit_Module_6_Handling(SystemState_t* SystemState)
{
	//Nicht Ã¤dern, muss so sein!
	ptr_system_state = SystemState;

	init_drink_lists();

	// Hier kÃ¶nnen jetzt noch - falls nÃ¶tig - Startwerte fÃ¼r die anderen Zustandsvariablen gegeben werden
}


void vEvaluate_Module_6_Handling(InputValues_t input, Module_State_6_Handling_t* state, OutputValues_t* output)
{
	return;
}

void init_drink_lists()
{
	/*
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

	list_append(&(ptr_system_state->Ice.drinkList), empty);
	for (int i = 0; i<2; i++) {
		list_append((&ptr_system_state->Gravity.drinkList), empty);
	}
	for (int i = 0; i<3; i++) {
		list_append((&ptr_system_state->Pumping.drinkList), empty);
	}
	for (int i = 0; i<4; i++) {
		list_append((&ptr_system_state->Pouring.drinkList), empty);
	}

	//Hinzufügen eines leeren Eintrags, wiederholen für mehrere leere Einträge
	//list_append(*(ptr_system_state->Ice.drinkList), empty);
}


void packet_handler(char* ptr_packet_stack, int len_packet_stack)
{
	//packet handler
	/*Diese Funktion wird vom TCP-Prozess ausgefÃ¼hrt, wenn ein (oder mehrere) neue Pakete angekommen ist/sind.
	*Die Funktion bekommt einen Pointer auf den Beginn des ersten Pakets und die GrÃ¶ÃŸe aller Pakete (also z.B. 150, wenn
	*3 Pakete Ã  50 Byte angekommen sind).
	*Auf Basis dieser Daten liest du bitte die Pakete aus und rufst je Pakettyp die entsprechende Funktion auf.
	*/

	int still_to_do = len_packet_stack;

	// list_for_each(linked_list *list, listIterator iterator);

	const int header_length = 2;

	while (still_to_do != 0) {
		// first value displays the package's length
		switch (*ptr_packet_stack) {
		case 48: packet_handler_type_1(ptr_packet_stack);	// 36 + 12 (da uint_16 2 Byte?!)
			still_to_do = still_to_do - 48 - header_length;	// 2 = length of header
			ptr_packet_stack = ptr_packet_stack + 48 + header_length;
			break;
		case 0: packet_handler_type_2(ptr_packet_stack);	// keine Nutzdaten im Paket 2
			still_to_do = still_to_do - 0 - header_length;
			ptr_packet_stack = ptr_packet_stack + 0 + header_length;
			break;
		case 5: packet_handler_type_5(ptr_packet_stack);	// 1 + 4 Bytes
					still_to_do = still_to_do - 5 - header_length;
					ptr_packet_stack = ptr_packet_stack + 5 + header_length;
					break;
		default: break;
		}
	}

}

void packet_handler_type_1(char* ptr_packet)
{
   /*
	* Diese Funktion behandelt die Pakete vom Pakettyp 1
	* 1. Loope durch die 48 Byte (ab Byte 3 bis Byte 50) und lies die 12 Zutaten aus. Falls keine 12 Zutaten definiert sind,
	* sind die restlichen Bytes = 0.
	* Speichere sie in einem Array vom Typ module_ingredient_t[12] (Definition siehe ganz oben) ab.
	* 2. Sortiere das Array nach Modulen
	* 		Bubble Sort reicht...
	* 3. HÃ¤nge die Zutaten an die entsprechenden Listen an
	* Entgegen meiner Aussage musst du bei Dopplungen keine leeren EintrÃ¤ge generieren. Die Implementierung hat hierfÃ¼r
	* die Variable lastInstruction bei dem Datentyp der Zutaten erstellt. Du kannst die doppelten Zutaten einfach nacheinander
	* an die Liste anhÃ¤ngen, wobei du bei allen lastIntstruction=FALSE und nur beim letzen =TRUE machst.
	* PrÃ¼fe einfach, ob die folgende Zutat die gleiche Modulnummer hat und setze lastInstruction dementsprechend
	* 4. HÃ¤nge an jedes nicht benutzte Modul einen leeren Eintrag an.
	* 		Definition leerer Eintrag siehe oben
	*
	* Das AnhÃ¤ngen an die Listen geschieht mit der Funktion list_append, welche einen pointer auf den die Liste
	* und einen Eintrag des Datentypes ingredient_t (siehe Linked_List.h) bekommt.
	*/


// 1. Pakete speichern
	module_ingredient_t module_ingredient[12];
	for (int i = 0; i < 12; i += 4) {
		if (*(ptr_packet + i) != 0) {
			module_ingredient[i].module = *(ptr_packet + i);
			module_ingredient[i].ingredient.bottleID = *(ptr_packet + i + 1);
			module_ingredient[i].ingredient.amount = *(ptr_packet + i + 2);
		}
	}

	// 2. BubbleSort implementieren

	for (int j = 0; j < 12; j++) {
		for (int i = 0; i < 12 - j; i++) {
			if (module_ingredient[i + 1].module < module_ingredient[i].module) {
				int save = module_ingredient[i + 1].module;
				module_ingredient[i + 1].module = module_ingredient[i].module;
				module_ingredient[i].module = save;
			}
		}
	}

	// 3. Zutaten an Liste anhängen

	// Leeres Element
	ingredient_t empty;
	empty.amount = -1;
	empty.bottleID = 0;
	empty.lastInstruction = TRUE;

	/*typedef struct {
	int bottleID;
	float amount;
	listbool lastInstruction;
	}ingredient_t;
	*/
	for (int i = 0; i < 12; i++) {
		ingredient_t ingredient;
		ingredient.amount = module_ingredient[i].ingredient.amount;
		ingredient.bottleID = module_ingredient[i].ingredient.bottleID;

		ingredient.lastInstruction = TRUE; // standardmäßig die letzte, nur bei mehreren Einträgen pro Modul Änderung

											  // Überprüfen, ob gleiche Modulnummer nacheinander -> letztes gleiches Modul ist TRUE
		if (i < 11 && module_ingredient[i].module == module_ingredient[i + 1].module) {
			ingredient.lastInstruction = FALSE;
		}

		typedef struct {
			int anzahl;
			int module;
		}anzahl_zutaten_t;


		// Init
		anzahl_zutaten_t anzahl_zutaten[4];
		for (int i = 0; i<4; i++) { anzahl_zutaten[i].anzahl = 0; }
		anzahl_zutaten[0].module = 2;
		anzahl_zutaten[1].module = 3;
		anzahl_zutaten[2].module = 4;
		anzahl_zutaten[3].module = 7;

		// TODO Philipp -> Olaf: Reihenfolge der Module. Hier gemäß Modularchitektur, nicht gemäß Reihenfolge in Cocktailmaschine!
		// TODO Olaf: Problemstellung: Erkennen, wie viele Zutaten maximal pro Modul. empty-Modul bei den Modulen zuweisen, wo es weniger sind.
		switch (module_ingredient[i].module) {
		case 2:
			list_append(&ptr_system_state->Gravity.drinkList, ingredient);
			anzahl_zutaten[0].anzahl += 1;
			break;
		case 3:
			list_append(&ptr_system_state->Pumping.drinkList, ingredient);
			anzahl_zutaten[1].anzahl += 1;
			break;
		case 4:
			list_append(&ptr_system_state->Pouring.drinkList, ingredient);
			anzahl_zutaten[2].anzahl += 1;
			break;
		case 7:
			list_append(&ptr_system_state->Ice.drinkList, ingredient);
			anzahl_zutaten[3].anzahl += 1;
			break;
		default:
			break;
		}

		// max. Anzahl bestimmen: aufsteigend sortieren
		for (int j = 0; j < 4; j++) {
			for (int i = 0; i < 4 - j; i++) {
				if (anzahl_zutaten[i + 1].anzahl < anzahl_zutaten[i].anzahl) {
					anzahl_zutaten_t save = anzahl_zutaten[i + 1];
					anzahl_zutaten[i + 1] = anzahl_zutaten[i];
					anzahl_zutaten[i] = save;
				}
			}
		}
		int max_anzahl_zutaten = anzahl_zutaten[3].anzahl;

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < (max_anzahl_zutaten - anzahl_zutaten[i].anzahl); j++) {
				switch (anzahl_zutaten[i].module) {
				case 2:	list_append(&ptr_system_state->Gravity.drinkList, empty);
					break;
				case 3:	list_append(&ptr_system_state->Pumping.drinkList, empty);
					break;
				case 4:	list_append(&ptr_system_state->Pouring.drinkList, empty);
					break;
				case 7:	list_append(&ptr_system_state->Ice.drinkList, empty);
					break;
				default: break;
				}
			}

		}


	}

}


void packet_handler_type_2()
{



	//	ptr_system_state->Pumping.drink_list

	// TODO Philipp->Olaf : in Bearbeitung: geht nicht -> nur für nächsten Schritt, da in Liste jeweils die Zutaten für den nächsten Schritt stehen und bei Beginn der Ausführung rausgelöscht werden, oder ????
	int anzahl_inBearbeitung;
	int anzahl_ausstehend[4]; // 0: Gravity, 1: Pumping, 2: Pouring, 3: Ice

							  // Ausstehende: Alle Trinklisten durchgehen -> jeweils aufaddieren, wenn lastInstruction = TRUE
	for (int i = 0; i < list_size(&ptr_system_state->Gravity.drinkList); i++) {
		if (ptr_system_state->Gravity.drinkList.head->ingredient.lastInstruction == TRUE) //TODO Philipp->Olaf: Richtig ausgelesen? -> oder mit list_for_each!!
			anzahl_ausstehend[0]++;
		if (ptr_system_state->Pumping.drinkList.head->ingredient.lastInstruction == TRUE)
			anzahl_ausstehend[1]++;
		if (ptr_system_state->Pouring.drinkList.head->ingredient.lastInstruction == TRUE)
			anzahl_ausstehend[2]++;
		if (ptr_system_state->Ice.drinkList.head->ingredient.lastInstruction == TRUE)
			anzahl_ausstehend[3]++;
	}

	// als nächstes in Bearbeitung:
	anzahl_inBearbeitung = 0;
	if(ptr_system_state->Gravity.drinkList.head->ingredient.bottleID == -1)
	{
		anzahl_inBearbeitung++;
	}
	if(ptr_system_state->Pumping.drinkList.head->ingredient.bottleID == -1)
	{
		anzahl_inBearbeitung++;
	}
	if(ptr_system_state->Pouring.drinkList.head->ingredient.bottleID == -1)
	{
		anzahl_inBearbeitung++;
	}
	if(ptr_system_state->Ice.drinkList.head->ingredient.bottleID == -1)
	{
		anzahl_inBearbeitung++;
	}

	//Schnüren des Pakets (musst du nicht mehr verÃ¤ndern)
	char* ptr_packet;
	ptr_packet = (char*)malloc(2 + 2 * sizeof(int));
	*(ptr_packet + 2) = anzahl_inBearbeitung;
	*(ptr_packet + 2 + sizeof(int)) = anzahl_ausstehend[0];

	//TODO packet an TCP weitergeben
}

void packet_handler_type_5()
{
	//TODO packet handler type 5
	//Diese Pakete enthalten einen gewünschten Systemzustand.
	//Diesen kannst du mehr oder weniger (einfach bei allem mal kurz drüber nachdenken, wann wir es nicht machen sollten)
	//in system_state->General.operation_mode übernehmen.
	// Philipp->Olaf Eine Weiterleitung des Systemzustand-Befehls ist derzeit nicht an die einzelnen Module möglich, nur als General State, richtig?
	// Würde es nicht auch Sinn machen, dass die Module einzeln Hinweise bekommen oder ist das nicht notwendig?
	// Jeder Ausfall eines Moduls führt zu einem Stopp der gesamten Anlage. Außer man untersucht jeden einzelnen Cocktail danach, ob das aussetzende Modul dafür überhaupt benötigt wird.

}



