/*
 * 002_led_button.c
 *
 *  Created on: Apr 25, 2026
 *      Author: jameslittle
 */

#include "stm32f407xx.h"
#include "stm32f407xx_gpio_driver.h"
#include "stm32f407xx_spi_driver.h"
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
	//GPIO_spi.GPIO_pin_config.GPIO_pin_number = GPIO_PIN_6;
	//GPIO_init(&GPIO_spi);
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

int main(void){

	char user_data[] = "Hello Universe!";

	SPI_2_gpio_init();

	SPI_two_init();
	//SPI_SSI_config(SPI_2, ENABLE);
	SPI_SSOE_config(SPI_2, ENABLE);
	GPIO_button_init();


	while(1){
	while(!(GPIO_read_input_pin(GPIOA, GPIO_PIN_0)));
	SPI_Peripheral_ctrl(SPI_2, ENABLE);
	delay();
	uint8_t dataLength = strlen(user_data);
	SPI_SendData(SPI_2, &dataLength, 1);
	SPI_SendData(SPI_2, (uint8_t*)user_data, (sizeof(user_data)/sizeof(user_data[0])));
	//CONFIRM STATUS - WHILE LOOP WHILE ITS BUSY
	while(SPI_GetFlagStatus(SPI_2, SPI_BSY_FLAG));
	SPI_Peripheral_ctrl(SPI_2, DISABLE);

	}
	return 0;
};

