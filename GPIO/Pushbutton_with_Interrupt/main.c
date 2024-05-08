/*
*Program to interface inbuild pushbutton with the inbuild led on the stm32f407vgt6 discovery board uisng interrupt.

GPIOD -> PIN 12 -> Green LED
GPIOA -> PIN 0  -> Pushbutton

1.	Enable clock access to GPIOD by setting bit 3 high in the RCC AHB1 peripheral clock enable register (RCC_AHB1ENR).
2.	Enable clock access to GPIOA by setting bit 0 high in the RCC AHB1 peripheral clock enable register (RCC_AHB1ENR).
3.	Enable clock access for SYSCFG.
4.	Set pin PA0 as input type by configuring the GPIO port mode register (GPIOx_MODER) with 00 for general-purpose input mode.
5.	Set pin PD12 as output type by configuring the GPIO port mode register (GPIOx_MODER) with 01 for general-purpose output mode.
6.	Disable global interrupt.
7.	Configure PA0 as the source input for EXTI0 external interrupt by modifying the SYSCFG_EXTICR1 register appropriately.
8.	Ensure that interrupt request from line 0 is not masked by setting the corresponding bit in the EXTI_IMR register.
9.	Enable falling trigger by setting the corresponding bit in the EXTI_FTSR register.
10.	Enable rising trigger by setting the corresponding bit in the EXTI_RTSR register.
11.	Enable EXTI0 line in the Nested Vector Interrupt Controller (NVIC) by calling NVIC_EnableIRQ() with the appropriate IRQ number for EXTI0.
12.	Enable global interrupt.
13. Implement the IRQHandler function.


Author            : Vysakh C S
Date              : 18-04-2024
Development board : STM32F407VGT6 Discovery Board
IDE               : Keil uVision V5.39.0.0
*/

//-----------------------------------------------------------------------------------------------
#include "stm32f407xx.h"
void clock_config(void);
void gpio_config(void);
void EXTI_config(void);
//-----------------------------------------------------------------------------------------------
int main(void)
{
	clock_config();
  gpio_config();
	EXTI_config();
   while(1)
	{

	}
}

//-----------------------------------------------------------------------------------------------
void clock_config(void)
{
  /*Enable clock for GPIOD and GPIOA*/
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	
	/*Enable clock access for SYSCFG*/
	RCC->APB2ENR |= RCC_APB2ENR_SYSCFGEN;
}
//-----------------------------------------------------------------------------------------------
void gpio_config(void)
{
  /*PD12 as Output*/
  GPIOD->MODER |= GPIO_MODER_MODE12_0;
  GPIOD->MODER &= ~GPIO_MODER_MODE12_1;
  
  /*PA0 as input*/
  GPIOA->MODER &= ~GPIO_MODER_MODE0_0;
  GPIOA->MODER &= ~GPIO_MODER_MODE0_1;
}
//-----------------------------------------------------------------------------------------------
void EXTI_config(void)
{
	/*Disable Global Interrupt*/
	__disable_irq();
	/* PA0 as the source input for the EXTI0  external interrupt */
	SYSCFG->EXTICR[0] &= ~SYSCFG_EXTICR1_EXTI0_PA;
	/* Interrupt request from line 0 is not masked */
	EXTI->IMR |= EXTI_IMR_MR0;
	/* Falling trigger enabled  */
	EXTI->FTSR |= EXTI_FTSR_TR0;
	/* Rising trigger enabled */
	EXTI->RTSR |= EXTI_RTSR_TR0;
	/* Enable EXTI0 line in NVIC*/
	NVIC_EnableIRQ(EXTI0_IRQn);
	/*Enable Global Interrupt*/
	__enable_irq();
}
//-----------------------------------------------------------------------------------------------
void EXTI0_IRQHandler(void)
{
	if((EXTI->PR) & (EXTI_PR_PR0))
	{
		/* Clear PR flag */
    EXTI->PR |= EXTI_PR_PR0;
		GPIOD->ODR ^= GPIO_ODR_OD12;
    
		
	}
}
//-----------------------------------------------------------------------------------------------