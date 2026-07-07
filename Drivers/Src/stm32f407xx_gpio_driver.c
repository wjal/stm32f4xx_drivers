/*
 * stm32f407xx_gpio_driver.c
 *
 *  Created on: Apr 21, 2026
 *      Author: jameslittle
 */

#include "stm32f407xx_gpio_driver.h"


void GPIO_Peri_clk_ctrl(GPIO_REG_DEF_t *pGPIO_x, uint8_t en_or_dis){
	if(en_or_dis==ENABLE){
		if(pGPIO_x==GPIOA){
			GPIOA_PCLK_EN();
		}else if(pGPIO_x==GPIOB){
			GPIOB_PCLK_EN();
		}else if(pGPIO_x==GPIOC){
			GPIOC_PCLK_EN();
		}else if(pGPIO_x==GPIOD){
			GPIOD_PCLK_EN();
		}else if(pGPIO_x==GPIOE){
			GPIOE_PCLK_EN();
		}else if(pGPIO_x==GPIOF){
			GPIOF_PCLK_EN();
		}else if(pGPIO_x==GPIOG){
			GPIOG_PCLK_EN();
		}else if(pGPIO_x==GPIOH){
			GPIOH_PCLK_EN();
		}else if(pGPIO_x==GPIOI){
			GPIOI_PCLK_EN();
		}
	}else if(en_or_dis==DISABLE){
		if(pGPIO_x==GPIOA){
			GPIOA_PCLK_DIS();
		}
		else if(pGPIO_x==GPIOB){
			GPIOB_PCLK_DIS();
		}
		else if(pGPIO_x==GPIOC){
			GPIOC_PCLK_DIS();
		}
		else if(pGPIO_x==GPIOD){
			GPIOD_PCLK_DIS();
		}
		else if(pGPIO_x==GPIOE){
			GPIOE_PCLK_DIS();
		}
		else if(pGPIO_x==GPIOF){
			GPIOF_PCLK_DIS();
		}
		else if(pGPIO_x==GPIOG){
			GPIOG_PCLK_DIS();
		}
		else if(pGPIO_x==GPIOH){
			GPIOH_PCLK_DIS();
		}
		else if(pGPIO_x==GPIOI){
			GPIOI_PCLK_DIS();
		}
		//throw error
	}
}

/*
 * Init and de-init
 */
void GPIO_init(GPIO_Handle_t *pGPIO_handle){
	//configure pin mode

	//create temp register
	uint32_t temp=0;

	GPIO_Peri_clk_ctrl(pGPIO_handle->pGPIO_x, ENABLE);


	if(pGPIO_handle->GPIO_pin_config.GPIO_pin_mode <= GPIO_MODE_ANALOG){
		temp = (pGPIO_handle->GPIO_pin_config.GPIO_pin_mode << ( 2 * pGPIO_handle->GPIO_pin_config.GPIO_pin_number));
		pGPIO_handle->pGPIO_x->MODER &= ~(0x3<< ( 2 * pGPIO_handle->GPIO_pin_config.GPIO_pin_number));
		pGPIO_handle->pGPIO_x->MODER |= temp;

	}else{
		// write later for init interupts
		if(pGPIO_handle->GPIO_pin_config.GPIO_pin_mode <= GPIO_MODE_IT_FT){

			// configure FT
			EXTI->FTSR|= (1 << pGPIO_handle->GPIO_pin_config.GPIO_pin_number);
			EXTI->RTSR&= ~(1 << pGPIO_handle->GPIO_pin_config.GPIO_pin_number);


		}else if(pGPIO_handle->GPIO_pin_config.GPIO_pin_mode <= GPIO_MODE_IT_RT){
			// config RT
			EXTI->RTSR|= (1 << pGPIO_handle->GPIO_pin_config.GPIO_pin_number);
			EXTI->FTSR&= ~(1 << pGPIO_handle->GPIO_pin_config.GPIO_pin_number);
		}else if(pGPIO_handle->GPIO_pin_config.GPIO_pin_mode <= GPIO_MODE_RTFT){
			//config both ft and rt
			EXTI->FTSR|= (1 << pGPIO_handle->GPIO_pin_config.GPIO_pin_number);
			EXTI->RTSR|= (1 << pGPIO_handle->GPIO_pin_config.GPIO_pin_number);
		}

		SYSCFG_PCLK_EN();

		//configure gpio port using syscfg_exti
		uint8_t temp1 = pGPIO_handle->GPIO_pin_config.GPIO_pin_number/4;
		uint8_t temp2 = pGPIO_handle->GPIO_pin_config.GPIO_pin_number%4;
		uint8_t portcode = GPIO_BASE_ADDRESS_TO_CODE(pGPIO_handle->pGPIO_x);
		SYSCFG->EXTICR[temp1] &= ~(0xF << (4 * temp2));
		SYSCFG->EXTICR[temp1]|=( portcode << 4 * temp2);


		//enable the interupt delivery
		EXTI->IMR|=(1 << pGPIO_handle->GPIO_pin_config.GPIO_pin_number);
	}

	//config speed
	temp = 0;
	temp = (pGPIO_handle->GPIO_pin_config.GPIO_pin_speed << ( 2 * pGPIO_handle->GPIO_pin_config.GPIO_pin_number));
	pGPIO_handle->pGPIO_x->OSPEEDR &= ~(0x3<< ( 2 * pGPIO_handle->GPIO_pin_config.GPIO_pin_number));
	pGPIO_handle->pGPIO_x->OSPEEDR |= temp;


	//config pupd setting

	temp = 0;
	temp = (pGPIO_handle->GPIO_pin_config.GPIO_pin_pdown_pup_ctrl << ( 2 * pGPIO_handle->GPIO_pin_config.GPIO_pin_number));
	pGPIO_handle->pGPIO_x->PUPDR &= ~(0x3<< ( 2 * pGPIO_handle->GPIO_pin_config.GPIO_pin_number));
	pGPIO_handle->pGPIO_x->PUPDR |= temp;

	//operation type
	temp = 0;
	temp = (pGPIO_handle->GPIO_pin_config.GPIO_pin_op_type << pGPIO_handle->GPIO_pin_config.GPIO_pin_number);
	pGPIO_handle->pGPIO_x->OTYPER &= ~(0x1<< pGPIO_handle->GPIO_pin_config.GPIO_pin_number);
	pGPIO_handle->pGPIO_x->OTYPER |= temp;

	temp = 0;
	//config alt func
	if(pGPIO_handle->GPIO_pin_config.GPIO_pin_mode  == GPIO_MODE_ALTFUN){
			temp = (pGPIO_handle->GPIO_pin_config.GPIO_pin_alt_func_mode << ( 4 * (pGPIO_handle->GPIO_pin_config.GPIO_pin_number % 8)));
			pGPIO_handle->pGPIO_x->AFR[pGPIO_handle->GPIO_pin_config.GPIO_pin_number  <= GPIO_PIN_7 ? 0 : 1]&= ~(0xF << ( 4 * (pGPIO_handle->GPIO_pin_config.GPIO_pin_number % 8)));
			pGPIO_handle->pGPIO_x->AFR[pGPIO_handle->GPIO_pin_config.GPIO_pin_number  <= GPIO_PIN_7 ? 0 : 1]|=temp;
	}
}

