/*
 * stm32f4xx_spi_driver.c
 *
 *  Created on: Apr 28, 2026
 *      Author: jameslittle
 */
#include "stm32f407xx_spi_driver.h"


static void spi_txe_interrupt_handle(SPI_Handle_t *pHandle);

//handle RXE
static void spi_rxe_interrupt_handle(SPI_Handle_t *pHandle);

//handle OVR ERROR
static void spi_ovr_err_interrupt_handle(SPI_Handle_t *pHandle);



void SPI_Peri_clk_ctrl(SPI_REG_DEF_t *pSPIx, uint8_t en_or_dis){
	if(en_or_dis == ENABLE){
		if(pSPIx == SPI_1){
			SPI1_PCLK_EN();
		}else if(pSPIx == SPI_2){
			SPI2_PCLK_EN();
		}else if(pSPIx == SPI_3){
			SPI3_PCLK_EN();
		}else if(pSPIx == SPI_4){
			SPI4_PCLK_EN();
		}
	}else{
		//to do
		if(pSPIx == SPI_1){
			SPI1_PCLK_DIS();
		}else if(pSPIx == SPI_2){
			SPI2_PCLK_DIS();
		}else if(pSPIx == SPI_3){
			SPI3_PCLK_DIS();
		}else if(pSPIx == SPI_4){
			SPI4_PCLK_DIS();
		}
	}
}


/*
 * Init and de-init
 */
void SPI_init(SPI_Handle_t *pSPI_Handle_t){

	SPI_Peri_clk_ctrl(pSPI_Handle_t->pSPIx, ENABLE);

	uint32_t temp = 0x0;

	//config device mode
	temp|= (pSPI_Handle_t->SPI_Config.SPI_DEVICE_MODE << SPI_CR1_MSTR);

	if(pSPI_Handle_t->SPI_Config.SPI_BUS_CONFIG == SPI_BUS_CONFIG_FD){
		//bidi mode cleared
		temp &=~( 1 << SPI_CR1_BIDIMODE);
	}else if(pSPI_Handle_t->SPI_Config.SPI_BUS_CONFIG == SPI_BUS_CONFIG_HD){
		//bidi mode show be set
		temp |= ( 1 << SPI_CR1_BIDIMODE);
	}else if(pSPI_Handle_t->SPI_Config.SPI_BUS_CONFIG == SPI_BUS_CONFIG_SIMPLEX_RX_ONLY){
		//bidi cleared, with receive on
		temp &=~( 1 << SPI_CR1_BIDIMODE);
		temp |= (1 << SPI_CR1_RXONLY);
	}

	//CONFIG SPI_SCLK_SPEED
	temp |= pSPI_Handle_t->SPI_Config.SPI_SCLK_SPEED << SPI_CR1_BR;
	//CONFIG SPI_DFF
	temp |= pSPI_Handle_t->SPI_Config.SPI_DFF << SPI_CR1_DFF;
	//CONFIG SPI CPOL
	temp |= pSPI_Handle_t->SPI_Config.SPI_CPOL << SPI_CR1_CPOL;
	//CONFIG SPI_CHPA
	temp |= pSPI_Handle_t->SPI_Config.SPI_CPHA << SPI_CR1_CPHA;

	temp |= pSPI_Handle_t->SPI_Config.SPI_SSM << SPI_CR1_SSM;

	pSPI_Handle_t->pSPIx->CR1 = temp;

}

void SPI_deinit(SPI_REG_DEF_t *pSPIx){
	if(pSPIx==SPI_1){
		SPI_1_RESET();
	}else if(pSPIx==SPI_2){
		SPI_2_RESET();
	}else if(pSPIx==SPI_3){
		SPI_3_RESET();}
	else if(pSPIx==SPI_4){
		SPI_4_RESET();
	}
}

void SPI_Peripheral_ctrl(SPI_REG_DEF_t *pSPIx, uint8_t en_or_dis){
	if(en_or_dis == ENABLE){
		pSPIx->CR1|= (1 << SPI_CR1_SPE);
	}else{
		pSPIx->CR1&= ~(1 << SPI_CR1_SPE);
	}
}

void SPI_SSI_config(SPI_REG_DEF_t *pSPIx, uint8_t en_or_dis){
	if(en_or_dis == ENABLE){
		pSPIx->CR1|= (1 << SPI_CR1_SSI);
	}else{
		pSPIx->CR1&= ~(1 << SPI_CR1_SSI);
	}
}

void SPI_SSOE_config(SPI_REG_DEF_t *pSPIx, uint8_t en_or_dis){
	if(en_or_dis == ENABLE){
		pSPIx->CR2|= (1 << SPI_CR2_SSOE);
	}else{
		pSPIx->CR2&= ~(1 << SPI_CR2_SSOE);
	}
}


