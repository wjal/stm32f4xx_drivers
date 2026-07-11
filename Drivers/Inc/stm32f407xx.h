/*
 * stm32f407xx.h
 *
 *  Created on: Apr 20, 2026
 *      Author: jameslittle
 */

#ifndef INC_STM32F407XX_H_
#define INC_STM32F407XX_H_

#include <stdint.h>
#include <stddef.h>
/////////////////////////////

/*
 * ARM M4 PROCESSOR SPECIFIC DETAILS
 *
 *
 */


#define NVIC_ISER0				(volatile uint32_t*)0xE000E100UL
#define NVIC_ISER1				(volatile uint32_t*)0xE000E104UL
#define NVIC_ISER2				(volatile uint32_t*)0xE000E108UL
#define NVIC_ISER3				(volatile uint32_t*)0xE000E10CUL
#define NVIC_ISER4				(volatile uint32_t*)0xE000E110UL
#define NVIC_ISER5				(volatile uint32_t*)0xE000E114UL
#define NVIC_ISER6				(volatile uint32_t*)0xE000E118UL
#define NVIC_ISER7				(volatile uint32_t*)0xE000E11CUL



#define NVIC_ICER0				(volatile uint32_t*)0xE000E180UL
#define NVIC_ICER1				(volatile uint32_t*)0xE000E184UL
#define NVIC_ICER2				(volatile uint32_t*)0xE000E188UL
#define NVIC_ICER3				(volatile uint32_t*)0xE000E18CUL
#define NVIC_ICER4				(volatile uint32_t*)0xE000E190UL
#define NVIC_ICER5				(volatile uint32_t*)0xE000E194UL
#define NVIC_ICER6				(volatile uint32_t*)0xE000E198UL
#define NVIC_ICER7				(volatile uint32_t*)0xE000E19CUL

#define NVIC_PRIORITY_BASE		(volatile uint32_t*)0xE000E400UL
#define NO_PRIORITY_BITS		4

#define NVIC_PRIORITY_0			0
#define NVIC_PRIORITY_1			1
#define NVIC_PRIORITY_2			2
#define NVIC_PRIORITY_3			3
#define NVIC_PRIORITY_4			4
#define NVIC_PRIORITY_5			5
#define NVIC_PRIORITY_6			6
#define NVIC_PRIORITY_7			7
#define NVIC_PRIORITY_8			8
#define NVIC_PRIORITY_9			9
#define NVIC_PRIORITY_10		10
#define NVIC_PRIORITY_11		11
#define NVIC_PRIORITY_12		12
#define NVIC_PRIORITY_13		13
#define NVIC_PRIORITY_14		14
#define NVIC_PRIORITY_15		15


#define ENABLE 				1
#define DISABLE 			0
#define SET					ENABLE
#define RESET				DISABLE
#define FLAG_SET			SET
#define FLAG_RESET			RESET
#define GPIO_PIN_SET		SET
#define GPIO_PIN_RESET		RESET

#define SRAM_BASE_ADDRESS	0x20000000UL
#define SRAM1_START			SRAM_BASE_ADDRESS
#define SRAM1_SIZE			0x1C000UL
#define SRAM1_END			((SRAM1_START) +(SRAM1_SIZE))

#define SRAM2_START			SRAM1_END
#define SRAM2_SIZE			0x4000UL
#define SRAM2_END			((SRAM2_START) +(SRAM2_SIZE))

#define FLASH_BASE_ADDRESS 	0x08000000UL
#define FLASH_SIZE			0x100000UL
#define FLASH_END			((FLASH_BASE_ADDRESS)+ (FLASH_SIZE))

#define ROM_START   		0x1FFF0000UL
#define ROM_SIZE			0x7800UL
#define ROM_END     		((ROM_START) + (ROM_SIZE))

#define CCM_START   		0x10000000UL
#define CCM_SIZE    		0x10000UL
#define CCM_END     		((CCM_START) + (CCM_SIZE))

#define PERIPH_BASE			0x40000000UL
#define APB1_BASE_ADDRESS 	PERIPH_BASE
#define APB2_BASE_ADDRESS 	0x40010000UL
#define AHB1_BASE			0x40020000UL
#define AHB2_BASE			0x50000000UL
#define AHB3_BASE			0xA0000000UL



