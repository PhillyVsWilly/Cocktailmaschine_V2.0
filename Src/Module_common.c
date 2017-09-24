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
 * Für alle Fehler die zum sofortigen Notstopp der Maschine führen müssen
 */
void ThrowErrorCritical(int module, CriticalErrorTypes error)
{
	SetOperationMode(stop);
}

/**
 * Für alle Fehler die zum Sofortstopp eines Moduls führen
 */
void ThrowError(int module, ErrorTypes error)
{
	SetOperationMode(repair);
}

/**
 * Für alle Warnungen/Fehler die das Modul den Vorgang noch abschließen lassen,
 * es dann aber in den Wartungsmodus übergehen lassen
 */
void ThrowWarning(int module, WarningTypes warning)
{

}

