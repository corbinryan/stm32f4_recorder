//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"

#include "stm32f4xx.h"
#include "stm32f4xx_hal.h"

// ----------------------------------------------------------------------------
//
// Semihosting STM32F4 empty sample (trace via DEBUG).
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the DEBUG output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

#include "GPIO.h"

volatile int usrBtnPressedFlag;

enum RecorderStates {IDLE, RECORD, PLAYBACK};

void EXTI0_IRQHandler(void);
enum RecorderStates NextState(enum RecorderStates currentState);


enum RecorderStates NextState(enum RecorderStates currentState) {
	enum RecorderStates retval;
	switch(currentState) {
		case IDLE:
			retval = RECORD;
			PlaybackLedOff();
			RecordLedOn();
			break;
		case RECORD:
			retval = PLAYBACK;
			PlaybackLedOn();
			RecordLedOff();
			break;
		case PLAYBACK:
		default:
			retval = IDLE;
			PlaybackLedOff();
			RecordLedOff();
			break;
	}

	return retval;
}

void EXTI0_IRQHandler(void){
	__HAL_GPIO_EXTI_CLEAR_IT((GPIO_PIN_0));
	HAL_NVIC_ClearPendingIRQ(EXTI0_IRQn);
	usrBtnPressedFlag = 1;

}

int
main(int argc, char* argv[])
{
  // At this stage the system clock should have already been configured
  // at high speed.

	usrBtnPressedFlag = 0;
	enum RecorderStates State = IDLE;

	HAL_StatusTypeDef halrc;
	halrc = HAL_Init();
	if(halrc == HAL_OK) {
		init_gpio();
	}
	if(halrc != HAL_OK) {
		// TODO: flash an LED or something... once you figure out how to do that
	}
	while(1) {
		if(usrBtnPressedFlag == 1) {
			State = NextState(State);
			usrBtnPressedFlag = 0;
		}

	}
	return 0;
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