/**
 *
 * BASE ADDRESSES of peripherals on AHB1
 *
 */
#define USB_OTG_HS_BASE				0x40040000UL
#define DMA2D_BASE					0x4002B000UL
#define ETHERNET_MAC_BASE			0x40028000UL
#define FLASH_INTERFACE_REG_BASE	0x40023C00UL
#define RCC_BASE					0x40023800UL
#define CRC_BASE					0x40023000UL
#define DMA2_BASE					0x40026400UL
#define DMA1_BASE					0x40026000UL
#define BKPSRAM_BASE				0x40024000UL
#define GPIOK_BASE					0x40022800UL
#define GPIOJ_BASE					0x40022400UL
#define GPIOI_BASE					0x40022000UL
#define GPIOH_BASE					0x40021C00UL
#define GPIOG_BASE					0x40021800UL
#define GPIOF_BASE					0x40021400UL
#define GPIOE_BASE					0x40021000UL
#define GPIOD_BASE					0x40020C00UL
#define GPIOC_BASE					0x40020800UL
#define GPIOB_BASE					0x40020400UL
#define GPIOA_BASE					0x40020000UL


/**
 *
 * BASE ADDRESSES of peripherals on AHB2
 *
 */
#define USB_OTG_FS_BASE			0x50000000UL
#define DCMI_BASE				0x50050000UL
#define CRYP_BASE				0x50060000UL
#define HASH_BASE				0x50060400UL
#define RNG_BASE 				0x50060800UL

/**
 *
 * BASE ADDRESSES of peripherals on APB2
 *
 */

#define LCD_TFT_BASE		0x40016800UL
#define SAI1_BASE			0x40015800UL
#define SPI6_BASE			0x40015400UL
#define SPI5_BASE			0x40015000UL
#define TIM11_BASE			0x40014400UL
#define TIM10_BASE			0x40014000UL
#define TIM9_BASE			0x40014800UL
#define EXTI_BASE			0x40013C00UL
#define SYSCFG_BASE			0x40013800UL

#define SPI4_BASE				0x40013400UL
#define SPI1_BASE				0x40013000UL
#define SDIO_BASE				0x40012C00UL
#define AD1_ADC2_ADC_3_BASE		0x40012000UL
#define USART6_BASE				0x40011400UL
#define USART1_BASE				0x40011000UL
#define TIM8_BASE				0x40010400UL
#define TIM1_BASE				0x40010000UL

/**
 *
 * BASE ADDRESSES of peripherals on APB1
 *
 */

#define UART8				0x40007C00UL
#define UART7				0x40007800UL
#define DAC					0x40007400UL
#define PWR					0x40007000UL
#define CAN2				0x40006800UL
#define CAN1				0x40006400UL
#define I2C3				0x40005C00UL
#define I2C2				0x40005800UL
#define IC21				0x40005400UL
#define UART5				0x40005000UL
#define UART4				0x40004C00UL
#define USART3				0x40004800UL
#define USART2				0x40004400UL
#define I2S3_EXT			0x40004000UL
#define SPI3_I2S3			0x40003C00UL
#define SPI2_I2S2			0x40003800UL
#define IS2_EXT				0x40003400UL
#define IWDG				0x40003000UL
#define WWDG				0x40002C00UL
#define RTC_BKP_REG			0x40002800UL
#define TIM14				0x40002000UL
#define TIM13				0x40001C00UL
#define TIM12				0x40001800UL
#define TIM7				0x40001400UL
#define TIM6				0x40001000UL
#define TIM5				0x40000C00UL
#define TIM4				0x40000800UL
#define TIM3				0x40000400UL
#define TIM2				0x40000000UL



/*
 * GPIO REGISTERS OFFSET
 *
 */

#define GPIOx_MODER_OFFSET		0x00UL
#define GPIOx_OTYPER_OFFSET		0x04UL
#define GPIOx_OSPEEDR_OFFSET	0x08UL
#define GPIOx_PUPDR_OFFSET		0x0CUL
#define GPIOx_IDR_OFFSET		0x10UL
#define GPIOx_ODR_OFFSET		0x14UL
#define GPIOx_BSRR_OFFSET		0x18UL
#define GPIOx_LCKR_OFFSET		0x1CUL
#define GPIOx_AFRL_OFFSET		0x20UL
#define GPIOx_AFRH_OFFSET		0x24UL

