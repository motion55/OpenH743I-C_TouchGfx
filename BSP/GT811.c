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
#include "i2c.h"
#include "stdio.h"
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

static I2C_HandleTypeDef *_hi2c;

uint8_t I2C_ReadReg(uint8_t I2c_Addr, uint16_t reg, uint8_t *buf, uint8_t len) {
	uint8_t result = HAL_I2C_Mem_Read(_hi2c, I2c_Addr, reg, sizeof(uint16_t), buf, len, 100+len);
	return result;
}

uint8_t I2C_WriteReg(uint8_t I2c_Addr, uint16_t reg, uint8_t *buf, uint8_t len) {
	uint8_t result = HAL_I2C_Mem_Write(_hi2c, I2c_Addr, reg, sizeof(reg), buf, len, 1009);
	return result;
}

/**
 * @}
 */

/** @defgroup GT811_Private_Functions ft6x06 Private Functions
 * @{
 */
/* touch screen configuration parameter (touch screen manufacturers provide) */
#if 1
const uint8_t GT811_CFG_DATA[106] = {
		0x12, 0x10, 0x0E, 0x0C, 0x0A, 0x08, 0x06, 0x04, 0x02, 0x00,	//0-9
		0x05, 0x55, 0x15, 0x55, 0x25, 0x55, 0x35, 0x55, 0x45, 0x55, //10-19
		0x55, 0x55, 0x65, 0x55, 0x75, 0x55, 0x85, 0x55, 0x95, 0x55, //20-20
		0xA5, 0x55, 0xB5, 0x55, 0xC5, 0x55, 0xD5, 0x55, 0xE5, 0x55, //30-30
		0xF5, 0x55,	// 40-41
		0x1B, 0x03, // 42-43
		0x00, 0x00, 0x00,	// 44-46
		0x13, 0x13, 0x13,	// 47-49
		0x0F, 0x0F, 0x0A, 0x50, 0x30, 0x05, 0x03, 0x64, 0x05,	// 50-58
		(GT811_MAX_HEIGHT & 0xff), (GT811_MAX_HEIGHT >> 8),	// 59-60 = 0xe0, 0x01, (480)
		(GT811_MAX_WIDTH & 0xff), (GT811_MAX_WIDTH >> 8), 	// 61-62 = 0x20, 0x03, (800)
		0x00, 0x00, 0x32, 0x2C,	0x34, 0x2E, 0x00,	// 64-69
		0x00, 0x04, 0x14, 0x22, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00,	// 70-79
		0x20, 0x14, 0xEC, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,	// 80-89
		0x00, 0x00, 0x00, 0x00, 0x0C, 0x30, 0x25, 0x28, 0x14, 0x00, // 90-99
		0x00, 0x00, 0x00, 0x00, 0x00, 0x01, // 100-105
		};
