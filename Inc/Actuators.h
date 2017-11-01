#ifndef ACUATORS_H_
#define ACUATORS_H_

#include <stdint.h>
#include "Sensors.h"

typedef unsigned char uint_8; 



typedef struct {
	int move_baum;
	int move_platform;
	int pwm_baum;
	int pwm_platform;
	int stepper_a;
	int stepper_b;
}Gravity_t;

typedef struct {
	int choose_motor;
	int pump;
	bool signal_sensor; // Wenn Ursprung erreicht, Signal an Sensor pos_start.
	int pwm_choose;
	int pwm_pump;
}Pumping_t;

typedef struct {
	int motor;
	bool signal_sensor; // Wenn Ursprung erreicht, Signal an Sensor pos_start.
	int pwm;
}Pouring_t;


//Muss noch Hardwaremaessig umgesetzt werden, hier als Platzhalter fuer was wir brauchen
typedef struct {
	int motor;
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
	int motor;
	float motorAngle; //Winkelmesser am Motor zum feststellen ob er sich dreht oder feststeckt
	int pwm;
}Ice_t;

typedef struct {
	int led_cocktail_chosen;
}Sensors_t;

//Define the Type where all output values will be stored later
typedef struct {
	uint_8 LED2;
	Gravity_t Gravity;
	Pumping_t Pumping;
	Pouring_t Pouring;
	Sensors_t Sensors;
	Transport_t Transport;
	Ice_t Ice;
} OutputValues_t;


void vInitActuatorValues(OutputValues_t*);
void vWriteActuatorValues(OutputValues_t*);

void getPWM(int, int, int, int);
void setPWM(int, uint16_t);

void vModule_1_ActuatorValues(Transport_t*);
void vModule_2_ActuatorValues(Gravity_t*);
void vModule_3_ActuatorValues(Pumping_t*);
void vModule_4_ActuatorValues(Pouring_t*);
void vModule_5_ActuatorValues(Sensors_t*);
void vModule_7_ActuatorValues(Ice_t*);

#endif