typedef struct{
	volatile uint32_t MODER; 			//GPIO port mode register
	volatile uint32_t OTYPER; 			//GPIO port output type register
	volatile uint32_t OSPEEDR; 			//GPIO port output speed register
	volatile uint32_t PUPDR; 			//GPIO port pull-up/pull-down register
	volatile uint32_t IDR;				//GPIO port input data register
	volatile uint32_t ODR;				//GPIO port output data register
	volatile uint32_t BSRR;				//GPIO port bit set/reset register
	volatile uint32_t LCKR;				//GPIO port configuration lock register
	volatile uint32_t AFR[2];  			// GPIO alternate function low/high registers  - low is [0], high is [1]
} GPIO_REG_DEF_t;

#define GPIOK				((GPIO_REG_DEF_t*)GPIOK_BASE)
#define GPIOJ				((GPIO_REG_DEF_t*)GPIOJ_BASE)
#define GPIOI				((GPIO_REG_DEF_t*)GPIOI_BASE)
#define GPIOH				((GPIO_REG_DEF_t*)GPIOH_BASE)
#define GPIOG				((GPIO_REG_DEF_t*)GPIOG_BASE)
#define GPIOF				((GPIO_REG_DEF_t*)GPIOF_BASE)
#define GPIOE				((GPIO_REG_DEF_t*)GPIOE_BASE)
#define GPIOD				((GPIO_REG_DEF_t*)GPIOD_BASE)
#define GPIOC				((GPIO_REG_DEF_t*)GPIOC_BASE)
#define GPIOB				((GPIO_REG_DEF_t*)GPIOB_BASE)
#define GPIOA				((GPIO_REG_DEF_t*)GPIOA_BASE)


typedef struct{
	volatile uint32_t CR;
	volatile uint32_t PLLCFGR;
	volatile uint32_t CFGR;
	volatile uint32_t CIR;
	volatile uint32_t AHB1RSTR;
	volatile uint32_t AHB2RSTR;
	volatile uint32_t AHB3RSTR;
	uint32_t _reserved_0;
	volatile uint32_t APB1RSTR;
	volatile uint32_t APB2RSTR;
	uint32_t _reserved_1[2];
	volatile uint32_t AHB1ENR;
	volatile uint32_t AHB2ENR;
	volatile uint32_t AHB3ENR;
	uint32_t _reserved_3;
	volatile uint32_t APB1ENR;
	volatile uint32_t APB2ENR;
	uint32_t _reserved_4[2];
	volatile uint32_t AHB1LPENR;
	volatile uint32_t AHB2LPENR;
	volatile uint32_t AHB3LPENR;
	uint32_t _reserved_6;
	volatile uint32_t APB1LPENR;
	volatile uint32_t APB2LPENR;
	uint32_t _reserved_7[2];
	volatile uint32_t BDCR;
	volatile uint32_t CSR;
	uint32_t _reserved_9[2];
	volatile uint32_t SSCGR;
	volatile uint32_t PLLI2SCFGR;
} RCC_DEFREG_t;


//GPIO CLOCK ENABLE PERIPHERALS

#define RCC 				((RCC_DEFREG_t*)RCC_BASE)
#define GPIOA_PCLK_EN() 	(RCC->AHB1ENR|=(1 << 0))
#define GPIOB_PCLK_EN() 	(RCC->AHB1ENR|=(1 << 1))
#define GPIOC_PCLK_EN() 	(RCC->AHB1ENR|=(1 << 2))
#define GPIOD_PCLK_EN() 	(RCC->AHB1ENR|=(1 << 3))
#define GPIOE_PCLK_EN() 	(RCC->AHB1ENR|=(1 << 4))
#define GPIOF_PCLK_EN() 	(RCC->AHB1ENR|=(1 << 5))
#define GPIOG_PCLK_EN() 	(RCC->AHB1ENR|=(1 << 6))
#define GPIOH_PCLK_EN() 	(RCC->AHB1ENR|=(1 << 7))
#define GPIOI_PCLK_EN() 	(RCC->AHB1ENR|=(1 << 8))

