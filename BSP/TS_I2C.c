#include "TS_I2C.h"
//#include "i2c.h"
//#include "debug_console.h"

#if !(defined(__i2c_H)||defined(__I2C_H__))
/**
 * @brief  I2C delay.
 * @param  None
 * @retval None
 */
static void delay_us(uint32_t value) {
	uint32_t i;
	i = value * 250;
	while (i--) {
		__DSB();
		__ISB();
	};
}
#endif

/**
 * @brief  I2C Init.
 * @param  None
 * @retval None
 */
void I2C_Init(void) {
#if !(defined(__i2c_H)||defined(__I2C_H__))
	GPIO_InitTypeDef GPIO_InitStruct;

	SDA_CLK()
	SCL_CLK()

	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;

	GPIO_InitStruct.Pin = SDA_GPIO_PIN;
	HAL_GPIO_Init(SDA_GPIO_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(SDA_GPIO_PORT, SDA_GPIO_PIN, GPIO_PIN_SET);

	GPIO_InitStruct.Pin = SCL_GPIO_PIN;
	HAL_GPIO_Init(SCL_GPIO_PORT, &GPIO_InitStruct);
	HAL_GPIO_WritePin(SCL_GPIO_PORT, SCL_GPIO_PIN, GPIO_PIN_SET);
#else
	MX_I2C4_Init();
#endif
}

#if !(defined(__i2c_H)||defined(__I2C_H__))
/**
 * @brief  Set SDA In.
 * @param  None
 * @retval None
 */
static void SDA_IN() {
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.Pin = SDA_GPIO_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(SDA_GPIO_PORT, &GPIO_InitStruct);

}

/**
 * @brief  Set SDA Out.
 * @param  None
 * @retval None
 */
static void SDA_OUT() {
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.Pin = SDA_GPIO_PIN;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
	HAL_GPIO_Init(SDA_GPIO_PORT, &GPIO_InitStruct);
}

/**
 * @brief  I2C Start.
 * @param  None
 * @retval None
 */
void I2C_Start(void) {
	SDA_OUT();

	SET_SDA();
	delay_us(1);

	SET_CLK();
	delay_us(1);

	CLR_SDA();
	delay_us(1);

	CLR_CLK();
	delay_us(1);
}

/**
 * @brief  I2C Stop.
 * @param  None
 * @retval None
 */
void I2C_Stop(void) {
	SDA_OUT();

	CLR_SDA();
	delay_us(1);

	SET_CLK();
	delay_us(1);

	SET_SDA();
	delay_us(1);
}

/**
 * @brief  I2C Ack.
 * @param  None
 * @retval None
 */
uint8_t I2C_Wait_Ack(void) {
	uint16_t ucErrTime = 0;

	SDA_IN();
	SET_SDA();
	delay_us(1);
	SET_CLK();
	delay_us(1);

	while (READ_SDA()) {
		ucErrTime++;
		if (ucErrTime > 250) {
			I2C_Stop();
			return 1;
		}
	}
	CLR_CLK();
	return 0;
}

/**
 * @brief  I2C Ack.
 * @param  None
 * @retval None
 */
void I2C_Ack(void) {
	SDA_OUT();
	CLR_CLK();

	CLR_SDA();
	delay_us(1);

	SET_CLK();
	delay_us(1);

	CLR_CLK();
	delay_us(1);
}

/**
 * @brief  No I2C Ack.
 * @param  None
 * @retval None
 */
void I2C_NAck(void) {
	SDA_OUT();

	SET_SDA();
	delay_us(1);

	SET_CLK();
	delay_us(1);

	CLR_CLK();
	delay_us(1);
}

/**
 * @brief  I2C Send one byte
 * @param  1:ack 0:without ack
 * @retval None
 */
void I2C_Send_Byte(uint8_t txd) {
	uint8_t t;

	SDA_OUT();
	CLR_CLK();       //Put low CLK to send data

	for (t = 0; t < 8; t++) {
		if (txd & 0x80)
			SET_SDA();
		else
			CLR_SDA();

		txd <<= 1;
		delay_us(1);

		SET_CLK();
		delay_us(1);
		CLR_CLK();
		delay_us(1);
	}

	SET_SDA();
	delay_us(1);
	CLR_CLK();
	delay_us(1);
}

/**
 * @brief  I2C read one byte
 * @param  1:ack 0:without ack
 * @retval read value
 */
uint8_t I2C_Read_Byte(unsigned char ack) {
	unsigned char i, receive = 0;
	SDA_IN();         //SDA set in
	for (i = 0; i < 8; i++) {
		CLR_CLK();
		delay_us(1);
		SET_CLK();
		receive <<= 1;
		if (READ_SDA())
			receive++;
		delay_us(1);
	}
	CLR_CLK();
	if (!ack)
		I2C_NAck();   //sent nACK
	else
		I2C_Ack();        //sent ACK

	return receive;
}

/*******************************************************************************
 * Function Name  : I2C_Send
 * Description    : I2C Send bytes to the device.
 * Input          : - buf: Need to send the BUF pointer.
 *                  - len: The length of the array
 * Output         : None
 * Return         : ret
 -0  succeed
 -1  error
 * Attention		 : None
 *******************************************************************************/
uint8_t I2C_Send(uint8_t I2c_Addr, uint8_t *buf, uint8_t len) {
	uint8_t i;
	uint8_t ret = 0;
	I2C_Start();

	I2C_Send_Byte(I2c_Addr);   //Slaver Addr
	I2C_Wait_Ack();

	for (i = 0; i < len; i++) {
		I2C_Send_Byte(buf[i]);
		ret = I2C_Wait_Ack();
		if (ret)
			break;
	}
	I2C_Stop();
	return ret;
}
#endif

/*******************************************************************************
 * Function Name  : I2C_WR_Reg
 * Description    : Writes to the selected register.
 * Input          : - reg: address of the selected register.
 *                  - buf: Need to write the BUF pointer.
 *                  - len: The length of the array
 * Output         : None
 * Return         : ret
 -0  succeed
 -1  error
 * Attention		 : None
 *******************************************************************************/
uint8_t I2C_WriteReg(uint8_t I2c_Addr, uint16_t reg, uint8_t *buf, uint8_t len) {
#if !(defined(__i2c_H)||defined(__I2C_H__))
	uint8_t i;
	uint8_t ret = 0;
	I2C_Start();

	I2C_Send_Byte(I2c_Addr);   //Slaver Addr
	I2C_Wait_Ack();

#ifdef I2C_MEMADD_16BIT
	I2C_Send_Byte(reg >> 8);   	     //Data Addr high
	I2C_Wait_Ack();
#endif

	I2C_Send_Byte(reg & 0xFF);   	   //Data Addr low
	I2C_Wait_Ack();

	for (i = 0; i < len; i++) {
		I2C_Send_Byte(buf[i]);
		ret = I2C_Wait_Ack();
		if (ret)
			break;
	}
	I2C_Stop();
	return ret;
#else
	uint8_t result = HAL_I2C_Mem_Write(&hi2c4, I2c_Addr, reg, sizeof(reg), buf,
			len, 1000);
	return result;
#endif
}

/*******************************************************************************
 * Function Name  : i2C_RD_Reg
 * Description    : Writes to the selected register.
 * Input          : - reg: address of the selected register.
 *                  - buf: Need to read the BUF pointer.
 *                  - len: The length of the array
 * Output         : None
 * Return         : None
 * Attention		 : None
 *******************************************************************************/
void I2C_ReadReg(uint8_t I2c_Addr, uint16_t reg, uint8_t *buf, uint8_t len) {
#if !(defined(__i2c_H)||defined(__I2C_H__))
	uint8_t i;
	I2C_Start();
	I2C_Send_Byte(I2c_Addr);
	I2C_Wait_Ack();

#ifdef I2C_MEMADD_16BIT
	I2C_Send_Byte(reg >> 8);   	     //Data Addr high
	I2C_Wait_Ack();
#endif

	I2C_Send_Byte(reg & 0xFF);   	   //Data Addr low
	I2C_Wait_Ack();
	I2C_Stop();

	I2C_Start();
	I2C_Send_Byte(I2c_Addr + 1);
	I2C_Wait_Ack();

	for (i = 0; i < len; i++) {
		buf[i] = I2C_Read_Byte(i == (len - 1) ? 0 : 1);
	}
	I2C_Stop();
#else
	HAL_I2C_Mem_Read(&hi2c4, I2c_Addr, reg, sizeof(uint16_t), buf, len, 1000);
#endif
}

