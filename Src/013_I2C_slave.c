/*
 * 013_I2C_slave.c
 *
 *  Created on: Jul 6, 2026
 *      Author: jameslittle
 */
#include "stm32f407xx.h"
#include "stm32f407xx_gpio_driver.h"
#include "stm32f407xx_i2c_driver.h"
#include <stdio.h>
#include "string.h"


#define SLAVE_ADDRESS 0x68
//redefine for this app

uint8_t Tx_Buffer[32] = "Oh, hi mark!";

void delay(void){
	for(int i = 0; i < 500000/2;i++){
	}
}

I2C_Handle_t I2C_handle;

void I2C_1_gpio_init(void){

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
	I2C_1_gpio_init();
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

void I2C_ApplicationEventCallback(I2C_Handle_t *pI2C_Handle_t, uint8_t app_event){
	static uint8_t command_code=0;
	static uint8_t count=0;

	if(app_event==I2C_EV_DATA_REQ){
		//MASTER REQUESTING DATA
		if(command_code == 0x51){
			//send the length of the message
			uint8_t len = strlen((char *)Tx_Buffer);
			I2C_SlaveSendData(pI2C_Handle_t->pI2Cx, len);
		}else if(command_code == 0x52){
			//send message
			I2C_SlaveSendData(pI2C_Handle_t->pI2Cx, Tx_Buffer[count++]);
		}
	}else if(app_event==I2C_EV_DATA_RCV){
		//data waiting for slave to read
		command_code = I2C_SlaveReceiveData(pI2C_Handle_t->pI2Cx);
	}else if(app_event==I2C_ERROR_AF){
		//only happens if slave txin
		//master sent a nack, slave doesnt need more data

		//reset the command code since transmission is done
		command_code=0xff;
		count=0;
	}else if(app_event == I2C_EV_STOPF){
		//master ended i2c comm with slave

	}

}
