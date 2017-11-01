#include "Module_common.h"
#include "main.h"
#include "Evaluation.h"

#include "string.h"

#define DPRINTF(...) printf(__VA_ARGS__)

static SystemState_t* ptr_SystemState;

void vInit_Module_common(SystemState_t* pState)
{
	ptr_SystemState=pState;
	memset(ptr_SystemState->General.modules_finished,1,7*sizeof(int));
	memset(ptr_SystemState->General.ErrFlags,0,7*sizeof(int));
	memset(ptr_SystemState->General.CritFlags,0,7*sizeof(int));
	memset(ptr_SystemState->General.WarnFlags,0,7*sizeof(int));
}

enum OperationMode_t SetOperationMode(enum OperationMode_t newMode)
{
	ptr_SystemState->General.operation_mode = newMode;
	return ptr_SystemState->General.operation_mode;
}


/**
 * Für alle Fehler die zum sofortigen Notstopp der Maschine führen müssen
 */
void ThrowErrorCritical(int module, enum CriticalErrorTypes error)
{
	ptr_SystemState->General.CritFlags[module-1] |= (1 << error);
	SetOperationMode(stop);
}

/**
 * Für alle Fehler die zum Sofortstopp eines Moduls führen
 */
void ThrowError(int module, enum ErrorTypes error)
{
	ptr_SystemState->General.ErrFlags[module-1] |= (1 << error);
	SetOperationMode(repair);
}

/**
 * Für alle Warnungen/Fehler die das Modul den Vorgang noch abschließen lassen,
 * es dann aber in den Wartungsmodus übergehen lassen
 */
void ThrowWarning(int module,enum WarningTypes warning)
{
	ptr_SystemState->General.WarnFlags[module-1] |= (1 << warning);
	DPRINTF("Setting Warning %d in Module %d!\n", warning, module);
	DPRINTF("Warning Status: %d", ptr_SystemState->General.WarnFlags[module-1]);
}

void ClearErrorCritical(int module , enum CriticalErrorTypes error)
{
	ptr_SystemState->General.CritFlags[module-1] &= ~(1 << error);
	if (ptr_SystemState->General.CritFlags[module-1] == 0)
	{
		SetOperationMode(normal);
	}
}

void ClearError(int module , enum ErrorTypes error)
{
	ptr_SystemState->General.ErrFlags[module-1] &= ~(1 >> error);
	if (ptr_SystemState->General.ErrFlags[module-1] == 0)
	{
		SetOperationMode(normal);
	}
}
void ClearWarning(int module, enum WarningTypes warning)
{
	ptr_SystemState->General.WarnFlags[module-1] &= ~(1 >> warning);
}

int isSetCritical(int module, enum CriticalErrorTypes bit)
{
	return ptr_SystemState->General.CritFlags[module-1] & (1 << bit);
}

int isSetError(int module, enum ErrorTypes bit)
{
	return ptr_SystemState->General.ErrFlags[module-1] & (1 << bit);
}

int isSetWarning(int module, enum WarningTypes bit)
{
	return ptr_SystemState->General.WarnFlags[module-1] & (1 << bit);
}