//GPIO CLOCK DISABLE PERIPHERALS
#define GPIOA_PCLK_DIS() 	(RCC->AHB1ENR&=~(1 << 0))
#define GPIOB_PCLK_DIS() 	(RCC->AHB1ENR&=~(1 << 1))
#define GPIOC_PCLK_DIS() 	(RCC->AHB1ENR&=~(1 << 2))
#define GPIOD_PCLK_DIS() 	(RCC->AHB1ENR&=~(1 << 3))
#define GPIOE_PCLK_DIS() 	(RCC->AHB1ENR&=~(1 << 4))
#define GPIOF_PCLK_DIS() 	(RCC->AHB1ENR&=~(1 << 5))
#define GPIOG_PCLK_DIS() 	(RCC->AHB1ENR&=~(1 << 6))
#define GPIOH_PCLK_DIS() 	(RCC->AHB1ENR&=~(1 << 7))
#define GPIOI_PCLK_DIS() 	(RCC->AHB1ENR&=~(1 << 8))

#define GPIO_BASE_ADDRESS_TO_CODE(x)	(x==GPIOA) ? 0 :\
										(x==GPIOB) ? 1 :\
										(x==GPIOC) ? 2 :\
										(x==GPIOD) ? 3 :\
										(x==GPIOE) ? 4 :\
										(x==GPIOF) ? 5 :\
										(x==GPIOG) ? 6 :\
										(x==GPIOH) ? 7 :\
										(x==GPIOI) ? 8 :0


#define GPIO_ALT_FUNCTION_0     0
#define GPIO_ALT_FUNCTION_1     1
#define GPIO_ALT_FUNCTION_2     2
#define GPIO_ALT_FUNCTION_3     3
#define GPIO_ALT_FUNCTION_4     4
#define GPIO_ALT_FUNCTION_5     5
#define GPIO_ALT_FUNCTION_6     6
#define GPIO_ALT_FUNCTION_7     7
#define GPIO_ALT_FUNCTION_8     8
#define GPIO_ALT_FUNCTION_9     9
#define GPIO_ALT_FUNCTION_10    10
#define GPIO_ALT_FUNCTION_11    11
#define GPIO_ALT_FUNCTION_12    12
#define GPIO_ALT_FUNCTION_13    13
#define GPIO_ALT_FUNCTION_14    14
#define GPIO_ALT_FUNCTION_15    15



typedef struct{
	volatile uint32_t CR1;
	volatile uint32_t CR2;
	volatile uint32_t SR;
	volatile uint32_t DR;
	volatile uint32_t CRCPR;
	volatile uint32_t RXCRCR;
	volatile uint32_t TXCRCR;
	volatile uint32_t I2SCFGR;
	volatile uint32_t I2SPR;
}SPI_REG_DEF_t;

#define SPI_2_I2S2_OFFSET	0x3800UL
#define SPI_3_I2S3_OFFSET	0x3C00UL
#define SPI_2_BASE_ADDRESS	((APB1_BASE_ADDRESS) + SPI_2_I2S2_OFFSET)
#define SPI_3_BASE_ADDRESS	((APB1_BASE_ADDRESS) + SPI_3_I2S3_OFFSET)


// APB2
#define SPI_1_APB2_OFFSET	0x3000UL
#define SPI_4_APB2_OFFSET	0x3400UL
#define SPI_5_APB2_OFFSET	0x5000UL
#define SPI_6_APB2_OFFSET	0x5400UL
#define SPI_1_BASE_ADDRESS	((APB2_BASE_ADDRESS) + SPI_1_APB2_OFFSET)
#define SPI_4_BASE_ADDRESS	((APB2_BASE_ADDRESS) + SPI_4_APB2_OFFSET)
#define SPI_5_BASE_ADDRESS	((APB2_BASE_ADDRESS) + SPI_5_APB2_OFFSET)
#define SPI_6_BASE_ADDRESS	((APB2_BASE_ADDRESS) + SPI_6_APB2_OFFSET)


