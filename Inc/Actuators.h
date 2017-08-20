#ifndef ACUATORS_H_
#define ACUATORS_H_

#include <stdint.h>
#include "Sensors.h"

typedef unsigned char uint_8; 



typedef struct {
	uint_8 move_baum;
	uint_8 move_platform;
	int pwm_baum;
	int pwm_platform;
}Gravity_t;

typedef struct {
	uint_8 choose_motor;
	uint_8 pump;
	bool signal_sensor; // Wenn Ursprung erreicht, Signal an Sensor pos_start.
	int pwm_choose;
	int pwm_pump;
}Pumping_t;

typedef struct {
	uint_8 motor;
	bool signal_sensor; // Wenn Ursprung erreicht, Signal an Sensor pos_start.
	int pwm;
}Pouring_t;


//Muss noch Hardwaremaessig umgesetzt werden, hier als Platzhalter fuer was wir brauchen
typedef struct {
	uint_8 motor;
	bool fullStop; //bool indicating transportation should do a full stop
	bool startUp; //bool indicating transportation should start up
	bool windDown; //bool indicating transportation should wind down
	//uint_8 startUp[10]; //Array fuer Werte um langsames Anfahren des Transportbandes zu ermoeglichen
	//uint_8 windDown[10]; //Array fuer Werte um langsames stoppen des Transportbandes zu ermoeglichen
	bool LED_Status; //LED die leuchten soll wenn das Transportband steht, damit man ein neues Glas reinstellen kann
	int pwm;
}Transport_t;

//Muss noch Hardwaremaessig umgesetzt werden, hier als Platzhalter fuer was wir brauchen
typedef struct {
	uint_8 motor;
	float motorAngle; //Winkelmesser am Motor zum feststellen ob er sich dreht oder feststeckt
	int pwm;
}Ice_t;

//Define the Type where all output values will be stored later
typedef struct {
	uint_8 LED2;
	int actuator_transport;
	Gravity_t Gravity;
	Pumping_t Pumping;
	Pouring_t Pouring;
	Transport_t Transport;
	Ice_t Ice;
	int allow_modules[7]; // Anweisung an die Module, ob erlaubt mit Arbeit zu beginnen.
	int placeholder_handling;
} OutputValues_t;


void vInitActuatorValues(OutputValues_t*);
void vWriteActuatorValues(OutputValues_t);

void vAssignMotorValues(OutputValues_t*);

#endif
