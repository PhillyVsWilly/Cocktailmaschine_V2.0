#ifndef _MODULE_6_HANDLING_H_
#define _MODULE_6_HANDLING_H_

#include "Sensors.h"
#include "Evaluation.h"
#include "Actuators.h"

void vEvaluate_Module_6_Handling(InputValues_t, Module_State_6_Handling_t*, OutputValues_t*);

uint_8 handling_rx_buffer[50];

void packet_handler_type_1(char*);
void packet_handler_type_2();
void packet_handler_type_5();

void init_drink_lists();


#endif
