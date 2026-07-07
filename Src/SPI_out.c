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

void SPI_1_gpio_init(void){
	GPIO_Handle_t GPIO_spi = {0};


	//spi mosi -- PA7
	GPIO_spi.pGPIO_x = GPIOA;
	GPIO_spi.GPIO_pin_config.GPIO_pin_number = GPIO_PIN_7;
	GPIO_spi.GPIO_pin_config.GPIO_pin_mode = GPIO_MODE_ALTFUN;
	GPIO_spi.GPIO_pin_config.GPIO_pin_speed = GPIO_OP_SPEED_HI;
	GPIO_spi.GPIO_pin_config.GPIO_pin_op_type = GPIO_OP_TYPE_PP;
	GPIO_spi.GPIO_pin_config.GPIO_pin_pdown_pup_ctrl = GPIO_NO_RESISTOR;
	GPIO_spi.GPIO_pin_config.GPIO_pin_alt_func_mode = GPIO_ALT_FUNCTION_5;

	GPIO_init(&GPIO_spi);

	//spi SCLK -- PA5
	GPIO_spi.GPIO_pin_config.GPIO_pin_number = GPIO_PIN_5;
	GPIO_init(&GPIO_spi);

	//spi NSS - PA4
	//GPIO_spi.GPIO_pin_config.GPIO_pin_number = GPIO_PIN_4;
	//GPIO_init(&GPIO_spi);

	//spi MISO -- PA6
	//GPIO_spi.GPIO_pin_config.GPIO_pin_number = GPIO_PIN_6;
	//GPIO_init(&GPIO_spi);
}

void SPI_one_init(void){
	SPI_Handle_t SPI_one;

	SPI_one.pSPIx = SPI_1;
	SPI_one.SPI_Config.SPI_DEVICE_MODE = 1;
	SPI_one.SPI_Config.SPI_BUS_CONFIG = SPI_BUS_CONFIG_FD;
	SPI_one.SPI_Config.SPI_SCLK_SPEED = SPI_BAUD_PCLK_DIV_64;
	SPI_one.SPI_Config.SPI_DFF = SPI_DFF_8_BITS;
	SPI_one.SPI_Config.SPI_CPOL = SPI_CPOL_LOW;
	SPI_one.SPI_Config.SPI_CPHA = SPI_CPHA_LOW;
	SPI_one.SPI_Config.SPI_SSM = SPI_SSM_EN;


	SPI_init(&SPI_one);
}

int main(void){

	char user_data[] = "Hello Universe!";

	SPI_1_gpio_init();

	SPI_one_init();
	SPI_SSI_config(SPI_1, ENABLE);
	SPI_Peripheral_ctrl(SPI_1, ENABLE);

	SPI_SendData(SPI_1, (uint8_t*)user_data, (sizeof(user_data)/sizeof(user_data[0])));
	while(SPI_GetFlagStatus(SPI_1, SPI_BSY_FLAG));
	SPI_Peripheral_ctrl(SPI_1, DISABLE);
	while(1){

	};
	return 0;
};

