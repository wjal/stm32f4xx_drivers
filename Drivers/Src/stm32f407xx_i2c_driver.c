/*
 * stm32f407xx_i2c_driver.c
 *
 *  Created on: Jun 10, 2026
 *      Author: jameslittle
 */

#include "stm32f407xx_i2c_driver.h"
//PROTOTYPES


uint8_t I2C_GetFlagStatus_SR1(I2C_REG_DEF_t *pI2Cx, uint32_t FLAGNAME);

void I2C_ManageAcking(I2C_REG_DEF_t *pI2Cx, uint8_t en_or_dis){
	if(en_or_dis == I2C_ACK_ENABLE){
		//enable
		pI2Cx->I2C_CR1|= (1 << I2C_CR1_ACK);
	}else{
		//disable
		pI2Cx->I2C_CR1&= ~(1 << I2C_CR1_ACK);
	}
}

void I2C_GenerateStartCondition(I2C_REG_DEF_t *pI2Cx){
	pI2Cx->I2C_CR1|= (1 << I2C_CR1_START);
}

void I2C_GenerateStopCondition(I2C_REG_DEF_t *pI2Cx){
	pI2Cx->I2C_CR1|= (1 << I2C_CR1_STOP);
}

uint8_t I2C_GetFlagStatus_SR1(I2C_REG_DEF_t *pI2Cx, uint32_t FLAGNAME){
	//printf("Flag: %d",FLAGNAME);
	if(pI2Cx->I2C_SR1 & FLAGNAME){
			return FLAG_SET;
		}
		return FLAG_RESET;
}

static void	I2C_ExecuteAddressPhaseWrite(I2C_REG_DEF_t *pI2Cx, uint8_t slave_address){
	slave_address = slave_address << 1;
	slave_address&=~(1);
	pI2Cx->I2C_DR = slave_address;
}

static void	I2C_ExecuteAddressPhaseRead(I2C_REG_DEF_t *pI2Cx, uint8_t slave_address){
	slave_address = slave_address << 1;
	slave_address|= 1;
	pI2Cx->I2C_DR = slave_address;
}

static void I2C_MasterHandleTXE_Interrupt(I2C_Handle_t *pI2CHandle){
	if(pI2CHandle->TxRxState == I2C_BUSY_IN_TX){
		if(pI2CHandle->TxLength > 0){
			//load data
			pI2CHandle->pI2Cx->I2C_DR = *(pI2CHandle->pTxBuffer);
			//decrement the Txlen
			pI2CHandle->TxLength--;
			//increment buffer address
			pI2CHandle->pTxBuffer++;
		}
	}
}

void I2C_CloseReceiveData(I2C_Handle_t *pI2CHandle){
	//CLEAR ITBUFEN
	pI2CHandle->pI2Cx->I2C_CR2 &= ~(1 << I2C_CR2_ITBUFEN);
	//CLEAR ITEVTEN
	pI2CHandle->pI2Cx->I2C_CR2 &= ~(1 << I2C_CR2_ITEVTEN);

	pI2CHandle->TxRxState = I2C_READY;
	pI2CHandle->pRxBuffer = NULL;
	pI2CHandle->RxLength = 0;
	pI2CHandle->RxSize = 0;
	if(pI2CHandle->I2C_Config.I2C_ACKControl == I2C_ACK_ENABLE){
		I2C_ManageAcking(pI2CHandle->pI2Cx, I2C_ACK_ENABLE);
	}
}

void I2C_CloseSendData(I2C_Handle_t *pI2CHandle){
	//DISABLE OTHER INTERUPTS FOR NOW
	//CLEAR ITBUFEN
	pI2CHandle->pI2Cx->I2C_CR2 &= ~(1 << I2C_CR2_ITBUFEN);
	//CLEAR ITBUFEN
	pI2CHandle->pI2Cx->I2C_CR2 &= ~(1 << I2C_CR2_ITEVTEN);
	pI2CHandle->TxRxState = I2C_READY;
	pI2CHandle->pTxBuffer = NULL;
}


