/*
 * stm32f407xx_gpio_driver.h
 *
 *  Created on: Apr 21, 2026
 *      Author: jameslittle
 */

#ifndef INC_STM32F407XX_GPIO_DRIVER_H_
#define INC_STM32F407XX_GPIO_DRIVER_H_

#include "stm32f407xx.h"

typedef struct{
	uint8_t GPIO_pin_number;
	uint8_t GPIO_pin_mode;//@GPIO_MODE
	uint8_t GPIO_pin_speed;//@GPIO_SPEED
	uint8_t GPIO_pin_pdown_pup_ctrl;
	uint8_t GPIO_pin_op_type;//@GPIO_OUTPUT_TYPE
	uint8_t GPIO_pin_alt_func_mode;//@GPIO_ALT_FUNC
}GPIO_PIN_CNFG_t;

typedef struct{
	GPIO_REG_DEF_t *pGPIO_x;
	GPIO_PIN_CNFG_t GPIO_pin_config;
}GPIO_Handle_t;

/*
 * @GPIO_PIN_NUMBER
 */
#define GPIO_PIN_0	0
#define GPIO_PIN_1	1
#define GPIO_PIN_2	2
#define GPIO_PIN_3	3
#define GPIO_PIN_4	4
#define GPIO_PIN_5	5
#define GPIO_PIN_6	6
#define GPIO_PIN_7	7
#define GPIO_PIN_8	8
#define GPIO_PIN_9	9
#define GPIO_PIN_10	10
#define GPIO_PIN_11	11
#define GPIO_PIN_12	12
#define GPIO_PIN_13	13
#define GPIO_PIN_14	14
#define GPIO_PIN_15	15


/*
 * //@GPIO_MODE
 */

#define GPIO_MODE_IN 		0
#define GPIO_MODE_OUT 		1
#define GPIO_MODE_ALTFUN	2
#define GPIO_MODE_ANALOG	3
#define GPIO_MODE_IT_FT 	4
#define GPIO_MODE_IT_RT 	5
#define GPIO_MODE_RTFT		6


/*
 * @GPIO_OUTPUT_TYPE
 *
 */

#define GPIO_OP_TYPE_PP			0
#define GPIO_OP_TYPE_OPEN_DRAIN	1

/*//@GPIO_SPEED
 * GPIO OUTPUT SPEEDS
 */

#define GPIO_OP_SPEED_LOW		0
#define GPIO_OP_SPEED_MED		1
#define GPIO_OP_SPEED_HI		2
#define GPIO_OP_SPEED_VHI		3


/*
 * GPIO pup/down
 */
#define GPIO_NO_RESISTOR		0
#define GPIO_PULL_UP 			1
#define GPIO_PULL_DOWN 			2

/*
 * @GPIO_ALT_FUNC
 */

/*
 * Peripheral clock control
 */






void GPIO_Peri_clk_ctrl(GPIO_REG_DEF_t *pGPIO_x, uint8_t en_or_dis);

/*
 * Init and de-init
 */
void GPIO_init(GPIO_Handle_t *pGPIO_handle);
void GPIO_deinit(GPIO_REG_DEF_t *pGPIO_x);

/*
 * read input
 */
uint8_t GPIO_read_input_pin(GPIO_REG_DEF_t *pGPIO_x, uint8_t pin_number);
uint16_t GPIO_read_input_port(GPIO_REG_DEF_t *pGPIOx);

/*
 * write output
 */

void GPIO_write_to_output_pin(GPIO_REG_DEF_t *pGPIO_x, uint8_t pin_number, uint8_t value);
void GPIO_write_to_output_port(GPIO_REG_DEF_t *pGPIO_x, uint16_t value);
void GPIO_toggle_output_pin(GPIO_REG_DEF_t *pGPIO_x, uint8_t pin_number);

/*
 * IRQ ISR Handling
 *
 */
void GPIO_IRQConfig(uint8_t IRQ_number, uint8_t IRQ_en_or_dis);
void GPIO_Priority_Config(uint8_t IRQ_number, uint32_t IRQ_priority);
void GPIO_IRQHandling(uint8_t pin_number);
#endif /* SRC_STM32F407XX_GPIO_DRIVER_C_ */
