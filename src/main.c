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

volatile int usrBtnPressedFlag;

enum RecorderStates {IDLE, RECORD, PLAYBACK};

void EXTI0_IRQHandler(void);
void init_gpio(void);
GPIO_PinState readUserButton(void);
enum RecorderStates NextState(enum RecorderStates currentState);
void RecordLedOff(void);
void RecordLedOn(void);
void PlaybackLedOff(void);
void PlaybackLedOn(void);


void init_gpio(void) {
	// User Button
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef gpio_init_user_btn = {
			.Pin = GPIO_PIN_0,
			.Mode = GPIO_MODE_IT_RISING,
			.Pull = GPIO_NOPULL,
			.Speed = GPIO_SPEED_LOW
	};
	HAL_GPIO_Init(GPIOA, &gpio_init_user_btn);
	// Set Up User Button External Interrupt
	HAL_NVIC_SetPriority(EXTI0_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);

	// Blue Button - Program Running
	__HAL_RCC_GPIOD_CLK_ENABLE();
	GPIO_InitTypeDef gpio_init_run_led =  {
			.Pin = GPIO_PIN_15,
			.Mode = GPIO_MODE_OUTPUT_PP,
			.Pull = GPIO_NOPULL,
			.Speed = GPIO_SPEED_LOW
	};
	HAL_GPIO_Init(GPIOD, &gpio_init_run_led);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_SET);

	// Red Button - Audio Recording
	__HAL_RCC_GPIOD_CLK_ENABLE();
	GPIO_InitTypeDef gpio_init_recording_led =  {
			.Pin = GPIO_PIN_14,
			.Mode = GPIO_MODE_OUTPUT_PP,
			.Pull = GPIO_NOPULL,
			.Speed = GPIO_SPEED_LOW
	};
	HAL_GPIO_Init(GPIOD, &gpio_init_recording_led);
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);

	//Green Button - Audio Playback
	__HAL_RCC_GPIOD_CLK_ENABLE();
	GPIO_InitTypeDef gpio_init_playback_led =  {
			.Pin = GPIO_PIN_12,
			.Mode = GPIO_MODE_OUTPUT_PP,
			.Pull = GPIO_NOPULL,
			.Speed = GPIO_SPEED_LOW
		};
		HAL_GPIO_Init(GPIOD, &gpio_init_playback_led);
		HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);
}

GPIO_PinState readUserButton(void) {
	HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0);
}

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

inline void RecordLedOff(void) {HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_RESET);}
inline void RecordLedOn(void) {HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, GPIO_PIN_SET);}
inline void PlaybackLedOff(void) {HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_RESET);}
inline void PlaybackLedOn(void){HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, GPIO_PIN_SET);}


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
