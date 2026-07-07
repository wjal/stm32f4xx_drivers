/*
 * 014_slave_strlen_Tx.c
 *
 *  Created on: Jul 7, 2026
 *      Author: jameslittle
 */


/*
 * 013_I2C_slave.c
 *
 *  Created on: Jul 6, 2026
 *      Author: jameslittle
 */

/*
 * PB6-> SCL
 * PB7 -> SDA
 */

#include "stm32f407xx.h"
#include "stm32f407xx_gpio_driver.h"
#include "stm32f407xx_i2c_driver.h"
#include <stdio.h>
#include "string.h"


#define SLAVE_ADDRESS 0x68
//redefine for this app
uint8_t Tx_Buffer[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyzABCDE";

uint32_t data_len=0;
//rcv buffer
uint8_t rcv_buf[32];


void delay(void){
	for(int i = 0; i < 500000/2;i++){
	}
}

I2C_Handle_t I2C_handle;

void IC2_1_gpio_init(void){

	GPIO_Handle_t GPIO_i2c = {0};

	GPIO_i2c.pGPIO_x = GPIOB;
	GPIO_i2c.GPIO_pin_config.GPIO_pin_number = GPIO_PIN_7;
	GPIO_i2c.GPIO_pin_config.GPIO_pin_mode = GPIO_MODE_ALTFUN;
	GPIO_i2c.GPIO_pin_config.GPIO_pin_speed = GPIO_OP_SPEED_HI;
	GPIO_i2c.GPIO_pin_config.GPIO_pin_op_type = GPIO_OP_TYPE_OPEN_DRAIN;
	GPIO_i2c.GPIO_pin_config.GPIO_pin_pdown_pup_ctrl = GPIO_PULL_UP;
	//alt func 4 on gpiob7 is sda
	GPIO_i2c.GPIO_pin_config.GPIO_pin_alt_func_mode = GPIO_ALT_FUNCTION_4;
	GPIO_init(&GPIO_i2c);
	//alt func on gpiob6 is sclk
	GPIO_i2c.GPIO_pin_config.GPIO_pin_number = GPIO_PIN_6;
	GPIO_init(&GPIO_i2c);
}

void I2C_1_init(void){

	I2C_handle.pI2Cx = I2C_1;
	I2C_handle.I2C_Config.I2C_ACKControl = I2C_ACK_ENABLE;
	I2C_handle.I2C_Config.I2C_SCLSpeed = I2C_SCL_SPEED_SM;
	//not uising fm, so can use anything
	I2C_handle.I2C_Config.I2C_FMDutyCycle = I2C_FM_DUTY_CYCLE_2;
	I2C_handle.I2C_Config.I2C_DeviceAddress = SLAVE_ADDRESS;
	//clock peripheral handled by init
	I2C_init(&I2C_handle);
}

void GPIO_button_init(void){

	GPIO_Handle_t GPIO_button;
	GPIO_button.pGPIO_x = GPIOA;
	GPIO_button.GPIO_pin_config.GPIO_pin_number = GPIO_PIN_0;
	GPIO_button.GPIO_pin_config.GPIO_pin_mode = GPIO_MODE_IN;
	GPIO_button.GPIO_pin_config.GPIO_pin_speed = GPIO_OP_SPEED_HI;
	GPIO_button.GPIO_pin_config.GPIO_pin_pdown_pup_ctrl = GPIO_NO_RESISTOR;
	GPIO_init(&GPIO_button);
}

int main(void){



	printf("Application is starting...\n");
	data_len = strlen((char*)Tx_Buffer);
	IC2_1_gpio_init();
	I2C_1_init();

	//I2C_ IRQ CONFIGS
	I2C_IRQConfig(IRQ_NO_I2C1_EV, ENABLE);
	I2C_IRQConfig(IRQ_NO_I2C1_ER, ENABLE);

	I2C_SlaveManageCallbackEvents(I2C_1,ENABLE);

	I2C_Peripheral_ctrl(I2C_1, ENABLE);
	I2C_ManageAcking(I2C_1, ENABLE);

	while(1){

	}
	return 0;
};

void I2C1_EV_IRQHandler(void){
	I2C_EV_IRQHandling(&I2C_handle);
}
void I2C1_ER_IRQHandler(void){
	I2C_ER_IRQHandling(&I2C_handle);
}

void I2C_ApplicationEventCallback(I2C_Handle_t *pI2CHandle, uint8_t app_event)
{
	static uint8_t command_code = 0x00;
	static uint32_t count = 0;
	static uint32_t w_ptr = 0;

	if(app_event == I2C_ERROR_AF)
	{
		//This will happen during slave transmitting data to master .
		// slave should understand master needs no more data
		//slave concludes end of Tx

		//if the current active code is 0x52 then dont invalidate
		if(!(command_code == 0x52)){
			command_code = 0xff;
		}
		//reset the cnt variable because its end of transmission
		count = 0;
		//Slave concludes it sent all the bytes when w_ptr reaches data_len
		if(w_ptr >= (data_len)){
			w_ptr=0;
			command_code = 0xff;
		}
	}else if (app_event == I2C_EV_STOPF){
		//This will happen during end slave reception
		//slave concludes end of Rx
		count = 0;
	}else if (app_event == I2C_EV_DATA_REQ){
		//Master is requesting for the data . send data
		if(command_code == 0x51){
			//Here we are sending 4 bytes of length information
			I2C_SlaveSendData(I2C_1,((data_len >> ((count%4) * 8)) & 0xFF));
		    count++;
		}else if (command_code == 0x52){
			//sending Tx_buf contents indexed by w_ptr variable
			I2C_SlaveSendData(I2C_1,Tx_Buffer[w_ptr++]);
		}
	}else if (app_event == I2C_EV_DATA_RCV){
		//Master has sent command code, read it
		 command_code = I2C_SlaveReceiveData(I2C_1);
	}
}