static void I2C_MasterHandleRXNE_Interrupt(I2C_Handle_t *pI2CHandle){
	if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX){
		//DATA RECEPTION
		if(pI2CHandle->RxSize == 1){
			*pI2CHandle->pRxBuffer = pI2CHandle->pI2Cx->I2C_DR;
			pI2CHandle->RxLength--;
		}
		if((pI2CHandle->RxSize >1)){
			if(pI2CHandle->RxLength ==2){
			//clear ack bit
				I2C_ManageAcking(pI2CHandle->pI2Cx, I2C_ACK_DISABLE);
			}
			//generate s stop condition
			*pI2CHandle->pRxBuffer = pI2CHandle->pI2Cx->I2C_DR;
			pI2CHandle->RxLength--;
			pI2CHandle->pRxBuffer++;
		}
		if(pI2CHandle->RxLength == 0){
			//generate the stop condition
			if(pI2CHandle->sr == I2C_DISABLE_SR){
				I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
			}
			//close rx
			I2C_CloseReceiveData(pI2CHandle);
			//notify app
			I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_RX_COMPLETE);
		}
	}
}

static void I2C_ClearAddrFlag(I2C_Handle_t *pI2C_Handle_t){
	//check for device mode

	if(pI2C_Handle_t->pI2Cx->I2C_SR2 & (1 << I2C_SR2_MSL)){
		//master mode
		if(pI2C_Handle_t->TxRxState==I2C_BUSY_IN_RX){
			if(pI2C_Handle_t->RxSize==1){
				//disable ack
				I2C_ManageAcking(pI2C_Handle_t->pI2Cx, I2C_ACK_DISABLE);
				//clear the address flag (read sr1 , sr2)
				uint32_t dummy_read = pI2C_Handle_t->pI2Cx->I2C_SR1;
				dummy_read = pI2C_Handle_t->pI2Cx->I2C_SR2;
				(void)dummy_read;
			}
		}else{
			//clear the address flag (read sr1 , sr2)
			uint32_t dummy_read = pI2C_Handle_t->pI2Cx->I2C_SR1;
			dummy_read = pI2C_Handle_t->pI2Cx->I2C_SR2;
			(void)dummy_read;
		}
	}else{
		//slave mode
		//clear the address flag (read sr1 , sr2)
		uint32_t dummy_read = pI2C_Handle_t->pI2Cx->I2C_SR1;
		dummy_read = pI2C_Handle_t->pI2Cx->I2C_SR2;
		(void)dummy_read;
	}
}


//DEFINITIONS

void I2C_Peri_clk_ctrl(I2C_REG_DEF_t *pI2Cx, uint8_t en_or_dis){
	if(en_or_dis == ENABLE){
			if(pI2Cx == I2C_1){
				I2C1_PCLK_EN();
			}else if(pI2Cx == I2C_2){
				I2C2_PCLK_EN();
			}else if(pI2Cx == I2C_3){
				I2C3_PCLK_EN();
			}
		}else{
			//to do
			if(pI2Cx == I2C_1){
				I2C1_PCLK_DIS();
			}else if(pI2Cx == I2C_2){
				I2C2_PCLK_DIS();
			}else if(pI2Cx == I2C_3){
				I2C3_PCLK_DIS();
			}
		}
}

uint32_t RCC_GetPLLOutputClock(){
	return 0;
}

void I2C_Peripheral_ctrl(I2C_REG_DEF_t *pI2Cx, uint8_t en_or_dis){
	if(en_or_dis == ENABLE){
		pI2Cx->I2C_CR1|= (1 << I2C_CR1_PE);
	}else{
		pI2Cx->I2C_CR1&= ~(1 << I2C_CR1_PE);
	}
}


uint16_t AHB_Prescaler[8] = {2,4,8,16, 32, 64, 128, 256};
uint8_t APB1_Prescaler[4] = {2,4,8,16};

