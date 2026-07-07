/*
 * 002_led_button.c
 *
 *  Created on: July 4, 2026
 *      Author: jameslittle
 */

#include "stm32f407xx.h"
#include "stm32f407xx_gpio_driver.h"
#include "stm32f407xx_i2c_driver.h"
#include <stdio.h>
#include "string.h"

/*
 *
 * pa7 mosi
 * pa5 sclk
 * pd5
 * pa4 SPI1_NSS
 * pa6 SPI1_MISO
 */
#define MY_ADDRESS 0x61
#define SLAVE_ADDRESS 0x68

volatile uint8_t Rx_complete = RESET;

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
	I2C_handle.I2C_Config.I2C_DeviceAddress = MY_ADDRESS;
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

	uint8_t rx_buff[32];
	uint8_t length;
	printf("Application is starting...\n");
	I2C_1_gpio_init();
	I2C_1_init();

	//I2C_ IRQ CONFIGS
	I2C_IRQConfig(IRQ_NO_I2C1_EV, ENABLE);
	I2C_IRQConfig(IRQ_NO_I2C1_ER, ENABLE);

	I2C_Peripheral_ctrl(I2C_1, ENABLE);
	I2C_ManageAcking(I2C_1, ENABLE);

	//SEND SOME DATA
	GPIO_button_init();
	for(int i = 0; i < 5; i++){
		delay();
	}
	printf("Setup complete.\n");
	int a = 0;
	while(1){
		if(a < 5){
			printf("waiting...\n");
		}
		while(!(GPIO_read_input_pin(GPIOA, GPIO_PIN_0)));
		printf("pressed\n");
		delay();
		//SEND CODE TO SLAVE TO GET LENGTH OF MESSAGE FIRST
		uint8_t command_code = 0x51;
		while(I2C_MasterSendData_IT(&I2C_handle, &command_code, 1, (uint8_t)SLAVE_ADDRESS, I2C_ENABLE_SR)!=I2C_READY);
		while(I2C_MasterReceiveData_IT(&I2C_handle, &length, 1, (uint8_t)SLAVE_ADDRESS, I2C_ENABLE_SR)!=I2C_READY);
		command_code = 0x52;
		while(I2C_MasterSendData_IT(&I2C_handle, &command_code, 1, (uint8_t)SLAVE_ADDRESS, I2C_ENABLE_SR)!=I2C_READY);
		while(I2C_MasterReceiveData_IT(&I2C_handle, rx_buff, length, (uint8_t)SLAVE_ADDRESS, I2C_DISABLE_SR)!=I2C_READY);
		Rx_complete = RESET;
		while(Rx_complete != SET);
		rx_buff[length] = '\0';
		printf("%s",rx_buff);
		Rx_complete = RESET;
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
	if(app_event==I2C_EV_TXE_COMPLETE){
	}else if(app_event==I2C_EV_RX_COMPLETE){
		Rx_complete = SET;
	}else if(app_event==I2C_ERROR_AF){
		printf("ACK ERROR!\n");
		//FOR MASTER, ACK ERROR OCCURS WHEN SLAVE DOES NOT SEND ACK
		I2C_CloseSendData(&I2C_handle);
		I2C_GenerateStopCondition(I2C_1);
		//doesnt make sense for any more code to execute in this application
		while(1);
	}
}
