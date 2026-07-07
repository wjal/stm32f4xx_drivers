/*
 * 002_led_button.c
 *
 *  Created on: Apr 25, 2026
 *      Author: jameslittle
 */

#include "stm32f407xx.h"
#include "stm32f407xx_gpio_driver.h"
#include "stm32f407xx_spi_driver.h"
#include "string.h"
#include <stdio.h>
/*
 *
 * pa7 mosi
 * pa5 sclk
 * pd5
 * pa4 SPI1_NSS
 * pa6 SPI1_MISO
 */

/////command codes
#define COMMAND_LED_CTRL	0x50
#define COMMAND_SENSOR_READ	0x51
#define COMMAND_LED_READ	0x52
#define COMMAND_PRINT		0x53
#define COMMAND_ID_READ		0x54

#define LED_ON				1
#define LED_OFF				0

#define ANALOG_PIN0			0
#define ANALOG_PIN1			1
#define ANALOG_PIN2			2
#define ANALOG_PIN3			3
#define ANALOG_PIN4			4
#define ANALOG_PIN5			5

#define ARDUINO_LED_PIN 	9


void delay(void){
	for(int i = 0; i < 500000/2;i++){
	}
}

void SPI_2_gpio_init(void){
	GPIO_Handle_t GPIO_spi = {0};


	//spi mosi -- PA7
	GPIO_spi.pGPIO_x = GPIOB;
	GPIO_spi.GPIO_pin_config.GPIO_pin_number = GPIO_PIN_15;
	GPIO_spi.GPIO_pin_config.GPIO_pin_mode = GPIO_MODE_ALTFUN;
	GPIO_spi.GPIO_pin_config.GPIO_pin_speed = GPIO_OP_SPEED_HI;
	GPIO_spi.GPIO_pin_config.GPIO_pin_op_type = GPIO_OP_TYPE_PP;
	GPIO_spi.GPIO_pin_config.GPIO_pin_pdown_pup_ctrl = GPIO_NO_RESISTOR;
	GPIO_spi.GPIO_pin_config.GPIO_pin_alt_func_mode = GPIO_ALT_FUNCTION_5;

	GPIO_init(&GPIO_spi);

	//spi SCLK -- PA5
	GPIO_spi.GPIO_pin_config.GPIO_pin_number = GPIO_PIN_13;
	GPIO_init(&GPIO_spi);

	//spi NSS - PA4
	GPIO_spi.GPIO_pin_config.GPIO_pin_number = GPIO_PIN_12;
	GPIO_init(&GPIO_spi);

	//spi MISO -- PA6
	GPIO_spi.GPIO_pin_config.GPIO_pin_number = GPIO_PIN_14;
	GPIO_init(&GPIO_spi);
}

