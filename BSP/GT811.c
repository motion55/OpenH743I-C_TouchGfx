/**
 ******************************************************************************
 * @file    ft6x06.c
 * @author  MCD Application Team
 * @version V1.0.0
 * @date    03-August-2015
 * @brief   This file provides a set of functions needed to manage the FT6X06
 *          IO Expander devices.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2015 STMicroelectronics</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "GT811.h"
#include "TS_I2C.h"
#include "stdio.h"
#include "main.h"
/** @addtogroup BSP
 * @{
 */

/** @addtogroup Component
 * @{
 */

/** @defgroup GT811
 * @{
 */

/* Private typedef -----------------------------------------------------------*/

/* Maximum border values of the touchscreen pad */
#define  GT811_MAX_WIDTH              ((uint16_t)800)     /* Touchscreen pad max width   */
#define  GT811_MAX_HEIGHT             ((uint16_t)480)     /* Touchscreen pad max height  */

/**
 * @}
 */

/** @defgroup GT811_Private_Functions ft6x06 Private Functions
 * @{
 */

/**
 * @brief  Initialize the GT811 communication bus
 *         from MCU to GT811 : ie I2C channel initialization (if required).
 * @retval None
 */

uint8_t GT811_Init(void) {
	I2C_Init();

	/* reset GT811 */
	HAL_GPIO_WritePin(TP_RST_GPIO_Port, TP_RST_Pin, GPIO_PIN_RESET);
	HAL_Delay(200);
	HAL_GPIO_WritePin(TP_RST_GPIO_Port, TP_RST_Pin, GPIO_PIN_SET);
	HAL_Delay(200);

	/* if Version is correct, send the configuration parameters */
	if (GT811_ReadID() == GT811_VERSION_VALUE) {
		/* touch screen configuration parameter (touch screen manufacturers provide) */
		uint8_t GTP_CFG_DATA[] = { 0x12, 0x10, 0x0E, 0x0C, 0x0A, 0x08, 0x06,
				0x04, 0x02, 0x00, 0x05, 0x55, 0x15, 0x55, 0x25, 0x55, 0x35,
				0x55, 0x45, 0x55, 0x55, 0x55, 0x65, 0x55, 0x75, 0x55, 0x85,
				0x55, 0x95, 0x55, 0xA5, 0x55, 0xB5, 0x55, 0xC5, 0x55, 0xD5,
				0x55, 0xE5, 0x55, 0xF5, 0x55, 0x1B, 0x03, 0x00, 0x00, 0x00,
				0x13, 0x13, 0x13, 0x0F, 0x0F, 0x0A, 0x50, 0x30, 0x05, 0x03,
				0x64, 0x05, 0xe0, 0x01, 0x20, 0x03, 0x00, 0x00, 0x32, 0x2C,
				0x34, 0x2E, 0x00, 0x00, 0x04, 0x14, 0x22, 0x04, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x20, 0x14, 0xEC, 0x01, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x30, 0x25,
				0x28, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, };

		/* config  */
		GTP_CFG_DATA[62] = GT811_MAX_WIDTH >> 8;
		GTP_CFG_DATA[61] = GT811_MAX_WIDTH & 0xff;
		GTP_CFG_DATA[60] = GT811_MAX_HEIGHT >> 8;
		GTP_CFG_DATA[59] = GT811_MAX_HEIGHT & 0xff;

		I2C_WriteReg(GT811_CMD_WR, GT811_CONFIG_REG, (uint8_t*) GTP_CFG_DATA,
				sizeof(GTP_CFG_DATA));
	}
	return HAL_OK;
}

/**
 * @brief  Read the GT811 device ID, pre initialize I2C in case of need to be
 *         able to read the GT811 device ID, and verify this is a GT811.
 * @param  DeviceAddr: I2C GT811 Slave address.
 * @retval The Device ID (two bytes).
 */
uint16_t GT811_ReadID() {
	uint8_t value[2];
	I2C_ReadReg(GT811_CMD_WR, GT811_VERSION, value, 2);
	printf("ID    %x\r\n", (((uint16_t) value[0] << 8) + value[1]));
	return value[0] * 0x100 + value[1];
}

void GT811_GetState(TS_StateTypeDef *TS_State) {
	uint8_t RegBuf[34];

	/* Read touch message */
	I2C_ReadReg(GT811_CMD_WR, GT811_READ_XY_REG, RegBuf, sizeof(RegBuf));

	/* get touch massage */
	TS_State->SensorId = RegBuf[0x00] >> 6;
	TS_State->touchDetected = RegBuf[0x00] & 0x1F;

	if (TS_State->touchDetected != 0) {
		//Touch point 1 coordinates
		TS_State->touchY[0] = GT811_MAX_HEIGHT
				- (((uint16_t) RegBuf[0x02] << 8) + RegBuf[0x03]);
		TS_State->touchX[0] = (((uint16_t) RegBuf[0x04] << 8) + RegBuf[0x05]);
		TS_State->touchWeight[0] = RegBuf[0x06];

		//Touch point 2 coordinates
		TS_State->touchY[1] = GT811_MAX_HEIGHT
				- (((uint16_t) RegBuf[0x07] << 8) + RegBuf[0x08]);
		TS_State->touchX[1] = (((uint16_t) RegBuf[0x09] << 8) + RegBuf[0x0A]);
		TS_State->touchWeight[1] = RegBuf[0x0B];

		//Touch point 3 coordinates
		TS_State->touchY[2] = GT811_MAX_HEIGHT
				- (((uint16_t) RegBuf[0x0C] << 8) + RegBuf[0x0D]);
		TS_State->touchX[2] = (((uint16_t) RegBuf[0x0E] << 8) + RegBuf[0x0F]);
		TS_State->touchWeight[2] = RegBuf[0x10];

		//Touch point 4 coordinates
		TS_State->touchY[3] = GT811_MAX_HEIGHT
				- (((uint16_t) RegBuf[0x11] << 8) + RegBuf[0x18]);
		TS_State->touchX[3] = (((uint16_t) RegBuf[0x19] << 8) + RegBuf[0x1A]);
		TS_State->touchWeight[3] = RegBuf[0x1B];

		//Touch point 5 coordinates
		TS_State->touchY[4] = GT811_MAX_HEIGHT
				- (((uint16_t) RegBuf[0x1C] << 8) + RegBuf[0x1D]);
		TS_State->touchX[4] = (((uint16_t) RegBuf[0x1E] << 8) + RegBuf[0x1F]);
		TS_State->touchWeight[4] = RegBuf[0x20];
	}

}

/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