uint32_t RCC_GetPCLK1Value(void){
	uint32_t pclk1, system_clock, temp, ahbp, apb1;

	uint8_t clksrc;

	clksrc = ((RCC->CFGR >> 2) & 0x3);

	if(clksrc == 0){
		//hsi
		system_clock = 16000000;
	}else if(clksrc == 1){
		//hse
		system_clock = 8000000;
	}else if(clksrc == 2){
		//pll
		system_clock  = RCC_GetPLLOutputClock();
	}

	temp = (RCC->CFGR >> 4) & 0xF;
	if(temp < 8){
		ahbp = 1;
	}else{
		ahbp = AHB_Prescaler[temp-8];
	}

	temp = (RCC->CFGR >> 10) & 0x7;
	if(temp < 4){
		apb1 = 1;
	}else{
		apb1 = APB1_Prescaler[temp-4];
	}
	pclk1 = (system_clock / ahbp) / apb1;
	return pclk1;
}



/*
 * Init and de-init
 */
void I2C_init(I2C_Handle_t *pI2C_Handle_t){
	I2C_Peri_clk_ctrl(pI2C_Handle_t->pI2Cx, ENABLE);

	uint32_t temp=0x0;

	//ACK CONTROL BIT CR1
	temp|=pI2C_Handle_t->I2C_Config.I2C_ACKControl << I2C_CR1_ACK;
	pI2C_Handle_t->pI2Cx->I2C_CR1=temp;

	//SPEED CR2
	temp=0x0;
	temp|= RCC_GetPCLK1Value() / 1000000U;
	pI2C_Handle_t->pI2Cx->I2C_CR2=(temp & 0x3F);


	//SLAVE ADDRESS
	temp=0x0;
	temp|= pI2C_Handle_t->I2C_Config.I2C_DeviceAddress << I2C_OAR1_ADD_7_1;
	//BIT 14 OF OAR1 MUST ALWAYS BE KEPT ON BY SOFTWARE
	temp|= (1 << I2C_OAR1_14_SW_ON);
	pI2C_Handle_t->pI2Cx->I2C_OAR1=temp;

	//CCR CALCULATIONS
	temp=0x0;
	uint16_t ccr_value=0x0;
	if(pI2C_Handle_t->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM){
		//STANDARD MODE
		ccr_value=RCC_GetPCLK1Value()/(2 * (pI2C_Handle_t->I2C_Config.I2C_SCLSpeed));
		temp|=(ccr_value & 0xFFF);
	}else{
		//fast mode
		temp|= (1 << I2C_CCR_FS);
		temp|=(pI2C_Handle_t->I2C_Config.I2C_FMDutyCycle << I2C_CCR_DUTY);
		if(pI2C_Handle_t->I2C_Config.I2C_FMDutyCycle == I2C_FM_DUTY_CYCLE_2){
			ccr_value=RCC_GetPCLK1Value()/(3 * (pI2C_Handle_t->I2C_Config.I2C_SCLSpeed));
		}else{
			ccr_value=RCC_GetPCLK1Value()/(25 * (pI2C_Handle_t->I2C_Config.I2C_SCLSpeed));
		}
		temp|=(ccr_value & 0xFFF);
	}
	pI2C_Handle_t->pI2Cx->I2C_CCR=temp;
	if(pI2C_Handle_t->I2C_Config.I2C_SCLSpeed <= I2C_SCL_SPEED_SM){
		temp = (RCC_GetPCLK1Value() / 1000000U) + 1;
	}else{
		temp = ((RCC_GetPCLK1Value() / 1000000U) * 300 / 1000) + 1;
	}
	pI2C_Handle_t->pI2Cx->I2C_TRISE = (temp & 0x3F);
}


void I2C_deinit(I2C_REG_DEF_t *pI2Cx){
	if(pI2Cx==I2C_1){
		I2C_1_RESET();
	}else if(pI2Cx==I2C_2){
		I2C_2_RESET();
	}else if(pI2Cx==I2C_3){
		I2C_3_RESET();}
}