void SPI_two_init(void){
	SPI_Handle_t SPI_two;

	SPI_two.pSPIx = SPI_2;
	SPI_two.SPI_Config.SPI_DEVICE_MODE = 1;
	SPI_two.SPI_Config.SPI_BUS_CONFIG = SPI_BUS_CONFIG_FD;
	SPI_two.SPI_Config.SPI_SCLK_SPEED = SPI_BAUD_PCLK_DIV_8;
	SPI_two.SPI_Config.SPI_DFF = SPI_DFF_8_BITS;
	SPI_two.SPI_Config.SPI_CPOL = SPI_CPOL_LOW;
	SPI_two.SPI_Config.SPI_CPHA = SPI_CPHA_LOW;
	SPI_two.SPI_Config.SPI_SSM = SPI_SSM_DIS;


	SPI_init(&SPI_two);
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

uint8_t SPI_verifyResponse(uint8_t ackbyte){
	if(ackbyte==0xF5){
		//ack
		return 1;
	}else{
		//nack
		return 1;
	}
}



int main(void){

	uint8_t dummy_byte = 0xff;
	uint8_t dummy_read;

	uint8_t ackbyte = 0x0;
	uint8_t args[2];

	SPI_2_gpio_init();

	SPI_two_init();
	//SPI_SSI_config(SPI_2, ENABLE);
	SPI_SSOE_config(SPI_2, ENABLE);
	GPIO_button_init();



	while(!(GPIO_read_input_pin(GPIOA, GPIO_PIN_0)));

	delay();
	SPI_Peripheral_ctrl(SPI_2, ENABLE);

	//1. CMD_LED_CTRL <PINno(1)> <value(1)>
	uint8_t command = COMMAND_LED_CTRL;

	SPI_SendData(SPI_2, &command, 1);
	//dummy_read
	SPI_ReceiveData(SPI_2,&dummy_read,1);

	SPI_SendData(SPI_2, &dummy_byte, 1);
	//receive ack or nack
	SPI_ReceiveData(SPI_2,&ackbyte,1);
	//check ack/nack
	if(SPI_verifyResponse(ackbyte)){
		args[0]=ARDUINO_LED_PIN;
		args[1]=LED_ON;
		SPI_SendData(SPI_2, args, sizeof(args)/sizeof(args[0]));
	}
	//end of CMD_LED_CTRL

	//CONFIRM STATUS - WHILE LOOP WHILE ITS BUSY
	while(SPI_GetFlagStatus(SPI_2, SPI_BSY_FLAG));
	SPI_Peripheral_ctrl(SPI_2, DISABLE);




	while(!(GPIO_read_input_pin(GPIOA, GPIO_PIN_0)));

	delay();
	SPI_Peripheral_ctrl(SPI_2, ENABLE);

	//2. CMD_LED_CTRL <PINno(1)> <value(1)>
	command = COMMAND_SENSOR_READ;

	SPI_SendData(SPI_2, &command, 1);
	//dummy_read
	SPI_ReceiveData(SPI_2,&dummy_read,1);

	SPI_SendData(SPI_2, &dummy_byte, 1);
	//receive ack or nack
	SPI_ReceiveData(SPI_2,&ackbyte,1);
	//check ack/nack
	if(SPI_verifyResponse(ackbyte)){
		args[0]=ANALOG_PIN0;
		SPI_SendData(SPI_2, args, sizeof(args)/sizeof(args[0]));

		SPI_ReceiveData(SPI_2,&dummy_read,1);
		//add a small delay to allow the arduino to check the adc
		delay();
		//send dummy bite to request the data
		SPI_SendData(SPI_2, &dummy_byte, 1);
		uint8_t analogRead;
		SPI_ReceiveData(SPI_2,&analogRead,1);
		printf("Arduino analog pin %d value is %d\n", ANALOG_PIN0, analogRead);
	}
	//end of CMD_LED_CTRL

	//CONFIRM STATUS - WHILE LOOP WHILE ITS BUSY
	while(SPI_GetFlagStatus(SPI_2, SPI_BSY_FLAG));
	SPI_Peripheral_ctrl(SPI_2, DISABLE);

	while(!(GPIO_read_input_pin(GPIOA, GPIO_PIN_0)));

	delay();
	SPI_Peripheral_ctrl(SPI_2, ENABLE);

	//3. CMD_LED_read <msg>
	command = COMMAND_LED_READ;

	SPI_SendData(SPI_2, &command, 1);
	//dummy_read
	SPI_ReceiveData(SPI_2,&dummy_read,1);

	SPI_SendData(SPI_2, &dummy_byte, 1);
	//receive ack or nack
	SPI_ReceiveData(SPI_2,&ackbyte,1);
	//check ack/nack
	if(SPI_verifyResponse(ackbyte)){
		args[0]=ARDUINO_LED_PIN;
		SPI_SendData(SPI_2, args, 1);

		SPI_ReceiveData(SPI_2,&dummy_read,1);
		//add a small delay to allow the arduino to check the adc
		delay();
					//send dummy bite to request the data
		SPI_SendData(SPI_2, &dummy_byte, 1);
		uint8_t ledValue;
		SPI_ReceiveData(SPI_2,&ledValue,1);
		printf("Arduino pin %d is %s\n", ARDUINO_LED_PIN, ledValue ? "on":"off");
	}
	//end of CMD_LED_read

	//CONFIRM STATUS - WHILE LOOP WHILE ITS BUSY
	while(SPI_GetFlagStatus(SPI_2, SPI_BSY_FLAG));
	SPI_Peripheral_ctrl(SPI_2, DISABLE);

	while(!(GPIO_read_input_pin(GPIOA, GPIO_PIN_0)));

	delay();
	SPI_Peripheral_ctrl(SPI_2, ENABLE);

	//1. CMD_print <PINno(1)> <value(1)>
	command = COMMAND_PRINT;
	SPI_SendData(SPI_2, &command, 1);
	//dumm_read
	SPI_ReceiveData(SPI_2,&dummy_read,1);
	SPI_SendData(SPI_2, &dummy_byte, 1);
	//receive ack or nack
	SPI_ReceiveData(SPI_2,&ackbyte,1);
	//check ack/nack
	if(SPI_verifyResponse(ackbyte)){
		char message[] = "Hello Universe!";
		uint8_t dataLength = strlen(message);
		SPI_SendData(SPI_2, &dataLength, 1);
		SPI_SendData(SPI_2, (uint8_t*)message, dataLength);
	}
	//end of CMD_LED_CTRL

	//CONFIRM STATUS - WHILE LOOP WHILE ITS BUSY
	while(SPI_GetFlagStatus(SPI_2, SPI_BSY_FLAG));
	SPI_Peripheral_ctrl(SPI_2, DISABLE);



	while(!(GPIO_read_input_pin(GPIOA, GPIO_PIN_0)));

	delay();
	SPI_Peripheral_ctrl(SPI_2, ENABLE);

	//5. CMD_LED_CTRL <PINno(1)> <value(1)>
	command = COMMAND_ID_READ;

	SPI_SendData(SPI_2, &command, 1);
	//dummy_read
	SPI_ReceiveData(SPI_2,&dummy_read,1);

	SPI_SendData(SPI_2, &dummy_byte, 1);
	//receive ack or nack
	SPI_ReceiveData(SPI_2,&ackbyte,1);
	//check ack/nack
	uint8_t id[11];
	uint32_t i = 0;



	if(SPI_verifyResponse(ackbyte)){
	    delay();
	    for(i = 0; i < 10; i++){
	        SPI_SendData(SPI_2, &dummy_byte, 1);
	        SPI_ReceiveData(SPI_2, &id[i], 1);
	    }
	    id[10] = '\0';
	}

	printf("ID of device is %s\n", id);
	//end of CMD_id_Read

	//CONFIRM STATUS - WHILE LOOP WHILE ITS BUSY
	while(SPI_GetFlagStatus(SPI_2, SPI_BSY_FLAG));
	SPI_Peripheral_ctrl(SPI_2, DISABLE);

	while(1){
	}
	return 0;
};

