/*
 * VirtualInput.c
 *
 *  Created on: 18.06.2017
 *      Author: Philipp
 */

#include "VirtualInput.h"
#include "Sensors.h"

int virtual_testenv_timer_variable = 0;

void updateVirtualInput_Module_1(InputValues_t* input)
{
	// 15sec = 75 Schritte des Timers
	// Nulletzen könnte man auch machen, aber wahrscheinlich ist ledilich erstmal ein Durchgang notwendig?!
	/*switch(virtual_testenv_timer_variable)
	{
		case 5: 
			input->Transportation.start = 1;		// transportation allowed to begin to work
			break;
		case 10:
			input->Transportation.inModule2 = 1;	// glass in module 2 (gravity)
			break;
		case 15:
			input->Transportation.inModule3 = 1;
			input->Transportation.inModule4 = 1;
			break;
		case 30:
			input->Transportation.inModule7 = 1;
			break;
		case 65:
			input->Transportation.inModule2 = 1;
		default:
			break;
	}*/
	
}


void updateVirtualInput_Module_2(InputValues_t* input)
{
	switch(virtual_testenv_timer_variable)
	{
		case 15: 
			//TODO input->Gravity.start = 1;				// gravity allowed to begin to work
			break;									
		case 16:
			input->Gravity.sensor_down = 1;			// sensor measures glass's weigth on the bottom
			input->Gravity.sensor_up = 0;	
			break;
		case 20:									
			input->Gravity.position_tree = 0.5;		// tree moving to desired position -> altering voltage
			break;
		case 21:
			input->Gravity.position_tree = 0.7;
			break;
		case 24:
			input->Gravity.position_tree = 1.3;
			break;
		case 26: 
			input->Gravity.position_tree = 2;		// reached desired position
			break;
		case 28:									// glass getting lifted
			input->Gravity.sensor_down = 0;	
			break;
		case 35:	
			input->Gravity.sensor_up = 1;			// sensor measures glass docking on tree
			break;
		case 50:	
			input->Gravity.sensor_up = 0;			// back
			break;
		case 57:	
			input->Gravity.sensor_down = 1;
			break;
		case 65:
			input->Gravity.doors_open = 1;
		default:
			break;
	}
	
}


void updateVirtualInput_Module_3(InputValues_t* input)
{
	switch(virtual_testenv_timer_variable)
	{
		case 16:
			input->Pumping.weight_glass = 1;
			break;						// in the beginning, there is already some liquid
		case 21:
			input->Pumping.valve_position = 0.4215;			// valve is opening -> voltage rises
		case 23:
			input->Pumping.valve_position = 0.83;
		case 25:									
			input->Pumping.weight_glass = 1.2;	
			break;
		case 26:
			input->Pumping.valve_position = 1.7;
			break;
		case 30:
			input->Pumping.weight_glass = 1.4;		
			break;
		case 33:
			input->Pumping.weight_glass = 1.6;
			break;
		case 40:
			input->Pumping.weight_glass = 2.5;
			break;
		case 43:
			input->Pumping.weight_glass = 2.8;
			break;
		case 45:
			input->Pumping.weight_glass = 2.9;
			break;
	}
}

void updateVirtualInput_Module_4(InputValues_t* input)
{
	switch(virtual_testenv_timer_variable)
	{
		case 16:
			input->Pouring.position_down = 1;
			input->Pouring.position_up = 0;
			input->Pouring.weight = 0.5;
			break;
		case 17:
			input->Pouring.position_down = 0;
			break;
		case 27:									
			input->Pouring.weight = 0.55;			// inclination angle of bottle increases  
			break;
		case 30:
			input->Pouring.weight = 0.6;			// weigth of glass increases [g] -> consider the amount of flowing liquid due to the inclination angle 
			input->Pouring.position_up = 1;
			break;
		case 33:
			input->Pouring.weight = 0.63;
			break;
		case 40:
			input->Pouring.weight = 0.8;
			break;
		case 43:
			input->Pouring.weight = 0.83;
			break;
		case 44:
			input->Pouring.weight = 0.84;
			break;
		case 45:
			input->Pouring.weight = 0.845;
			break;

	}
}

void updateVirtualInput_Module_5(InputValues_t* input)
{
		bool start_doors_open;
		bool start_light_barrier;
		bool end_doors_open;
		bool end_light_barrier;
		bool glass_finished;
	switch(virtual_testenv_timer_variable)
	{
		case 10:
			input->Sensors.start_light_barrier = 1;		// human's hand stops light interaction of light barrier
			break;
		case 60:
			input->Sensors.end_light_barrier = 1;		// someone picks the cocktail
			break;
		case 70:
			input->Sensors.end_doors_open = 1;	
			break;
			
	}
}

void updateVirtualInput_Module_6(InputValues_t* input)
{
//TODO

}

void updateVirtualInput_Module_7(InputValues_t* input)
{
	switch(virtual_testenv_timer_variable)
	{
	case 35:
		input->Ice.weight = 1;
		break;
	case 38:
		input->Ice.weight = 1.8;
		break;	
	case 40:
		input->Ice.light_barrier_enough_cube_ice = 0;
	case 65:
		input->Ice.doors_open = 1;
	}

}