//DATA SEND AND RECEIOVE
void I2C_MasterSendData(I2C_Handle_t *pI2C_Handle, uint8_t *pTxBuffer, uint32_t length, uint8_t slave_address, uint8_t sr){

	//CONFIRM START GEN IS COMPLETE
	I2C_GenerateStartCondition(pI2C_Handle->pI2Cx);
	while(!I2C_GetFlagStatus_SR1(pI2C_Handle->pI2Cx, I2C_FLAG_SB));
	//SEND THE ADDRESS OF THE SLAVE WITH R/NW BIT SET TO W(0)
	I2C_ExecuteAddressPhaseWrite(pI2C_Handle->pI2Cx, slave_address);
	while(!I2C_GetFlagStatus_SR1(pI2C_Handle->pI2Cx, I2C_FLAG_ADDR));
	I2C_ClearAddrFlag(pI2C_Handle);
	//check the txe flag
	while(length > 0){
		while(!I2C_GetFlagStatus_SR1(pI2C_Handle->pI2Cx, I2C_FLAG_TXE));
		pI2C_Handle->pI2Cx->I2C_DR=*pTxBuffer;
		pTxBuffer++;
		length--;
	}
	while(!I2C_GetFlagStatus_SR1(pI2C_Handle->pI2Cx, I2C_FLAG_TXE));
	while(!I2C_GetFlagStatus_SR1(pI2C_Handle->pI2Cx, I2C_FLAG_BTF));
	if(sr == I2C_DISABLE_SR){
		I2C_GenerateStopCondition(pI2C_Handle->pI2Cx);
	}
};

void I2C_MasterReceiveData(I2C_Handle_t *pI2C_Handle, uint8_t *pRxBuffer, uint32_t length, uint8_t slave_address, uint8_t sr){
	//generate start confition
	I2C_GenerateStartCondition(pI2C_Handle->pI2Cx);
	//confirm start generation is competed - check sb flag in sr1
	while(!I2C_GetFlagStatus_SR1(pI2C_Handle->pI2Cx, I2C_FLAG_SB));
	//send the address of the slave with r/nw set to r(1)
	I2C_ExecuteAddressPhaseRead(pI2C_Handle->pI2Cx, slave_address);
	//wait until the address phase is completed by checking the addr flagxx
	while(!I2C_GetFlagStatus_SR1(pI2C_Handle->pI2Cx, I2C_FLAG_ADDR));

	if(length == 1){
		//disable acking
		I2C_ManageAcking(pI2C_Handle->pI2Cx, I2C_ACK_DISABLE);
		///clear he addr flag
		I2C_ClearAddrFlag(pI2C_Handle);
		//wait til rxne becomes 1
		while(!I2C_GetFlagStatus_SR1(pI2C_Handle->pI2Cx, I2C_FLAG_RXNE));
		//generate s stop condition
		if(sr == I2C_DISABLE_SR){
				I2C_GenerateStopCondition(pI2C_Handle->pI2Cx);
			}		//read data in to buffer
		*pRxBuffer = pI2C_Handle->pI2Cx->I2C_DR;
	}

	if(length > 1){
		//wait til rxne becomes 1
		I2C_ClearAddrFlag(pI2C_Handle);

		for(uint32_t i = length; i > 0; i--){
			while(!I2C_GetFlagStatus_SR1(pI2C_Handle->pI2Cx, I2C_FLAG_RXNE));
			if(i==2){
				//clear ack bit
				I2C_ManageAcking(pI2C_Handle->pI2Cx, I2C_ACK_DISABLE);
				//generate s stop condition
				if(sr == I2C_DISABLE_SR){
						I2C_GenerateStopCondition(pI2C_Handle->pI2Cx);
					}			}
			*pRxBuffer = pI2C_Handle->pI2Cx->I2C_DR;
			pRxBuffer++;
		}
	}
	//RE-ENABLE ACKING IF NECESSARY
	if(pI2C_Handle->I2C_Config.I2C_ACKControl == I2C_ACK_ENABLE){
		I2C_ManageAcking(pI2C_Handle->pI2Cx, I2C_ACK_ENABLE);
	}
}

