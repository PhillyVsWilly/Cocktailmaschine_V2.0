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
	switch(virtual_testenv_timer_variable)
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
			input->Transportation.inModule5 = 1;
			break;
		case 65:
			input->Transportation.inModule2 = 0;
		default:
			break;
	}
	
}


void updateVirtualInput_Module_2(InputValues_t* input)
{
	switch(virtual_testenv_timer_variable)
	{
		case 15: 
			input->Gravity.start = 1;				// gravity allowed to begin to work
			break;									
		case 16:
			input->Gravity.sensor_down = 1;			// sensor measures glass's weigth on the bottom
			input->Gravity.sensor_up = 0;	
			break;
		case 20:									
			input->Gravity.position_tree = 1;		// tree moving to desired position -> altering voltage
			break;
		case 21:
			input->Gravity.position_tree = 2;
			break;
		case 24:
			input->Gravity.position_tree = 4;
			break;
		case 26: 
			input->Gravity.position_tree = 6;		// reached desired position
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
		default:
			break;
	}
	
}


void updateVirtualInput_Module_3(InputValues_t* input)
{
	switch(virtual_testenv_timer_variable)
	{
		case 16:
			input->Pumping.weight_glass = 700;		// in the beginning, there is already some liquid
		case 20:
			input->Pumping.start = 1;				
			break;
		case 27:									
			input->Pumping.weight_glass = 704;	
			break;
		case 30:
			input->Pumping.weight_glass = 710;		
			break;
		case 33:
			input->Pumping.weight_glass = 750;
			break;
		case 40:
			input->Pumping.weight_glass = 830;
			break;
		case 43:
			input->Pumping.weight_glass = 880;
			break;
		case 45:
			input->Pumping.weight_glass = 900;
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
			input->Pouring.start = 1;
			input->Pouring.weight = 100;
			break;
		case 17:
			input->Pouring.position_down = 0;
		case 30:
			input->Pouring.position_up = 1;
		case 27:									
			input->Pouring.weight = 104;			// inclination angle of bottle increases  
			break;
		case 30:
			input->Pouring.weight = 110;			// weigth of glass increases [g] -> consider the amount of flowing liquid due to the inclination angle 
			break;
		case 33:
			input->Pouring.weight_ = 150;
			break;
		case 40:
			input->Pouring.weight = 230;
			break;
		case 43:
			input->Pouring.weight = 280;
			break;
		case 44:
			input->Pouring.weight = 294;
			break;
		case 45:
			input->Pouring.weight = 300;
			break;

	}
}

void updateVirtualInput_Module_5(InputValues_t* input)
{
//TODO

}

void updateVirtualInput_Module_6(InputValues_t* input)
{
//TODO

}

void updateVirtualInput_Module_7(InputValues_t* input)
{
//TODO

}




