#include "Module_common.h"
#include "main.h"
#include "Evaluation.h"

static SystemState_t* ptr_SystemState;

int initModuleCommon(SystemState_t* pState)
{
	ptr_SystemState=pState;
}

int SetOperationMode(operation_mode newMode)
{
	ptrSystemState->General.operation_mode = newMode;
}


/**
 * F�r alle Fehler die zum sofortigen Notstopp der Maschine f�hren m�ssen
 */
void ThrowErrorCritical(int module, enum CriticalErrorTypes error)
{
	SetOperationMode(stop);
}

/**
 * F�r alle Fehler die zum Sofortstopp eines Moduls f�hren
 */
void ThrowError(int module, enum ErrorTypes error)
{
	SetOperationMode(repair);
}

/**
 * F�r alle Warnungen/Fehler die das Modul den Vorgang noch abschlieﾟen lassen,
 * es dann aber in den Wartungsmodus �bergehen lassen
 */
void ThrowWarning(int module, enum WarningTypes warning)
{

}

