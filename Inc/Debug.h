/**
 *	These functions will print the given message just like printf
 *	if DEBUG_ENABLED is 1. They will print the message with keywords
 *	"Warning: ", "Message: ", and "Error: " at the beginning.
 
 *	@author Ural Bayhan
 */

#ifndef __DEBUG_H
#define __DEBUG_H

#ifndef DEBUG_ENABLED
#define DEBUG_ENABLED 1 //Debug value to know if we are in debug
#endif

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>

/*Do not use these Funktions directly!!*/
#if DEBUG_ENABLED

int debugPrintWarning(char* s, ...);
int debugPrintMessage(char* s, ...);
int debugPrintError(char* s, ...);

/*use the following macros instead*/

#define DPRINT_WARNING(...) debugPrintWarning (__VA_ARGS__)
#define DPRINT_MESSAGE(...) debugPrintMessage(__VA_ARGS__)
#define DPRINT_ERROR(...) debugPrintError(__VA_ARGS__)
#else
#define DPRINT_WARNING(...)
#define DPRINT_ERROR(...)
#define DPRINT_MESSAGE(...)
#endif


#endif