uint8_t  I2C_MasterSendData_IT(I2C_Handle_t *pI2C_Handle, uint8_t *pTxBuffer, uint32_t length, uint8_t slave_address, uint8_t sr){

	uint8_t busystate = pI2C_Handle->TxRxState;

	if( (busystate != I2C_BUSY_IN_TX) && (busystate != I2C_BUSY_IN_RX))
	{
		pI2C_Handle->pTxBuffer = pTxBuffer;
		pI2C_Handle->TxLength = length;
		pI2C_Handle->TxRxState = I2C_BUSY_IN_TX;
		pI2C_Handle->devAddress = slave_address;
		pI2C_Handle->sr = sr;
		//Implement code to Generate START Condition
		I2C_GenerateStartCondition(pI2C_Handle->pI2Cx);

		//Implement the code to enable ITBUFEN Control Bit
		pI2C_Handle->pI2Cx->I2C_CR2 |= ( 1 << I2C_CR2_ITBUFEN);

		//Implement the code to enable ITEVFEN Control Bit
		pI2C_Handle->pI2Cx->I2C_CR2 |= ( 1 << I2C_CR2_ITEVTEN);


		//Implement the code to enable ITERREN Control Bit
		pI2C_Handle->pI2Cx->I2C_CR2 |= ( 1 << I2C_CR2_ITERREN);


	}
	return busystate;

}

uint8_t I2C_MasterReceiveData_IT(I2C_Handle_t *pI2C_Handle, uint8_t *pRxBuffer, uint32_t length, uint8_t slave_address, uint8_t sr)
{

	uint8_t busystate = pI2C_Handle->TxRxState;

	if( (busystate != I2C_BUSY_IN_TX) && (busystate != I2C_BUSY_IN_RX))
	{
		pI2C_Handle->pRxBuffer = pRxBuffer;
		pI2C_Handle->RxLength = length;
		pI2C_Handle->TxRxState = I2C_BUSY_IN_RX;
		pI2C_Handle->RxSize = length; //Rxsize is used in the ISR code to manage the data reception
		pI2C_Handle->devAddress = slave_address;
		pI2C_Handle->sr = sr;

		//Implement code to Generate START Condition
		I2C_GenerateStartCondition(pI2C_Handle->pI2Cx);

		//Implement the code to enable ITBUFEN Control Bit
		pI2C_Handle->pI2Cx->I2C_CR2 |= ( 1 << I2C_CR2_ITBUFEN);

		//Implement the code to enable ITEVFEN Control Bit
		pI2C_Handle->pI2Cx->I2C_CR2 |= ( 1 << I2C_CR2_ITEVTEN);

		//Implement the code to enable ITERREN Control Bit
		pI2C_Handle->pI2Cx->I2C_CR2 |= ( 1 << I2C_CR2_ITERREN);
	}

	return busystate;
}

void I2C_SlaveSendData(I2C_REG_DEF_t *pI2Cx, uint8_t data){
	pI2Cx->I2C_DR = data;
}
uint8_t I2C_SlaveReceiveData(I2C_REG_DEF_t *pI2Cx){
	return (uint8_t)pI2Cx->I2C_DR;
}


/*
 * IRQ CONFIG AND ISR HANDLING
 *
 *
 */

