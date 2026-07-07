/*
 * 001_led_toggle.c
 *
 *  Created on: Apr 23, 2026
 *      Author: jameslittle
 */
#include "stm32f407xx.h"
#include "stm32f407xx_gpio_driver.h"

void delay(void){
	for(int i = 0; i < 500000;i++){
	}
}

int main(void){

	GPIO_Handle_t GPIO_led;

	GPIO_led.pGPIO_x = GPIOD;
	GPIO_led.GPIO_pin_config.GPIO_pin_number = GPIO_PIN_12;
	GPIO_led.GPIO_pin_config.GPIO_pin_mode = GPIO_MODE_OUT;
	GPIO_led.GPIO_pin_config.GPIO_pin_speed = GPIO_OP_SPEED_HI;
	//GPIO_led.GPIO_pin_config.GPIO_pin_op_type = GPIO_OP_TYPE_PP;
	//GPIO_led.GPIO_pin_config.GPIO_pin_pdown_pup_ctrl = GPIO_PULL_UP;
	GPIO_led.GPIO_pin_config.GPIO_pin_op_type = GPIO_OP_TYPE_OPEN_DRAIN;
	GPIO_led.GPIO_pin_config.GPIO_pin_pdown_pup_ctrl = GPIO_PULL_UP;

	GPIO_Peri_clk_ctrl(GPIOD, ENABLE);

	GPIO_init(&GPIO_led);

	while(1){
		GPIO_toggle_output_pin(GPIOD, GPIO_PIN_12);
		delay();
	};
	return 0;
};