uint8_t SPI_GetFlagStatus(SPI_REG_DEF_t *pSPIx, uint32_t FLAGNAME){
	if(pSPIx->SR & FLAGNAME){
		return FLAG_SET;
	}
	return FLAG_RESET;
}

//DATA SEND AND RECIEVE
//THIS IS A BLOCKING CALL ---> THE WHILE LOOPS HAVE POTENTIAL TO HANG PERMANENTALY
void SPI_SendData(SPI_REG_DEF_t *pSPIx, uint8_t *pTx_buffer, uint32_t length){
	while(length > 0){
		//WAIT FOR TX BUFFER TO EMPTY
		while(SPI_GetFlagStatus(pSPIx, SPI_TXE_FLAG) == FLAG_RESET);

		if(pSPIx->CR1 & (1 << SPI_CR1_DFF)){
			//16 BIT FRAME
			pSPIx->DR = *((uint16_t*)pTx_buffer);
			//decrease for two bytes
			length--;
			length--;
			pTx_buffer += 2;
		}else{
			//8 bit frame
			pSPIx->DR = *pTx_buffer;
						//decrease for 1 byte
			length--;
			pTx_buffer++;
		}
	}
}

void SPI_ReceiveData(SPI_REG_DEF_t *pSPIx, uint8_t *pRx_buffer, uint32_t length){
	while(length > 0){
			//wait until rx free
		while(SPI_GetFlagStatus(pSPIx, SPI_RXNE_FLAG) == FLAG_RESET);

		if(pSPIx->CR1 & (1 << SPI_CR1_DFF)){
		//16 BIT FRAME
		//read into buff
			*((uint16_t*)pRx_buffer) = pSPIx->DR;
			//decrease for two bytes
			length--;
			length--;
			pRx_buffer += 2;
		}else{
			//8 bit frame
			*(uint8_t*)pRx_buffer = pSPIx->DR;
			//decrease for 1 byte
			length--;
			pRx_buffer++;
		}
	}
}



//DATA SEND AND RECIEVE
//THIS IS A BLOCKING CALL ---> THE WHILE LOOPS HAVE POTENTIAL TO HANG PERMANENTALY
uint8_t SPI_SendData_IT(SPI_Handle_t *pSPI_Handle_t, uint8_t *pTx_buffer, uint32_t length){
	//GET STATE
	uint8_t state = pSPI_Handle_t->TxState;

	if(state != SPI_BUSY_IN_TX){
		//save the buffer address and the length of buffer
		pSPI_Handle_t->pTxBuffer = pTx_buffer;
		pSPI_Handle_t->TxLength = length;
		//mark txstate as busy
		pSPI_Handle_t->TxState = SPI_BUSY_IN_TX;
		//enable the txeie bit
		pSPI_Handle_t->pSPIx->CR2 |= (1 << SPI_CR2_TXEIE);
	}
	return state;
}

uint8_t SPI_ReceiveData_IT(SPI_Handle_t *pSPI_Handle_t, uint8_t *pRx_buffer, uint32_t length){
	uint8_t state = pSPI_Handle_t->RxState;

	if(state != SPI_BUSY_IN_RX){
		//save the buffer address and the length of buffer
		pSPI_Handle_t->pRxBuffer = pRx_buffer;
		pSPI_Handle_t->RxLength = length;
		//mark txstate as busy
		pSPI_Handle_t->RxState = SPI_BUSY_IN_RX;
		//enable the txeie bit
		pSPI_Handle_t->pSPIx->CR2 |= (1 << SPI_CR2_RXNEIE);
	}
	return state;

}




/*
 * IRQ CONFIG AND ISR HANDLING
 *
 *
 */
void SPI_IRQConfig(uint8_t IRQ_number, uint8_t IRQ_en_or_dis){
	if(IRQ_en_or_dis==ENABLE){
		if(IRQ_number <= 31){
			//program ISER0
			*NVIC_ISER0|=(1 << IRQ_number);
		}else if(IRQ_number > 31 && IRQ_number < 64){
			//program ISER1
			*NVIC_ISER1|=(1 << IRQ_number%32);
		}else if(IRQ_number >= 64 && IRQ_number < 96){
			//program ISER2
			*NVIC_ISER2|=(1 << IRQ_number%32);
		}
	}else{
		if(IRQ_number <= 31){
			//program ICER0
			*NVIC_ICER0|=(1 << IRQ_number);
		}else if(IRQ_number > 31 && IRQ_number < 64){
			//program ISER1
			*NVIC_ICER1|=(1 << IRQ_number%32);
		}else if(IRQ_number >= 64 && IRQ_number < 96){
			//program ISER2
			*NVIC_ICER2|=(1 << IRQ_number%32);
		}
	}
}

