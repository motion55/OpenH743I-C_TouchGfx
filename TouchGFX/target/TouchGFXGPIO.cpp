/**
  ******************************************************************************
  * File Name          : TouchGFXGPIO.cpp
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */

#include <touchgfx/hal/GPIO.hpp>

/**
 * GPIO_ID Enum, these are used bt TouchGFX framework to signal events.
 *
 * VSYNC_FREQ,  /// Pin is toggled at each VSYNC
 * RENDER_TIME, /// Pin is high when frame rendering begins, low when finished
 * FRAME_RATE,  /// Pin is toggled when the frame buffers are swapped.
 * MCU_ACTIVE   /// Pin is high when framework is utilizing the MCU.
 *
 */

/* USER CODE BEGIN TouchGFXGPIO.cpp */
#include "main.h"

using namespace touchgfx;

/*
 * Perform configuration of IO pins.
 */
void GPIO::init()
{

}

/*
 * Sets a pin high.
 */
void GPIO::set(GPIO_ID id)
{
	switch (id) {
	case VSYNC_FREQ:
		HAL_GPIO_WritePin(LED1_Out_GPIO_Port, LED1_Out_Pin, GPIO_PIN_SET);
		break;
	case RENDER_TIME:
		break;
	case FRAME_RATE:
		HAL_GPIO_WritePin(LED2_Out_GPIO_Port, LED2_Out_Pin, GPIO_PIN_SET);
		break;
	case MCU_ACTIVE:
		HAL_GPIO_WritePin(LED3_Out_GPIO_Port, LED3_Out_Pin, GPIO_PIN_SET);
		break;
	}
}

/*
 * Sets a pin low.
 */
void GPIO::clear(GPIO_ID id)
{
	switch (id) {
	case VSYNC_FREQ:
		HAL_GPIO_WritePin(LED1_Out_GPIO_Port, LED1_Out_Pin, GPIO_PIN_RESET);
		break;
	case RENDER_TIME:
		break;
	case FRAME_RATE:
		HAL_GPIO_WritePin(LED2_Out_GPIO_Port, LED2_Out_Pin, GPIO_PIN_RESET);
		break;
	case MCU_ACTIVE:
		HAL_GPIO_WritePin(LED3_Out_GPIO_Port, LED3_Out_Pin, GPIO_PIN_RESET);
		break;
	}
}

/*
 * Toggles a pin.
 */
void GPIO::toggle(GPIO_ID id)
{
	switch (id) {
	case VSYNC_FREQ:
		HAL_GPIO_TogglePin(LED1_Out_GPIO_Port, LED1_Out_Pin);
		break;
	case RENDER_TIME:
		break;
	case FRAME_RATE:
		HAL_GPIO_TogglePin(LED2_Out_GPIO_Port, LED2_Out_Pin);
		break;
	case MCU_ACTIVE:
		HAL_GPIO_TogglePin(LED3_Out_GPIO_Port, LED3_Out_Pin);
		break;
	}
}

/*
 * Gets the state of a pin.
 */
bool GPIO::get(GPIO_ID id)
{
  return false;
}

/* USER CODE END TouchGFXGPIO.cpp */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