#define SPI_1	((SPI_REG_DEF_t*)SPI_1_BASE_ADDRESS)
#define SPI_2	((SPI_REG_DEF_t*)SPI_2_BASE_ADDRESS)
#define SPI_3	((SPI_REG_DEF_t*)SPI_3_BASE_ADDRESS)
#define SPI_4	((SPI_REG_DEF_t*)SPI_4_BASE_ADDRESS)
//EXTI_BASE
typedef struct{
	volatile uint32_t IMR;
	volatile uint32_t EMR;
	volatile uint32_t RTSR;
	volatile uint32_t FTSR;
	volatile uint32_t SWIER;
	volatile uint32_t PR;
}EXTI_REG_DEF_t;

#define EXTI 				((EXTI_REG_DEF_t*)EXTI_BASE)


typedef struct{
	volatile uint32_t MEMRMP;  //00
	volatile uint32_t PMC;		//0x04
	volatile uint32_t EXTICR[4];	//0x14
	uint32_t reserved[2];
	volatile uint32_t CMPCR;
	uint32_t reserved2[2];
	uint32_t CFGR;
}SYSCFG_REG_DEF_t;

#define SYSCFG			((SYSCFG_REG_DEF_t*)SYSCFG_BASE)

//GPIO RCC ahb1 reset
#define AHB1_RESET_OFFSET	0x10UL
#define RCC_AHB1RSTR		((AHB1_BASE) + (AHB1_RESET_OFFSET))

#define GPIOA_REG_RESET()	do {(RCC->AHB1RSTR|= (1 << 0)); (RCC->AHB1RSTR&= ~(1 << 0));}while(0)
#define GPIOB_REG_RESET()	do {(RCC->AHB1RSTR|= (1 << 1)); (RCC->AHB1RSTR&= ~(1 << 1));}while(0)
#define GPIOC_REG_RESET()	do {(RCC->AHB1RSTR|= (1 << 2)); (RCC->AHB1RSTR&= ~(1 << 2));}while(0)
#define GPIOD_REG_RESET()	do {(RCC->AHB1RSTR|= (1 << 3)); (RCC->AHB1RSTR&= ~(1 << 3));}while(0)
#define GPIOE_REG_RESET()	do {(RCC->AHB1RSTR|= (1 << 4)); (RCC->AHB1RSTR&= ~(1 << 4));}while(0)
#define GPIOF_REG_RESET()	do {(RCC->AHB1RSTR|= (1 << 5)); (RCC->AHB1RSTR&= ~(1 << 5));}while(0)
#define GPIOG_REG_RESET()	do {(RCC->AHB1RSTR|= (1 << 6)); (RCC->AHB1RSTR&= ~(1 << 6));}while(0)
#define GPIOH_REG_RESET()	do {(RCC->AHB1RSTR|= (1 << 7)); (RCC->AHB1RSTR&= ~(1 << 7));}while(0)
#define GPIOI_REG_RESET()	do {(RCC->AHB1RSTR|= (1 << 8)); (RCC->AHB1RSTR&= ~(1 << 8));}while(0)



#define SPI1_PCLK_EN()		(RCC->APB2ENR|=(1 << 12))
#define SPI2_PCLK_EN()		(RCC->APB1ENR|=(1 << 14))
#define SPI3_PCLK_EN()		(RCC->APB1ENR|=(1 << 15))
#define SPI4_PCLK_EN()		(RCC->APB2ENR|=(1 << 13))

#define SPI1_PCLK_DIS()		(RCC->APB2ENR&=~(1 << 12))
#define SPI2_PCLK_DIS()		(RCC->APB1ENR&=~(1 << 14))
#define SPI3_PCLK_DIS()		(RCC->APB1ENR&=~(1 << 15))
#define SPI4_PCLK_DIS()		(RCC->APB2ENR|=~(1 << 13))