#else
const uint8_t GT811_CFG_DATA[106]=
	{
		// [0 - 9] TS induction wire configuration
		0x12,0x10,0x0E,0x0C,0x0A,0x08,0x06,0x04,0x02,0x00,

		// [10 - 41] channels configuration
		0x05,0x55,0x15,0x55,0x25,0x55,0x35,0x55,0x45,0x55,0x55,
		0x55,0x65,0x55,0x75,0x55,0x85,0x55,0x95,0x55,0xA5,0x55,
		0xB5,0x55,0xC5,0x55,0xD5,0x55,0xE5,0x55,0xF5,0x55,

		// [42 - 43] Chip scan control parameters
		0x1B,0x03,

		// [44 - 46] Drive pulse frequency 1, 2, 3
		0x00,0x00,0x00,

		// [47 - 49] Number of drive pulses 1, 2, 4
		0x13,0x13,0x13,

		// [50] The total number of drive channels used (screen drive line + key drive line)
		0x0F,

		// [51] Use the drive wire on the screen
		0x0F,

		// [52] Use a sense line on the screen
		0x0A,

		// [53] The screen key threshold
		0x50,

		// [54] Screen loose threshold
		0x30,

		// [55]
		// b8 Reserved
		// b7 DD2
		// b6 R1
		// b5 R0
		// b4 INT
		// b3 SITO
		// b2 RT
		// b1 ST
		0x05,

		// [56]
		// b8 Reserved
		// b7 Reserved
		// b6-b1 Auto No button to enter the low-power time, 0-63 effective to s as a unit
		0x03,

		// [57] Touch Touch refresh rate control parameter (50Hz - 100Hz): 0-100 effective
		0x64,

		// [58] Number of touch points (1 - 5)
		// b8-b4 reserved
		0x05,

		// [59 - 60] X coordinate outputs the maximum value
		0xe0,0x01,  // 480

		// [61 - 62] The Y coordinate outputs the maximum value
		0x20,0x03,  // 800

		// [63]  X coordinate output threshold: 0-255, with 4 original coordinate points as a unit
		0x00,

		// [64] Y coordinate output threshold: 0-255, with 4 original coordinate points as a unit
		0x00,

		// [65] X direction smoothing control variable, 0-255 can be configured, 0 means off
		0x32,

		// [66] Y direction smoothing control variable, 0-255 can be configured, 0 means off
		0x2C,

		// [67] X direction Smooth upper limit speed: 0-255 can be configured, 0 means off
		0x34,

		// [68] Y direction Smooth upper limit speed: 0-255 can be configured, 0 means off
		0x2E,

		// Reserved
		0x00,0x00,

		// [71] Filter
		// b8-b5: Number of discarded data frames
		// b4-b1: The coordinate window filter value, with 4 as the base
		0x04,

		// [72] 0-255 effective: a single touch area contains more than the number of nodes will be judged as a large area touch
		0x14,

		// [73] Shake_Cu
		// b8-b5: The Touch event is created to debounce
		// b4-b1: The number of fingers from more to less to shake
		0x22,

		// [74] Noise reduction
		// b8-b5: Reserved
		// b4-b1: The white noise reduction amount (low nibble) is valid
		0x04,

		// reserved
		0x00,0x00,0x00,0x00,0x00,

		// [80] Normal Update Time, 0-255 Configurable, Zero Off Base Update (Base Cycle Time)
		0x20,

		// [81] 0-255 Configurable, Zero Closed Base Update (based on the main cycle time)
		0x14,

		// [82 - 83] The baseline updates the control variables
		0xEC,0x01,

		// [84] Reserved
		0x00,

		// [85]
		// b7: button command
		// b6: button independent parameter (007A follow-up version is valid)
		// b5-b0: reserved
		0x00,

		// [86] FPC button ADCCFG parameter (applies only to the drive as the button common)
		0x00,

		// [87] FPC button drive frequency selection (only for the drive as the public key)
		0x00,

		// [88] FPC key drive pulse number (only for driving the button common)
		0x00,

		// [89] Reserved
		0x00,

		// [90 - 93] Key 1 to 4 Position: 0-255 (independent keys when all key positions are multiples of 16)
		0x00,0x00,0x00,0x00,

		// [95]
		// b7-b4: reserved
		// b3-b0: key effective width (one side)
		0x0C,

		// [96] Key press threshold
		0x30,

		// [96] Key release threshold
		0x25,

		// [97] Independent key judgment, the second largest difference in the upper limit
		0x28,

		// [98] Independent key to determine the maximum and maximum gap between the lower limit
		0x14,

		// [99 - 103] Reserved
		0x00,0x00,0x00,0x00,0x00,

		// [104] Configuration update flag, the master write configuration information to write to the location 1
		0x00,

		// [105] ??no clue??
		0x01
	};
#endif
/**
 * @brief  Initialize the GT811 communication bus
 *         from MCU to GT811 : ie I2C channel initialization (if required).
 * @retval None
 */

uint8_t GT811_Init(I2C_HandleTypeDef *hi2c) {
	_hi2c = hi2c;

	/* reset GT811 */
	HAL_GPIO_WritePin(TP_RST_GPIO_Port, TP_RST_Pin, GPIO_PIN_RESET);
	HAL_Delay(200);
	HAL_GPIO_WritePin(TP_RST_GPIO_Port, TP_RST_Pin, GPIO_PIN_SET);
	HAL_Delay(500);

	/* if Version is correct, send the configuration parameters */
	if (GT811_ReadID() == GT811_VERSION_VALUE) {

		I2C_WriteReg(GT811_CMD_WR, GT811_CONFIG_REG,
				(uint8_t*) GT811_CFG_DATA, sizeof(GT811_CFG_DATA));
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
	uint16_t version = ((uint16_t) value[0] << 8) + value[1];
	printf("ID    %x\r\n", version);
	return version;
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