void GPIO_deinit(GPIO_REG_DEF_t *pGPIO_x){
	if(pGPIO_x==GPIOA)
		GPIOA_REG_RESET();
	else if(pGPIO_x==GPIOB)
		GPIOB_REG_RESET();
	else if(pGPIO_x==GPIOC)
		GPIOC_REG_RESET();
	else if(pGPIO_x==GPIOD)
		GPIOD_REG_RESET();
	else if(pGPIO_x==GPIOE)
		GPIOE_REG_RESET();
	else if(pGPIO_x==GPIOF)
		GPIOF_REG_RESET();
	else if(pGPIO_x==GPIOG)
		GPIOG_REG_RESET();
	else if(pGPIO_x==GPIOH)
		GPIOH_REG_RESET();
	else if(pGPIO_x==GPIOI)
		GPIOI_REG_RESET();
}

/*
 * read input
 */
uint8_t GPIO_read_input_pin(GPIO_REG_DEF_t *pGPIO_x, uint8_t pin_number){
	uint8_t input = 0x0;
	input = (uint8_t)((pGPIO_x->IDR >> pin_number) & 0x00000001);
	return input;
}
uint16_t GPIO_read_input_port(GPIO_REG_DEF_t *pGPIOx){
	uint16_t input = 0x0;
	input = (uint16_t)pGPIOx->IDR;
	return input;
	//return pGPIO_x->IDR  & 0xFF;
}

/*
 * write output
 */

void GPIO_write_to_output_pin(GPIO_REG_DEF_t *pGPIO_x, uint8_t pin_number, uint8_t value){
	if(value==GPIO_PIN_SET){
		pGPIO_x->ODR|=(1 << pin_number);
	}else{
		pGPIO_x->ODR&=~(1 << pin_number);

	}
}
void GPIO_write_to_output_port(GPIO_REG_DEF_t *pGPIO_x, uint16_t value){
	pGPIO_x->ODR=value;

}
void GPIO_toggle_output_pin(GPIO_REG_DEF_t *pGPIO_x, uint8_t pin_number){
	pGPIO_x->ODR^=(1 << pin_number);

}

/*
 * IRQ ISR Handling
 *
 */
void GPIO_IRQConfig(uint8_t IRQ_number, uint8_t IRQ_en_or_dis){
	if(IRQ_en_or_dis==ENABLE){
		if(IRQ_number <= 31){
			//program ISER0
			*NVIC_ISER0|=(1 << IRQ_number);
		}else if(IRQ_number > 31 && IRQ_number < 64){
			//program ISER1
			*NVIC_ISER1|=(1 << IRQ_number%32);

		}else if(IRQ_number >= 64 && IRQ_number < 96){
			//program ISER2
			*NVIC_ISER2|=(1 << IRQ_number%32);
		}
	}else{
		if(IRQ_number <= 31){
			//program ICER0
			*NVIC_ICER0|=(1 << IRQ_number);
		}else if(IRQ_number > 31 && IRQ_number < 64){
			//program ISER1
			*NVIC_ICER1|=(1 << IRQ_number%32);

		}else if(IRQ_number >= 64 && IRQ_number < 96){
					//program ISER2
			*NVIC_ICER2|=(1 << IRQ_number%32);
		}
	}
}

void GPIO_Priority_Config(uint8_t IRQ_number, uint32_t IRQ_priority){
	uint8_t iprx = IRQ_number/4;
	uint8_t iprx_section = IRQ_number%4;
	uint8_t priority_shift = (8 * iprx_section ) + (8 - NO_PRIORITY_BITS);
	*(NVIC_PRIORITY_BASE + (iprx))|= (IRQ_priority << priority_shift);

}
void GPIO_IRQHandling(uint8_t pin_number){
	if(EXTI->PR & (1 << pin_number)){
		//clear the pr
		EXTI->PR|=(1 << pin_number);
	}
}
