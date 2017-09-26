#ifndef _MODULE_COMMON_H_
#define _MODULE_COMMON_H_

#define DEBUG_ENABLED 1
#include "Debug.h"

/**Erweiterbare Enumeration f�r verschiedene Fehlertypen
 *Aber gleich für alle Module
 *Die Modulbezeichnung wird extra übermittelt
 */
enum ErrorTypes {MOTOR1_NOT_MOVING, MOTOR2_NOT_MOVING, FAULTY_SENSOR_VALUE, BAD_POSITION, DOOR_OPEN};

enum CriticalErrorTypes{};

enum WarningTypes {};

int SetOperationMode(int);


//Diese Funktion �bernimmt das globale Fehlerhandling
//Sie setzt aber nur gewisse Flags und macht Prints, die lokale Behandlung erfolgt in den Modulen selbst
void ThrowErrorCritical(int ,enum CriticalErrorTypes );
void ThrowError(int ,enum ErrorTypes );
void ThrowWarning(int ,enum WarningTypes );

	
#endif


