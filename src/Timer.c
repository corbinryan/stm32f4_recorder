/*
 * Timer.c
 *
 *  Created on: Feb 15, 2016
 *      Author: ryan
 */

#include "Timer.h"

void init_systick_timer(void) {
	SysTick_Config(SystemCoreClock/1000u);
}