void SPI_Priority_Config(uint8_t IRQ_number, uint32_t IRQ_priority){
	uint8_t iprx = IRQ_number/4;
	uint8_t iprx_section = IRQ_number%4;
	uint8_t priority_shift = (8 * iprx_section ) + (8 - NO_PRIORITY_BITS);
	*(NVIC_PRIORITY_BASE + (iprx))|= (IRQ_priority << priority_shift);

}
void SPI_IRQHandling(SPI_Handle_t *pHandle){
	//CHECK WHAT CAUSED INTERUPT
	uint8_t temp_1, temp_2;
	//CHECK FOR TXE
	temp_1 = pHandle->pSPIx->SR & (1 << SPI_SR_TXE);
	temp_2 = pHandle->pSPIx->CR2 & (1 << SPI_CR2_TXEIE);

	if(temp_1 && temp_2){
		//handle txe
		spi_txe_interrupt_handle(pHandle);
	}

	temp_1 = pHandle->pSPIx->SR & (1 << SPI_SR_RXNE);
	temp_2 = pHandle->pSPIx->CR2 & (1 << SPI_CR2_RXNEIE);

	if(temp_1 && temp_2){
		//handle RXE
		spi_rxe_interrupt_handle(pHandle);
	}

	temp_1 = pHandle->pSPIx->SR & (1 << SPI_SR_OVR);
	temp_2 = pHandle->pSPIx->CR2 & (1 << SPI_CR2_ERRIE);

	if(temp_1 && temp_2){
		//handle OVR ERROR
		spi_ovr_err_interrupt_handle(pHandle);
	}


};




static void spi_txe_interrupt_handle(SPI_Handle_t *pHandle){

	if(pHandle->pSPIx->CR1 & (1 << SPI_CR1_DFF)){
		//16 BIT FRAME
		pHandle->pSPIx->DR = *((uint16_t*)pHandle->pTxBuffer);
		//decrease for two bytes
		pHandle->TxLength--;
		pHandle->TxLength--;
		pHandle->pTxBuffer += 2;
	}else{
	//8 bit frame
		pHandle->pSPIx->DR = *pHandle->pTxBuffer;
		//decrease for 1 byte
		pHandle->TxLength--;
		pHandle->pTxBuffer++;
	}
	if(!pHandle->TxLength){
		//spi tx is over, end transmission
		//PREVENTS INTERUPTS FROM TXE FLAG
		SPI_Close_Transmission(pHandle);
		SPI_ApplicationEventCallback(pHandle, SPI_EVENT_TX_COMPLETE);
	}
}

//handle RXE
static void spi_rxe_interrupt_handle(SPI_Handle_t *pHandle){
	if(pHandle->pSPIx->CR1 & (1 << SPI_CR1_DFF)){
	//16 BIT FRAME
		*((uint16_t*)pHandle->pRxBuffer) = pHandle->pSPIx->DR;
		//decrease for two bytes
		pHandle->RxLength--;
		pHandle->RxLength--;
		pHandle->pRxBuffer += 2;
	}else{
	//8 bit frame
		*pHandle->pRxBuffer = pHandle->pSPIx->DR;
		//decrease for 1 byte
		pHandle->RxLength--;
		pHandle->pRxBuffer++;
	}
	if(!pHandle->RxLength){
		//spi tx is over, end transmission
		//PREVENTS INTERUPTS FROM TXE FLAG
		SPI_Close_Reception(pHandle);
		SPI_ApplicationEventCallback(pHandle, SPI_EVENT_RX_COMPLETE);
	}

}

//handle OVR ERROR
static void spi_ovr_err_interrupt_handle(SPI_Handle_t *pHandle){
	//CLEAR THE OVR FLAG
	//ALERT APP
	uint8_t temp;
	if(pHandle->TxState != SPI_BUSY_IN_TX){
		temp = pHandle->pSPIx->DR;
		temp = pHandle->pSPIx->SR;
	}
	(void)temp;
	SPI_ApplicationEventCallback(pHandle, SPI_EVENT_OVR_ERR);
}

void SPI_Clear_OVR_Flag(SPI_REG_DEF_t *pSPIx){
	uint8_t temp;

	temp = pSPIx->DR;
	temp = pSPIx->SR;
	(void)temp;
}
void SPI_Close_Transmission(SPI_Handle_t *pSPI_Handle_t){
	pSPI_Handle_t->pSPIx->CR2 &= ~(1 << SPI_CR2_TXEIE);
	pSPI_Handle_t->pTxBuffer = NULL;
	pSPI_Handle_t->TxLength = 0;
	pSPI_Handle_t->TxState = SPI_READY;
}
void SPI_Close_Reception(SPI_Handle_t *pSPI_Handle_t){
	pSPI_Handle_t->pSPIx->CR2 &= ~(1 << SPI_CR2_RXNEIE);
	pSPI_Handle_t->pRxBuffer = NULL;
	pSPI_Handle_t->RxState = SPI_READY;
	pSPI_Handle_t->RxLength = 0;
}

__attribute__((weak)) void SPI_ApplicationEventCallback(SPI_Handle_t *pSPI_Handle_t, uint8_t app_event){
//this is a weak function and should be overridden in the application
}

