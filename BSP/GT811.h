/**
 ******************************************************************************
 * @file    ft6x06.h
 * @author  MCD Application Team
  * @version V1.0.0
  * @date    03-August-2015
 * @brief   This file contains all the functions prototypes for the
 *          ft6x06.c IO expander driver.
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GT811_H
#define __GT811_H

#ifdef __cplusplus
extern "C" {
#endif
  
/* Includes ------------------------------------------------------------------*/
#include "main.h"
/* Macros --------------------------------------------------------------------*/

/** @typedef GT811_handle_TypeDef
 *  GT811 Handle definition.
 */


  /** @addtogroup BSP
   * @{
   */

  /** @addtogroup Component
   * @{
   */

  /** @defgroup GT811
   * @{
   */

  /* Exported types ------------------------------------------------------------*/

  /** @defgroup GT811_Exported_Types
   * @{
   */

  /* Exported constants --------------------------------------------------------*/

  /** @defgroup GT811_Exported_Constants
   * @{
   */
/**
  * @brief TouchScreen GT811 Slave I2C address
  */
#define GT811_CMD_WR		   0XBA	/* GT811 Write address  */
#define GT811_CMD_RD   	   0XBB	/* GT811 Read  address  */


/* Max detectable simultaneous touches */
#define TS_MAX_NB_TOUCH    5

/**
 * @brief : Definitions for GT811 I2C register addresses on 8 bit
 **/

#define GT811_EXCHG_XY		 1		    /* Swap XY  */

#define GT811_READ_XY_REG  0x721  	/* GT811 touch message start address  */
#define GT811_CONFIG_REG   0x6A2	  /* GT811 configuration start address  */

#define GT811_WAKEUP       0x0FFF   /* GT811 wakeup command  */
#define GT811_SLEEP        0x8000   /* GT811 sleep  command  */

#define GT811_VERSION      0x717    /* GT811 version register addresses */

#define GT811_VERSION_VALUE      0x2010   /* GT811 firmware version */

typedef struct
{
	uint8_t  touchDetected;                /*!< Total number of active touches detected at last scan */
	uint16_t touchX[TS_MAX_NB_TOUCH];      /*!< Touch X[0], X[1] coordinates on 12 bits */
	uint16_t touchY[TS_MAX_NB_TOUCH];      /*!< Touch Y[0], Y[1] coordinates on 12 bits */
  uint8_t  touchWeight[TS_MAX_NB_TOUCH]; /*!< Touch_Weight[0], Touch_Weight[1] : weight property of touches */
	uint32_t SensorId; /*!< type of gesture detected : take value of type @ref TS_GestureIdTypeDef */
} TS_StateTypeDef;


uint8_t GT811_Init(void);
uint16_t GT811_ReadID(void);
void GT811_GetState(TS_StateTypeDef *TS_State);
/**
 * @}
 */

/**
 * @}
 */

/**
 * @}
 */
#ifdef __cplusplus
}
#endif

#endif
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
