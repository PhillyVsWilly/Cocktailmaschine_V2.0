#ifndef ACUATORS_H_
#define ACUATORS_H_

#include <stdint.h>
#include "Sensors.h"

typedef unsigned char uint_8; 



typedef struct {
	uint_8 move_baum;
	uint_8 move_platform;
	bool platform_Position_Down; //TRUE wenn plattform unten ist und baum gedreht werden kann
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


//Muss noch Hardwaremaessig umgesetzt werden, hier als Platzhalter fuer was wir brauchen
typedef struct {
	uint_8 motor;
	bool startUp; //Bool um anzuzeigen dass das transportband anfahren soll
	bool windDown; //Bool um anzuzeigen dass das transportband stoppen soll
	bool fullStopp; //Bool für Notstopp (z.B. bei triggern von Sicherheitssensoren)
	bool LED_Status; //LED die leuchten soll wenn das Transportband steht, damit man ein neues Glas reinstellen kann
}Transport_t;

//Muss noch Hardwaremaessig umgesetzt werden, hier als Platzhalter fuer was wir brauchen
typedef struct {
	uint_8 motor;
	float motorAngle; //Winkelmesser am Motor zum feststellen ob er sich dreht oder feststeckt
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
#endif
