/*
 * 002_led_button.c
 *
 *  Created on: Apr 25, 2026
 *      Author: jameslittle
 */



#include "stm32f407xx.h"
#include "stm32f407xx_gpio_driver.h"

void delay(void){
	for(int i = 0; i < 500000/2;i++){
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

	GPIO_Handle_t GPIO_button;
	GPIO_button.pGPIO_x = GPIOA;
	GPIO_button.GPIO_pin_config.GPIO_pin_number = GPIO_PIN_0;
	GPIO_button.GPIO_pin_config.GPIO_pin_mode = GPIO_MODE_IN;

	GPIO_button.GPIO_pin_config.GPIO_pin_speed = GPIO_OP_SPEED_HI;
	GPIO_led.GPIO_pin_config.GPIO_pin_pdown_pup_ctrl = GPIO_NO_RESISTOR;

	GPIO_Peri_clk_ctrl(GPIOA, ENABLE);

	GPIO_init(&GPIO_button);


	while(1){

		if(GPIO_read_input_pin(GPIOA, GPIO_PIN_0)){
			delay();
			GPIO_toggle_output_pin(GPIOD, GPIO_PIN_12);
		}


	};
	return 0;
};
