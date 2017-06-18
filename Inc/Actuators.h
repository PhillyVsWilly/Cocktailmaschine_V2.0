#ifndef ACUATORS_H_
#define ACUATORS_H_

#include <stdint.h>

typedef unsigned char uint_8; 

typedef struct {
	int placeholder_pump;
	int placeholder_valve;
}Pumping_t;

//Define the Type where all output values will be stored later
typedef struct {
	uint_8 LED2;
	int placeholder_transport;
	int placeholder_gravity;
	Pumping_t Pumping;
	int placeholder_pouring;
	int placeholder_sensors;
	int placeholder_handling;
} OutputValues_t;


void vInitActuatorValues(OutputValues_t*);
void vWriteActuatorValues(OutputValues_t);
#endif
