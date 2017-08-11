#ifndef _MODULE_COMMON_H_
#define _MODULE_COMMON_H_

#define DEBUG_ENABLED 1
#include "Debug.h"

//Erweiterbare Enumeration f�r verschiedene Fehlertypen
enum ErrorTypes {MOTOR1_NOT_MOVING, MOTOR2_NOT_MOVING, FAULTY_SENSOR_VALUE, BAD_POSITION, DOOR_OPEN};
enum Service {Service};

//Diese Funktion �bernimmt das globale Fehlerhandling
//Sie setzt aber nur gewisse Flags und macht Prints, die lokale Behandlung erfolgt in den Modulen selbst
inline void ThrowError(int module, int error)
{
	DPRINT_ERROR("Error %d in Module %d\r\n", error, module);
	
	//Berechne die Folgen bestimmter Fehler 
	return;
}
	
#endif