void I2C_IRQConfig(uint8_t IRQ_number, uint8_t IRQ_en_or_dis){
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

void I2C_Priority_Config(uint8_t IRQ_number, uint32_t IRQ_priority){
	uint8_t iprx = IRQ_number/4;
	uint8_t iprx_section = IRQ_number%4;
	uint8_t priority_shift = (8 * iprx_section ) + (8 - NO_PRIORITY_BITS);
	*(NVIC_PRIORITY_BASE + (iprx))|= (IRQ_priority << priority_shift);
}

void I2C_EV_IRQHandling(I2C_Handle_t *pI2CHandle)
{

	//Interrupt handling for both master and slave mode of a device
	uint32_t temp_1, temp_2, temp_3;

	temp_1 = pI2CHandle->pI2Cx->I2C_CR2 & (1 << I2C_CR2_ITEVTEN);
	temp_2 = pI2CHandle->pI2Cx->I2C_CR2 & (1 << I2C_CR2_ITBUFEN);
	temp_3 = pI2CHandle->pI2Cx->I2C_SR1 & (1 << I2C_SR1_SB);

	//1. Handle For interrupt generated by SB event
	//	Note : SB flag is only applicable in Master mode
	if(temp_1 && temp_3){
		//SB FLAG SET
		//WILL ONLY BE EXECUTED IN MASTER SINCE SB IS 0 FOR SLAVE
		//EX ADDRESS PHASE
		if(pI2CHandle->TxRxState == I2C_BUSY_IN_TX){
			I2C_ExecuteAddressPhaseWrite(pI2CHandle->pI2Cx, pI2CHandle->devAddress);
		}else if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX){
			I2C_ExecuteAddressPhaseRead(pI2CHandle->pI2Cx, pI2CHandle->devAddress);
		}

	}

	temp_3 = pI2CHandle->pI2Cx->I2C_SR1 & (1 << I2C_SR1_ADDR);

	//2. Handle For interrupt generated by ADDR event
	//Note : When master mode : Address is sent
	//		 When Slave mode   : Address matched with own address
	if(temp_1 && temp_3){
		//ADDR FLAG SET
		//cLEAR IT
		I2C_ClearAddrFlag(pI2CHandle);
	}

	temp_3 = pI2CHandle->pI2Cx->I2C_SR1 & (1 << I2C_SR1_BTF);
	//3. Handle For interrupt generated by BTF(Byte Transfer Finished) event
	if(temp_1 && temp_3){
	//BTF FLAG SET
		if(pI2CHandle->TxRxState == I2C_BUSY_IN_TX){
			//CEHCK IF TX IS SET
			if(pI2CHandle->pI2Cx->I2C_SR1 & (1 << I2C_SR1_TXE)){
				//BTF, TXE 1
				if(pI2CHandle->TxLength == 0){
					//generate stop
					if(pI2CHandle->sr == I2C_DISABLE_SR){
						I2C_GenerateStopCondition(pI2CHandle->pI2Cx);
					}
					//reset member elements of the handle structure
					I2C_CloseSendData(pI2CHandle);

					//notify app about app complete
					I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_TXE_COMPLETE);
				}
			}
		}else if(pI2CHandle->TxRxState == I2C_BUSY_IN_RX){
			//CHECK IF RXNE
		}

	}

	temp_3 = pI2CHandle->pI2Cx->I2C_SR1 & (1 << I2C_SR1_STOPF);

	//4. Handle For interrupt generated by STOPF event
	// Note : Stop detection flag is applicable only slave mode . For master this flag will never be set
	if(temp_1 && temp_3){
		//STOPF FLAG SET
		//clea the flag by read sr1 and write to cr1. SR1 READ HAS BEEN DONE ALREADY ABOVE
		//write nothing over top
		pI2CHandle->pI2Cx->I2C_CR1|= 0x0000;
		I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_STOPF);

	}

	temp_3 = pI2CHandle->pI2Cx->I2C_SR1 & (1 << I2C_SR1_TXE);
	//5. Handle For interrupt generated by TXE event
	if(temp_1 && temp_2 && temp_3){
		//check if master
		if(pI2CHandle->pI2Cx->I2C_SR2 & (1 << I2C_SR2_MSL)){
			//TXE FLAG SET
			//TRANSMIT DATA
			I2C_MasterHandleTXE_Interrupt(pI2CHandle);
		}else{
			//slave
			if(pI2CHandle->pI2Cx->I2C_SR2 & ( 1 << I2C_SR2_TRA)){
				I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_DATA_REQ);
			}
		}
	}
	//6. Handle For interrupt generated by RXNE event
	temp_3 = pI2CHandle->pI2Cx->I2C_SR1 & (1 << I2C_SR1_RXNE);

	if(temp_1 && temp_2 && temp_3){
	//RXNE FLAG SET
		if(pI2CHandle->pI2Cx->I2C_SR2 & (1 << I2C_SR2_MSL)){
			//ONLY MASTER
			I2C_MasterHandleRXNE_Interrupt(pI2CHandle);
		}else{
			//slave
			if(!(pI2CHandle->pI2Cx->I2C_SR2 & ( 1 << I2C_SR2_TRA))){
				I2C_ApplicationEventCallback(pI2CHandle, I2C_EV_DATA_RCV);
			}
		}
	}
}

