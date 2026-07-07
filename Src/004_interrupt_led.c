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

	GPIO_Handle_t GPIO_led = {0};

	GPIO_led.pGPIO_x = GPIOC;
	GPIO_led.GPIO_pin_config.GPIO_pin_number = GPIO_PIN_0;
	GPIO_led.GPIO_pin_config.GPIO_pin_mode = GPIO_MODE_OUT;
	GPIO_led.GPIO_pin_config.GPIO_pin_speed = GPIO_OP_SPEED_HI;
	//GPIO_led.GPIO_pin_config.GPIO_pin_op_type = GPIO_OP_TYPE_PP;
	//GPIO_led.GPIO_pin_config.GPIO_pin_pdown_pup_ctrl = GPIO_PULL_UP;
	GPIO_led.GPIO_pin_config.GPIO_pin_op_type = GPIO_OP_TYPE_PP;
	GPIO_led.GPIO_pin_config.GPIO_pin_pdown_pup_ctrl = GPIO_NO_RESISTOR;

	GPIO_Peri_clk_ctrl(GPIOC, ENABLE);

	GPIO_init(&GPIO_led);

	GPIO_Handle_t GPIO_button = {0};
	GPIO_button.pGPIO_x = GPIOD;
	GPIO_button.GPIO_pin_config.GPIO_pin_number = GPIO_PIN_5;
	GPIO_button.GPIO_pin_config.GPIO_pin_mode = GPIO_MODE_IT_FT;
	GPIO_button.GPIO_pin_config.GPIO_pin_speed = GPIO_OP_SPEED_HI;
	GPIO_button.GPIO_pin_config.GPIO_pin_pdown_pup_ctrl = GPIO_NO_RESISTOR;


	GPIO_Peri_clk_ctrl(GPIOD, ENABLE);

	GPIO_init(&GPIO_button);

	//irq configs
	GPIO_Priority_Config(IRQ_NO_EXTI_5_9, NVIC_PRIORITY_15);
	GPIO_IRQConfig(IRQ_NO_EXTI_5_9, ENABLE);

	while(1){

	};
	return 0;
};


void EXTI9_5_IRQHandler(void){
	delay();
	GPIO_IRQHandling(GPIO_PIN_5);
	GPIO_toggle_output_pin(GPIOC, GPIO_PIN_0);
}
