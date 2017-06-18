/**
 * This file contains the implementations of the functions 
 * debugPrintWarning, debugPrintMessage, debugPrintError
 *
 * @author Ural Bayhan
 */

#include "Debug.h"
#include "String.h"

#ifdef DEBUG_ENABLED

int debugPrintWarning(char* s, ...) {
		va_list arg;
		int done;
		
		va_start (arg, s);
		
		char *a=(char*)malloc(sizeof(char)*(strlen(s) + strlen("Warning: ") + 1));
		snprintf(a, strlen(s) + strlen("Warning: ") + 1, "Warning: %s", s);
		
		done = vfprintf (stdout, a, arg);
		
		va_end (arg);
		free(a);
		
		return done;
}

int debugPrintMessage(char* s, ...) {
		va_list arg;
		int done;
		
		va_start (arg, s);
		
		char *a=(char*)malloc(sizeof(char)*(strlen(s) + strlen("Message: ") + 1));
		snprintf(a, strlen(s) + strlen("Message: ") + 1, "Message: %s", s);
		
		done = vfprintf (stdout, a, arg);
		
		va_end (arg);
		free(a);
		
		return done;
}
int debugPrintError(char* s, ...) {
		va_list arg;
		int done;
		
		va_start (arg, s);
		
		char *a=(char*)malloc(sizeof(char)*(strlen(s) + strlen("Error: ") + 1));
		snprintf(a, strlen(s) + strlen("Error: ") + 1, "Error: %s", s);
		
		done = vfprintf (stdout, a, arg);
		
		va_end (arg);
		free(a);
		
		return done;
}
#endif