#define SPI_1_RESET()		do {(RCC->APB2RSTR|= (1 << 12)); (RCC->APB2RSTR&= ~(1 << 12));}while(0)
#define SPI_2_RESET()		do {(RCC->APB1RSTR|= (1 << 14)); (RCC->APB1RSTR&= ~(1 << 14));}while(0)
#define SPI_3_RESET()		do {(RCC->APB1RSTR|= (1 << 15)); (RCC->APB1RSTR&= ~(1 << 15));}while(0)
#define SPI_4_RESET()		do {(RCC->APB2RSTR|= (1 << 13)); (RCC->APB2RSTR&= ~(1 << 13));}while(0)

//// spi clock dis

#define USART1_PCLK_EN()	(RCC->APB2ENR|=(1 << 4))

#define USART2_PCLK_EN()	(RCC->APB1ENR|=(1 << 17))
#define USART3_PCLK_EN()	(RCC->APB1ENR|=(1 << 18))
#define UART4_PCLK_EN()		(RCC->APB1ENR|=(1 << 19))
#define UART5_PCLK_EN()		(RCC->APB1ENR|=(1 << 20))

#define USART6_PCLK_EN()	(RCC->APB2ENR|=(1 << 5))


#define SYSCFG_PCLK_EN()	(RCC->APB2ENR|=(1 << 14))

/*
 * IRQ_NO_EXTI'S
 */
#define IRQ_NO_EXTI_0		6
#define IRQ_NO_EXTI_1		7
#define IRQ_NO_EXTI_2		8
#define IRQ_NO_EXTI_3		9
#define IRQ_NO_EXTI_4		10
#define IRQ_NO_EXTI_5_9		23
#define IRQ_NO_EXTI_10_15	40



/*
 * IRQ_NO_S'Spi
 */
#define IRQ_NO_SPI1			35
#define IRQ_NO_SPI2			36
#define IRQ_NO_SPI3			51


/*
 * IRQ_NO_S'i2c
 */

#define IRQ_NO_I2C1_EV			31
#define IRQ_NO_I2C1_ER			32
#define IRQ_NO_I2C2_EV			33
#define IRQ_NO_I2C2_ER			34
#define IRQ_NO_I2C3_EV			72
#define IRQ_NO_I2C3_ER			73

/*
 * BIT POSITION MACROS
 */
#define SPI_CR1_CPHA		0
#define SPI_CR1_CPOL		1
#define SPI_CR1_MSTR		2
#define SPI_CR1_BR			3
#define SPI_CR1_SPE			6
#define SPI_CR1_LSBFIRST	7
#define SPI_CR1_SSI			8
#define SPI_CR1_SSM			9
#define SPI_CR1_RXONLY		10
#define SPI_CR1_DFF			11
#define SPI_CR1_CRCNEXT		12
#define SPI_CR1_CRCEN		13
#define SPI_CR1_BIDIOE		14
#define SPI_CR1_BIDIMODE	15



#define SPI_CR2_RXDMAEN		0
#define SPI_CR2_TXDMAEN		1
#define SPI_CR2_SSOE		2
#define SPI_CR2_FRF			4
#define SPI_CR2_ERRIE		5
#define SPI_CR2_RXNEIE		6
#define SPI_CR2_TXEIE		7


#define SPI_SR_RXNE			0
#define SPI_SR_TXE			1
#define SPI_SR_CHSIDEE		2
#define SPI_SR_UDR			3
#define SPI_SR_CRCERR		4
#define SPI_SR_MODF			5
#define SPI_SR_OVR			6
#define SPI_SR_BSY			7
#define SPI_SR_FRE			8




/**
 * I2C REG DEF
 */

typedef struct {
	volatile uint32_t I2C_CR1;
	volatile uint32_t I2C_CR2;
	volatile uint32_t I2C_OAR1;
	volatile uint32_t I2C_OAR2;
	volatile uint32_t I2C_DR;
	volatile uint32_t I2C_SR1;
	volatile uint32_t I2C_SR2;
	volatile uint32_t I2C_CCR;
	volatile uint32_t I2C_TRISE;
	volatile uint32_t I2C_FLTR;
} I2C_REG_DEF_t;


#define I2C1_PCLK_EN()		(RCC->APB1ENR|=(1 << 21))
#define I2C2_PCLK_EN()		(RCC->APB1ENR|=(1 << 22))
#define I2C3_PCLK_EN()		(RCC->APB1ENR|=(1 << 23))