void I2C_SlaveManageCallbackEvents(I2C_REG_DEF_t *pI2Cx, uint8_t en_or_dis){
	if(en_or_dis==ENABLE){
		//Implement the code to enable ITBUFEN Control Bit
		pI2Cx->I2C_CR2 |= ( 1 << I2C_CR2_ITBUFEN);

		//Implement the code to enable ITEVFEN Control Bit
		pI2Cx->I2C_CR2 |= ( 1 << I2C_CR2_ITEVTEN);


		//Implement the code to enable ITERREN Control Bit
		pI2Cx->I2C_CR2 |= ( 1 << I2C_CR2_ITERREN);
	}else{
		pI2Cx->I2C_CR2 &= ~( 1 << I2C_CR2_ITBUFEN);

		//Implement the code to enable ITEVFEN Control Bit
		pI2Cx->I2C_CR2 &= ~( 1 << I2C_CR2_ITEVTEN);


		//Implement the code to enable ITERREN Control Bit
		pI2Cx->I2C_CR2 &= ~( 1 << I2C_CR2_ITERREN);
	}
}


void I2C_ER_IRQHandling(I2C_Handle_t *pI2CHandle)
{

	uint32_t temp1,temp2;

    //Know the status of  ITERREN control bit in the CR2
	temp2 = (pI2CHandle->pI2Cx->I2C_CR2) & ( 1 << I2C_CR2_ITERREN);


/***********************Check for Bus error************************************/
	temp1 = (pI2CHandle->pI2Cx->I2C_SR1) & ( 1<< I2C_SR1_BERR);
	if(temp1  && temp2 )
	{
		//This is Bus error

		//Implement the code to clear the buss error flag
		pI2CHandle->pI2Cx->I2C_SR1 &= ~( 1 << I2C_SR1_BERR);

		//Implement the code to notify the application about the error
	   I2C_ApplicationEventCallback(pI2CHandle,I2C_ERROR_BERR);
	}

/***********************Check for arbitration lost error************************************/
	temp1 = (pI2CHandle->pI2Cx->I2C_SR1) & ( 1 << I2C_SR1_ARLO );
	if(temp1  && temp2)
	{
		//This is arbitration lost error

		//Implement the code to clear the arbitration lost error flag
		pI2CHandle->pI2Cx->I2C_SR1 &= ~( 1 << I2C_SR1_ARLO);

		//Implement the code to notify the application about the error
		I2C_ApplicationEventCallback(pI2CHandle,I2C_ERROR_ARLO);

	}

/***********************Check for ACK failure  error************************************/

	temp1 = (pI2CHandle->pI2Cx->I2C_SR1) & ( 1 << I2C_SR1_AF);
	if(temp1  && temp2)
	{
		//This is ACK failure error

	    //Implement the code to clear the ACK failure error flag
		pI2CHandle->pI2Cx->I2C_SR1 &= ~( 1 << I2C_SR1_AF);

		//Implement the code to notify the application about the error
		I2C_ApplicationEventCallback(pI2CHandle,I2C_ERROR_AF);

	}

/***********************Check for Overrun/underrun error************************************/
	temp1 = (pI2CHandle->pI2Cx->I2C_SR1) & ( 1 << I2C_SR1_OVR);
	if(temp1  && temp2)
	{
		//This is Overrun/underrun

	    //Implement the code to clear the Overrun/underrun error flag
		pI2CHandle->pI2Cx->I2C_SR1 &= ~( 1 << I2C_SR1_OVR);

		//Implement the code to notify the application about the error
		I2C_ApplicationEventCallback(pI2CHandle,I2C_ERROR_OVR);

	}

/***********************Check for Time out error************************************/
	temp1 = (pI2CHandle->pI2Cx->I2C_SR1) & ( 1 << I2C_SR1_TIMEOUT);
	if(temp1  && temp2)
	{
		//This is Time out error

	    //Implement the code to clear the Time out error flag
		pI2CHandle->pI2Cx->I2C_SR1 &= ~( 1 << I2C_SR1_TIMEOUT);

		//Implement the code to notify the application about the error
		I2C_ApplicationEventCallback(pI2CHandle,I2C_ERROR_TIMEOUT);

	}

}

/*
 * OTHER PERIPH CTRLS
 */
__attribute__((weak)) void  I2C_ApplicationEventCallback(I2C_Handle_t *pI2C_Handle_t, uint8_t app_event){
//this is a weak function and should be overridden in the application
}
