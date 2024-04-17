//-----------------------------------------------------------------------------------------------
/*
*Procedure:




Author            : Vysakh C S
Date              : 17-04-2024
Development board : STM32F407VGT6 Discovery Board
IDE               : Keil uVision V5.39.0.0

*/
//-----------------------------------------------------------------------------------------------
#include "stm32f407xx.h"
void gpio_config(void);
void uart2_config(void);
//void uart2_string_write(char *str);
void uart2_write(char ch);
char uart2_read(void);
//-----------------------------------------------------------------------------------------------
int main(void)
{
	/* GPIO and UART2 Conmfiguration setting function call */
	gpio_config();
	uart2_config();
	
	while(1)
	{ 
		char ch = uart2_read();		
		uart2_write(ch);
	}
}
//-----------------------------------------------------------------------------------------------
/*GPIO configurations */
void gpio_config(void)
{
	/*Clock access enabled for GPIOA */
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	
	/*PA2 set to alternate function mode */
	GPIOA->MODER |= GPIO_MODER_MODE2_1;
	GPIOA->MODER &= ~GPIO_MODER_MODE2_0;

	/* PA2 set to AF7 */
	/*GPIOA->AFR[0] is the low register*/
	GPIOA->AFR[0] |=  GPIO_AFRL_AFRL2_2 | GPIO_AFRL_AFRL2_1 | GPIO_AFRL_AFRL2_0;
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFRL2_3;
	
	/*PA3 set to alternate function mode */
	GPIOA->MODER |= GPIO_MODER_MODE3_1;
	GPIOA->MODER &= ~GPIO_MODER_MODE3_0;
	
	/* PA3 set to AF7 */
	/*GPIOA->AFR[0] is the low register*/
	GPIOA->AFR[0] |=  GPIO_AFRL_AFRL3_2 | GPIO_AFRL_AFRL3_1 | GPIO_AFRL_AFRL3_0;
	GPIOA->AFR[0] &= ~GPIO_AFRL_AFRL3_3;
}
//-----------------------------------------------------------------------------------------------
/*UART configurations */
void uart2_config(void)
{
	/* Bit 17 USART2EN: USART2 clock enable */
	/* 1: USART2 clock enabled */
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
	
	/* USART Baud Rate */
	/* 9600 baud @ 16 MHz */
	USART2->BRR =0x683; 
	
	/* Bit 3 TE: Transmitter enable */
	/* 1: Transmitter is enabled */
	USART2->CR1 |= USART_CR1_TE;
	
	/* Bit 13 UE: USART enable */
	/* 1: USART enabled */
	USART2->CR1 |= USART_CR1_UE;
	
	/* Bit 12 M: Word length */
	/* 0: 1 Start bit, 8 Data bits, 1 Stop bit */
	USART2->CR1 &= ~USART_CR1_M;
	
	/* Bits 13:12 STOP: STOP bit */
	/* 00: 1 Stop bit */
	USART2->CR2 &= ~USART_CR2_STOP_1;
	USART2->CR2 &= ~USART_CR2_STOP_0;
	
	/* Bit 2 RE: Receiver enable */
	/* 1: Receiver is enabled and begins searching for a start bit */
	USART2->CR1 |= USART_CR1_RE;
	}
//-----------------------------------------------------------------------------------------------
/* Write a character to UART4 */
void uart2_write(char ch) 
{
 while (!(USART2->SR & USART_SR_TXE)) {}
USART2->DR = ch;
}
//-----------------------------------------------------------------------------------------------	
	/* Read from UART2 */
char uart2_read(void) 
{
		 /* wait until RXNE buffer empty */
		 while (!(USART2->SR & USART_SR_RXNE)) {}
		 return USART2->DR;
}
//-----------------------------------------------------------------------------------------------