#define I2C1_PCLK_DIS()		(RCC->APB1ENR&=~(1 << 21))
#define I2C2_PCLK_DIS()		(RCC->APB1ENR&=~(1 << 22))
#define I2C3_PCLK_DIS()		(RCC->APB1ENR&=~(1 << 23))


#define I2C_1_OFFSET 		0x5400UL
#define I2C_2_OFFSET 		0x5800UL
#define I2C_3_OFFSET 		0x5C00UL


#define I2C_1_BASE_ADDRESS 	((APB1_BASE_ADDRESS) + I2C_1_OFFSET)
#define I2C_2_BASE_ADDRESS	((APB1_BASE_ADDRESS) + I2C_2_OFFSET)
#define I2C_3_BASE_ADDRESS	((APB1_BASE_ADDRESS) + I2C_3_OFFSET)

#define I2C_1				((I2C_REG_DEF_t*)I2C_1_BASE_ADDRESS)
#define	I2C_2				((I2C_REG_DEF_t*)I2C_2_BASE_ADDRESS)
#define I2C_3				((I2C_REG_DEF_t*)I2C_3_BASE_ADDRESS)

#define I2C_1_RESET()		do{(RCC->APB1RSTR|=(1 << 21)); (RCC->APB1RSTR&=~(1 << 21));}while(0)
#define I2C_2_RESET()		do{(RCC->APB1RSTR|=(1 << 22)); (RCC->APB1RSTR&=~(1 << 22));}while(0)
#define I2C_3_RESET()		do{(RCC->APB1RSTR|=(1 << 23)); (RCC->APB1RSTR&=~(1 << 23));}while(0)

/*
 * I2C_REGISTER_BIT_POSITIONS
 */
#define	I2C_CR1_PE			0
#define	I2C_CR1_SMBUS		1
#define	I2C_CR1_SMB_TYPE	3
#define	I2C_CR1_ENARP		4
#define	I2C_CR1_ENPEC		5
#define	I2C_CR1_ENGC		6
#define	I2C_CR1_NOSTRETCH	7
#define	I2C_CR1_START		8
#define	I2C_CR1_STOP		9
#define	I2C_CR1_ACK			10
#define	I2C_CR1_POS			11
#define	I2C_CR1_PEC			12
#define	I2C_CR1_ALERT		13
#define	I2C_CR1_SWRST		15

#define I2C_CR2_FREQ		0
#define I2C_CR2_ITERREN		8
#define I2C_CR2_ITEVTEN		9
#define I2C_CR2_ITBUFEN		10
#define I2C_CR2_DMAEN		11
#define I2C_CR2_LAST		12

#define I2C_OAR1_ADD0		0
#define I2C_OAR1_ADD_7_1	1
#define I2C_OAR1_ADD_9_8	8
#define I2C_OAR1_14_SW_ON	14
#define I2C_OAR1_ADD_MODE	15

#define I2C_OAR2_ENDUAL		0
#define I2C_OAR2_ADD2		1



#define I2C_SR1_SB			0
#define I2C_SR1_ADDR		1
#define I2C_SR1_BTF			2
#define I2C_SR1_ADD10		3
#define I2C_SR1_STOPF		4
#define I2C_SR1_RXNE		6
#define I2C_SR1_TXE			7
#define I2C_SR1_BERR		8
#define I2C_SR1_ARLO		9
#define I2C_SR1_AF			10
#define I2C_SR1_OVR			11
#define I2C_SR1_PECERR		12
#define I2C_SR1_TIMEOUT		14
#define I2C_SR1_SMBALERT	15

#define I2C_SR2_MSL			0
#define I2C_SR2_BUSY		1
#define I2C_SR2_TRA			2
#define I2C_SR2_GENCALL		4
#define I2C_SR2_SMBDEFAUL	5
#define I2C_SR2_SMBHOST		6
#define I2C_SR2_DUALAF		7
#define I2C_SR2_PEC			8

#define I2C_CCR_BIT_0			0
#define	I2C_CCR_DUTY		14
#define I2C_CCR_FS			15

#define I2C_FLTR_DNF		0
#define I2C_FLTR_ANOFF		4


#endif /* INC_STM32F407XX_H_ */
