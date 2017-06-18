/*
 * threads.h
 *
 *  Created on: 18.06.2017
 *      Author: Philipp
 */

#include "main.h"

#ifndef USER_TASKS_H_
#define USER_TASKS_H_

#define MAIN_CYCLE_PRIORITY (tskIDLE_PRIORITY+1)
void vMainCycle(void const* pvParameters);


#endif /* USER_TASKS_H_ */
