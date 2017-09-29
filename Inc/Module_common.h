#ifndef _MODULE_COMMON_H_
#define _MODULE_COMMON_H_

#include "Evaluation.h"



/**Erweiterbare Enumeration f�r verschiedene Fehlertypen
 *Aber gleich für alle Module
 *Die Modulbezeichnung wird extra übermittelt
 */
enum ErrorTypes {MOTOR1_NOT_MOVING,
				MOTOR2_NOT_MOVING,
				FAULTY_SENSOR_VALUE,
				BAD_POSITION,
				DOOR_OPEN,
				};

enum CriticalErrorTypes{CritError1};

enum WarningTypes {ICE_1_EMPTY,
					ICE_2_EMPTY};

enum OperationMode_t SetOperationMode(enum OperationMode_t);

void vInit_Module_common(SystemState_t*);

//Diese Funktion �bernimmt das globale Fehlerhandling
//Sie setzt aber nur gewisse Flags und macht Prints, die lokale Behandlung erfolgt in den Modulen selbst
void ThrowErrorCritical(int ,enum CriticalErrorTypes );
void ThrowError(int ,enum ErrorTypes );
void ThrowWarning(int ,enum WarningTypes );

void ClearErrorCritical(int, enum CriticalErrorTypes);
void ClearError(int, enum ErrorTypes);
void ClearWarning(int, enum WarningTypes);

int isSetCritical(int, enum CriticalErrorTypes);
int isSetError(int,enum ErrorTypes);
int isSetWarning(int,enum WarningTypes);




	
#endif


