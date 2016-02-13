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

volatile int btnPressedFlag;

void EXTI0_IRQHandler(void);
void init_gpio(void);

void init_gpio(void) {
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef gpioinit = {
			.Pin = GPIO_PIN_0,
			.Mode = GPIO_MODE_IT_RISING,
			.Pull = GPIO_NOPULL,
			.Speed = GPIO_SPEED_LOW
	};

	HAL_GPIO_Init(GPIOA, &gpioinit);
	HAL_NVIC_SetPriority(EXTI0_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);

}


void EXTI0_IRQHandler(void){
	__HAL_GPIO_EXTI_CLEAR_IT((GPIO_PIN_0));
	HAL_NVIC_ClearPendingIRQ(EXTI0_IRQn);
	btnPressedFlag = 1;

}

int
main(int argc, char* argv[])
{
  // At this stage the system clock should have already been configured
  // at high speed.

	btnPressedFlag = 0;

	HAL_StatusTypeDef halrc;
	halrc = HAL_Init();
	if(halrc == HAL_OK) {
		init_gpio();
	}
	if(halrc != HAL_OK) {
		// TODO: flash an LED or something... once you figure out how to do that
	}
	while(1) {
		if(btnPressedFlag == 1) {
			GPIO_PinState rc = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
			trace_printf("Read BTN %d\n", rc == GPIO_PIN_SET);
			btnPressedFlag = 0;
		}

	}
	return 0;
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
