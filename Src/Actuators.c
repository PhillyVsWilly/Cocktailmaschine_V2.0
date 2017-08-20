#include "Actuators.h"

#include "stm32f7xx_nucleo_144.h"

//Rampen in V/s (Steuerspannung)
#define TRANSPORT_RAMP_UP 1
#define TRANSPORT_RAMP_DOWN 1
#define GRAVITY_PLATFORM_RAMP_UP 1
#define GRAVITY_PLATFORM_RAMP_DOWN 1
#define GRAVITY_TREE_RAMP_UP 1
#define GRAVITY_TREE_RAMP_DOWN 1
#define PUMPING_CHOOSER_RAMP_UP 1
#define PUMPING_CHOOSER_RAMP_DOWN 1
#define PUMPING_PUMP_RAMP_UP 1
#define PUMPING_PUMP_RAMP_DOWN 1
#define POURING_RAMP_UP 1
#define POURING_RAMP_DOWN 1
#define ICE_RAMP_UP 1
#define ICE_RAMP_DOWN 1

/*
 * TODO Benedikt: Hier bitte deine Hilfsvariablen (speichern alter Werte ...)
 */


void vInitActuatorValues(OutputValues_t* output_values)
{
	output_values->LED2 = 0;
}

void vWriteActuatorValues(OutputValues_t output)
{

	vAssignMotorValues(output);

	if(output.LED2 == 1){
		BSP_LED_On(LED2);
	} else { 
		BSP_LED_Off(LED2);
	}
}

void vAssignMotorValues(OutputValues_t* output)
{

	/*
	 * TODO Benedikt berechne die richtige PWM Werte für alle 7 Motoren
	 * Du kannst die untenstehende Funktion getPWMValue() zur Hilfe nehmen
	 */

	//output->Transport.pwm = ...;
	//output->Gravity.pwm = ...;
	//usw.


	return;
}

/** Gibt den neuen Wert für die PWM zurück
 *
 *  Die Funktion prüft zuerst, ob der Sollwert des Motors und der Istwert der PWM übereinstimmen.
 *  Wenn nicht, wird der Wert der PWM mit der angegebenen Steigung in Richtung des Sollwerts verändert
 *
 *  @param desired Der aktuelle Sollwert des Motors in -1/0/1
 *  @param last_pwm Der vorhergehende PWM-Wert
 *  @param ascend Die Steigung der steigenden Flanke
 *  @param descend Die Steigung der fallenden Flanke
 *
 *  @return Der PWM-Wert zwischen 0V und 3V als Zahl zwischen 0 (0V) und 256 (3V)
 **/
int getPWMValue(desired, last_pwm, ascend, descend)
{
	int pwm;
	/*
	 * TODO
	 */
	return pwm;
}
