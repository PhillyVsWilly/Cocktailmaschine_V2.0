#ifndef ACUATORS_H_
#define ACUATORS_H_

#include <stdint.h>
#include "Sensors.h"

typedef unsigned char uint_8; 



typedef struct {
	uint_8 move_baum;
	uint_8 move_platform;
}Gravity_t;

typedef struct {
	uint_8 choose_motor;
	uint_8 pump;
	bool signal_sensor; // Wenn Ursprung erreicht, Signal an Sensor pos_start.
}Pumping_t;

typedef struct {
	uint_8 motor;
	bool signal_sensor; // Wenn Ursprung erreicht, Signal an Sensor pos_start.
}Pouring_t;




//Define the Type where all output values will be stored later
typedef struct {
	uint_8 LED2;
	int actuator_transport;
	Gravity_t Gravity;
	Pumping_t Pumping;
	Pouring_t Pouring;
	int allow_modules[7]; // Anweisung an die Module, ob erlaubt mit Arbeit zu beginnen.
	int placeholder_handling;
	int placeholder_ice;
} OutputValues_t;


void vInitActuatorValues(OutputValues_t*);
void vWriteActuatorValues(OutputValues_t);
#endif
