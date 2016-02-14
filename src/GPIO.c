/*
 * GPIO.c
 *
 *  Created on: Feb 14, 2016
 *      Author: ryan
 */

#include "GPIO.h"

typedef struct gpio_struct {
	GPIO_TypeDef *Port;
	uint16_t Pin;
} GpioStruct;

inline void SetOutput(const GpioStruct *output, int state);
inline int ReadInput(const GpioStruct *input);

const GpioStruct UserButtonPin = {
		.Port = GPIOA,
		.Pin = GPIO_PIN_0
};

const GpioStruct ActiveLEDPin = {
		.Port = GPIOD,
		.Pin = GPIO_PIN_15
};

const GpioStruct RecordingLEDPin = {
		.Port = GPIOD,
		.Pin = GPIO_PIN_14
};

const GpioStruct PlaybackLEDPin = {
		.Port = GPIOD,
		.Pin = GPIO_PIN_12,
};


void init_gpio(void) {
	// User Button
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef gpio_init_user_btn = {
			.Pin = UserButtonPin.Pin,
			.Mode = GPIO_MODE_IT_RISING,
			.Pull = GPIO_NOPULL,
			.Speed = GPIO_SPEED_LOW
	};
	HAL_GPIO_Init(UserButtonPin.Port, &gpio_init_user_btn);
	// Set Up User Button External Interrupt
	HAL_NVIC_SetPriority(EXTI0_IRQn, 1, 0);
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);

	// Blue Button - Program Running
	__HAL_RCC_GPIOD_CLK_ENABLE();
	GPIO_InitTypeDef gpio_init_run_led =  {
			.Pin = ActiveLEDPin.Pin,
			.Mode = GPIO_MODE_OUTPUT_PP,
			.Pull = GPIO_NOPULL,
			.Speed = GPIO_SPEED_LOW
	};
	HAL_GPIO_Init(ActiveLEDPin.Port, &gpio_init_run_led);
	SetOutput(&ActiveLEDPin, 1);

	// Red Button - Audio Recording
	__HAL_RCC_GPIOD_CLK_ENABLE();
	GPIO_InitTypeDef gpio_init_recording_led =  {
			.Pin = RecordingLEDPin.Pin,
			.Mode = GPIO_MODE_OUTPUT_PP,
			.Pull = GPIO_NOPULL,
			.Speed = GPIO_SPEED_LOW
	};
	HAL_GPIO_Init(RecordingLEDPin.Port, &gpio_init_recording_led);
	SetOutput(&RecordingLEDPin, 0);

	//Green Button - Audio Playback
	__HAL_RCC_GPIOD_CLK_ENABLE();
	GPIO_InitTypeDef gpio_init_playback_led =  {
			.Pin = PlaybackLEDPin.Pin,
			.Mode = GPIO_MODE_OUTPUT_PP,
			.Pull = GPIO_NOPULL,
			.Speed = GPIO_SPEED_LOW
		};
	HAL_GPIO_Init(PlaybackLEDPin.Port, &gpio_init_playback_led);
	SetOutput(&PlaybackLEDPin, 0);
}

inline void SetOutput(const GpioStruct *output, int state) {
	HAL_GPIO_WritePin(output->Port, output->Pin, state ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

inline int ReadInput(const GpioStruct *input) {
	return (HAL_GPIO_ReadPin(input->Port, input->Pin) == GPIO_PIN_SET ? 1 : 0);
}

inline void RecordLedOff(void) {SetOutput(&RecordingLEDPin, 0);}
inline void RecordLedOn(void) {SetOutput(&RecordingLEDPin, 1);}
inline void PlaybackLedOff(void) {SetOutput(&PlaybackLEDPin, 0);}
inline void PlaybackLedOn(void){SetOutput(&PlaybackLEDPin, 1);}
inline int ReadUserButton(void) { return ReadInput(&UserButtonPin); }
