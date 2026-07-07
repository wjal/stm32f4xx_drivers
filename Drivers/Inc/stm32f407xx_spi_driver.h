/*
 * stm32f4xx_spi_driver.h
 *
 *  Created on: Apr 28, 2026
 *      Author: jameslittle
 */

#ifndef INC_STM32F407XX_SPI_DRIVER_H_
#define INC_STM32F407XX_SPI_DRIVER_H_

#include "stm32f407xx.h"



typedef struct{
	uint8_t SPI_DEVICE_MODE;
	uint8_t SPI_BUS_CONFIG;
	uint8_t SPI_SCLK_SPEED;
	uint8_t SPI_DFF;
	uint8_t SPI_CPOL;
	uint8_t SPI_CPHA;
	uint8_t SPI_SSM;
}SPI_CONFIG_t;

typedef struct{
	SPI_REG_DEF_t 	*pSPIx;
	SPI_CONFIG_t  	SPI_Config;
	uint8_t 		*pTxBuffer;
	uint8_t 		*pRxBuffer;
	uint32_t 		TxLength;
	uint32_t 		RxLength;
	uint8_t  		TxState;
	uint8_t  		RxState;
}SPI_Handle_t;
//APB1

/*
 * SPI_DEVICE MODE
 */
#define SPI_DEVICE_MODE_MASTER	1
#define SPI_DEVICE_MODE_SLAVE	0

/*
 * SPI_BUS_CONFIG
 */
#define SPI_BUS_CONFIG_FD				1
#define SPI_BUS_CONFIG_HD				2
#define SPI_BUS_CONFIG_SIMPLEX_RX_ONLY	3

/*
 * SPI_SCLK_SPPED
 */

#define SPI_BAUD_PCLK_DIV_2		0
#define SPI_BAUD_PCLK_DIV_4		1
#define SPI_BAUD_PCLK_DIV_8		2
#define SPI_BAUD_PCLK_DIV_16	3
#define SPI_BAUD_PCLK_DIV_32	4
#define SPI_BAUD_PCLK_DIV_64	5
#define SPI_BAUD_PCLK_DIV_128	6
#define SPI_BAUD_PCLK_DIV_256	7

/*
 * SPI_DFF
 */
#define SPI_DFF_8_BITS		0
#define SPI_DFF_16_BITS		1
/*
 * SPI_CPOL
 */
#define SPI_CPOL_LOW		0
#define SPI_CPOL_HI			1
/*
 * SPI_CPHA
 */
#define SPI_CPHA_LOW		0
#define SPI_CPHA_HI			1
/*
 * SPI_SSM
 */
#define SPI_SSM_DIS			0
#define SPI_SSM_EN			1

/*
 * SPI STATES
 */
#define SPI_READY			0
#define SPI_BUSY_IN_RX		1
#define SPI_BUSY_IN_TX		2

/*
 * SPI EVENTS
 */
#define SPI_EVENT_TX_COMPLETE	1
#define SPI_EVENT_RX_COMPLETE	2
#define SPI_EVENT_OVR_ERR		3
#define SPI_EVENT_CRC_ERR		4


/*
 * SPI RELATED FLAGS
 */
#define SPI_RXNE_FLAG 		(1 << SPI_SR_RXNE )
#define SPI_TXE_FLAG 		(1 << SPI_SR_TXE )
#define SPI_CHSIDEE_FLAG 	(1 << SPI_SR_CHSIDEE )
#define SPI_UDR_FLAG 		(1 << SPI_SR_UDR )
#define SPI_CRCERR_FLAG 	(1 << SPI_SR_CRCERR )
#define SPI_MODF_FLAG 		(1 << SPI_SR_MODF )
#define SPI_OVR_FLAG 		(1 << SPI_SR_OVR )
#define SPI_BSY_FLAG 		(1 << SPI_SR_BSY )
#define SPI_FRE_FLAG 		(1 << SPI_SR_FRE )


//PERIPH CLK CTRL
void SPI_Peri_clk_ctrl(SPI_REG_DEF_t *pSPIx, uint8_t en_or_dis);
void SPI_Peripheral_ctrl(SPI_REG_DEF_t *pSPIx, uint8_t en_or_dis);
void SPI_SSI_config(SPI_REG_DEF_t *pSPIx, uint8_t en_or_dis);
void SPI_SSOE_config(SPI_REG_DEF_t *pSPIx, uint8_t en_or_dis);
/*
 * Init and de-init
 */
void SPI_init(SPI_Handle_t *pSPI_Handle_t);
void SPI_deinit(SPI_REG_DEF_t *pSPIx);

uint8_t SPI_GetFlagStatus(SPI_REG_DEF_t *pSPIx, uint32_t FLAGNAME);
//DATA SEND AND RECIEVE

void SPI_SendData(SPI_REG_DEF_t *pSPIx, uint8_t *pTx_buffer, uint32_t length);
void SPI_ReceiveData(SPI_REG_DEF_t *pSPIx, uint8_t *pRx_buffer, uint32_t length);

//SEND RECIEVE INTERUPT-BASED,NON-BLOCKING
uint8_t SPI_SendData_IT(SPI_Handle_t *pSPI_Handle_t, uint8_t *pTx_buffer, uint32_t length);
uint8_t SPI_ReceiveData_IT(SPI_Handle_t *pSPI_Handle_t, uint8_t *pRx_buffer, uint32_t length);

/*
 * IRQ CONFIG AND ISR HANDLING
 *
 *
 */
void SPI_IRQConfig(uint8_t IRQ_number, uint8_t IRQ_en_or_dis);
void SPI_Priority_Config(uint8_t IRQ_number, uint32_t IRQ_priority);
void SPI_IRQHandling(SPI_Handle_t *pHandle);
void SPI_Clear_OVR_Flag(SPI_REG_DEF_t *pSPIx);
void SPI_Close_Transmission(SPI_Handle_t *pSPI_Handle_t);
void SPI_Close_Reception(SPI_Handle_t *pSPI_Handle_t);

/*
 * OTHER PERIPH CTRLS
 */
void SPI_ApplicationEventCallback(SPI_Handle_t *pSPI_Handle_t, uint8_t app_event);




#endif /* INC_STM32F407XX_SPI_DRIVER_H_ */
