/*
 * GPIO.h
 *
 *  Created on: Feb 14, 2016
 *      Author: ryan
 */

#ifndef GPIO_H_
#define GPIO_H_

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

void init_gpio(void);
void RecordLedOff(void);
void RecordLedOn(void);
void PlaybackLedOff(void);
void PlaybackLedOn(void);
int ReadUserButton(void);
void DebounceInputsTick(void);

#endif /* GPIO_H_ */
