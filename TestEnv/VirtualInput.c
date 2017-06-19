/*
 * VirtualInput.c
 *
 *  Created on: 18.06.2017
 *      Author: Philipp
 */

#include "VirtualInput.h"
#include "Sensors.h"

int timer_variable = 0;

void updateVirtualInput_Module_1(InputValues_t* input)
{
	//Beispiel
	if(timer_variable == 5)
	{
		 input->Transportation->lichtschranke_1 = 1;
	}
}


void updateVirtualInput_Module_2(InputValues_t* input)
{
//TODO

}


void updateVirtualInput_Module_3(InputValues_t* input)
{

//TODO
}

void updateVirtualInput_Module_4(InputValues_t* input)
{
//TODO

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




