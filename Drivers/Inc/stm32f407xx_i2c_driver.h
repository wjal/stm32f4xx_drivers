/*
 * stm32f407xx_i2c_driver.h
 *
 *  Created on: Jun 10, 2026
 *      Author: jameslittle
 */

#ifndef INC_STM32F407XX_I2C_DRIVER_H_
#define INC_STM32F407XX_I2C_DRIVER_H_

#include "stm32f407xx.h"

typedef struct{
	uint32_t	I2C_SCLSpeed;
	uint8_t		I2C_DeviceAddress;
	uint8_t		I2C_ACKControl;
	uint16_t	I2C_FMDutyCycle;
}I2C_CONFIG_t;



typedef struct{
	I2C_REG_DEF_t *pI2Cx;
	I2C_CONFIG_t I2C_Config;
	uint8_t 	*pTxBuffer;
	uint8_t 	*pRxBuffer;
	uint32_t 	RxLength;
	uint32_t 	TxLength;
	uint8_t 	TxRxState;
	uint8_t 	devAddress;
	uint32_t 	RxSize;
	uint8_t 	sr;
}I2C_Handle_t;

#define I2C_READY		0
#define I2C_BUSY_IN_RX	1
#define I2C_BUSY_IN_TX	2

#define I2C_SCL_SPEED_SM			100000
#define I2C_SCL_SPEED_FM4K			400000
#define I2C_SCL_SPEED_FM2K			200000

#define I2C_ACK_ENABLE				1
#define I2C_ACK_DISABLE				0

#define I2C_FM_DUTY_CYCLE_2			0
#define I2C_FM_DUTY_CYCLE_16_9		1


//FLAGS

#define I2C_FLAG_TXE (1 << I2C_SR1_TXE)
#define I2C_FLAG_RXNE (1 << I2C_SR1_RXNE)
#define I2C_FLAG_SB (1 << I2C_SR1_SB)
#define I2C_FLAG_ADDR (1 << I2C_SR1_ADDR)
#define I2C_FLAG_BTF (1 << I2C_SR1_BTF)
#define I2C_FLAG_ADD10 (1 << I2C_SR1_ADD10)
#define I2C_FLAG_STOPF (1 << I2C_SR1_STOPF)
#define I2C_FLAG_BERR (1 << I2C_SR1_BERR)
#define I2C_FLAG_ARLO (1 << I2C_SR1_ARLO)
#define I2C_FLAG_AF (1 << I2C_SR1_AF)
#define I2C_FLAG_OVR (1 << I2C_SR1_OVR)
#define I2C_FLAG_PECERR (1 << I2C_SR1_PECERR)
#define I2C_FLAG_TIMEOUT (1 << I2C_SR1_TIMEOUT)
#define I2C_FLAG_SMBALERT (1 << I2C_SR1_SMBALERT)

#define I2C_FLAG_MSL (1 << I2C_SR2_MSL)
#define I2C_FLAG_BUSY (1 << I2C_SR2_BUSY)
#define I2C_FLAG_TRA (1 << I2C_SR2_TRA)
#define I2C_FLAG_GENCALL (1 << I2C_SR2_GENCALL)
#define I2C_FLAG_SMBDEFAULT (1 << I2C_SR2_SMBDEFAUL)
#define I2C_FLAG_SMBHOST (1 << I2C_SR2_SMBHOST)
#define I2C_FLAG_DUALAF (1 << I2C_SR2_DUALAF)
#define I2C_FLAG_PEC (1 << I2C_SR2_PEC)




#define I2C_DISABLE_SR 	RESET
#define I2C_ENABLE_SR 		SET


/*
 * I2C_APP_EVENT MACROS
 */

#define I2C_EV_TXE_COMPLETE 	0
#define I2C_EV_RX_COMPLETE		1
#define I2C_EV_STOPF			2
#define I2C_ERROR_BERR			3
#define I2C_ERROR_ARLO			4
#define I2C_ERROR_OVR			5
#define I2C_ERROR_AF			6
#define I2C_ERROR_TIMEOUT 		7
#define I2C_EV_DATA_REQ			8
#define I2C_EV_DATA_RCV			9
/**
 * Periph clck
 */
void I2C_GenerateStartCondition(I2C_REG_DEF_t *pI2Cx);
void I2C_GenerateStopCondition(I2C_REG_DEF_t *pI2Cx);

void I2C_Peri_clk_ctrl(I2C_REG_DEF_t *pI2Cx, uint8_t en_or_dis);
void I2C_Peripheral_ctrl(I2C_REG_DEF_t *pI2Cx, uint8_t en_or_dis);
/*
 * Init and de-init
 */
void I2C_init(I2C_Handle_t *pI2C_Handle_t);
void I2C_deinit(I2C_REG_DEF_t *pI2Cx);


uint8_t I2C_GetFlagStatus(I2C_REG_DEF_t *pI2Cx, uint32_t FLAGNAME);
void I2C_ManageAcking(I2C_REG_DEF_t *pI2Cx, uint8_t en_or_dis);

void I2C_MasterSendData(I2C_Handle_t *pI2C_Handle, uint8_t *pTxBuffer, uint32_t length, uint8_t slave_address, uint8_t sr);
void I2C_MasterReceiveData(I2C_Handle_t *pI2C_Handle, uint8_t *pRxBuffer, uint32_t length, uint8_t slave_address, uint8_t sr);

uint8_t I2C_MasterSendData_IT(I2C_Handle_t *pI2C_Handle, uint8_t *pTxBuffer, uint32_t length, uint8_t slave_address, uint8_t sr);
uint8_t I2C_MasterReceiveData_IT(I2C_Handle_t *pI2C_Handle, uint8_t *pRxBuffer, uint32_t length, uint8_t slave_address, uint8_t sr);
void I2C_CloseReceiveData(I2C_Handle_t *pI2CHandle);
void I2C_CloseSendData(I2C_Handle_t *pI2CHandle);

void I2C_SlaveManageCallbackEvents(I2C_REG_DEF_t *pI2Cx, uint8_t en_or_dis);

/*
 * slave
 */
void I2C_SlaveSendData(I2C_REG_DEF_t *pI2Cx, uint8_t data);
uint8_t I2C_SlaveReceiveData(I2C_REG_DEF_t *pI2Cx);

/*
 * IRQ CONFIG AND ISR HANDLING
 *
 *
 */
void I2C_IRQConfig(uint8_t IRQ_number, uint8_t IRQ_en_or_dis);
void I2C_Priority_Config(uint8_t IRQ_number, uint32_t IRQ_priority);
void I2C_EV_IRQHandling(I2C_Handle_t *pI2C_Handle);
void I2C_ER_IRQHandling(I2C_Handle_t *pI2C_Handle);

/*
 * OTHER PERIPH CTRLS
 */
void I2C_ApplicationEventCallback(I2C_Handle_t *pI2C_Handle_t, uint8_t app_event);


#endif /* INC_STM32F407XX_I2C_DRIVER_H_ */

