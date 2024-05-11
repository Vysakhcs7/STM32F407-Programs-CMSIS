//-----------------------------------------------------------------------------------------------
#include <stdio.h>
#include "stm32f407xx.h"
#include<stdint.h>

// Function prototypes
void gpio_config(void);
void uart2_config(void);
void uart2_single_write(int ch);
void uart2_string_write(char *str);
void delay(void);

// Retargeting printf() to USART2
int __io_putchar(int ch) {
    while (!(USART2->SR & USART_SR_TXE)); // Wait for TX buffer to be empty
    USART2->DR = (ch & 0xFF);             // Transmit character
    return ch;
}

// Redirect STDOUT to USART2
FILE __stdout;
int fputc(int ch, FILE *f) {
  return __io_putchar(ch);
}

//-----------------------------------------------------------------------------------------------
int main(void)
{
  int i =0;
  /* GPIO and UART2 Configuration setting function call */
  gpio_config();
  uart2_config();
  
  while(1)
  {  i++; 
    /* Passing single character */
    printf("i = %d\r\n",i);
    //uart2_single_write('A');
    //uart2_single_write('\r');
    //uart2_single_write('\n');
    //delay();
  }
}
//-----------------------------------------------------------------------------------------------
/*GPIO configurations */
/*PA2 is connected to the USART2_TX */
void gpio_config(void)
{
  /*Clock access enabled for GPIOA */
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
  
  /*PA2 set to alternate function mode */
  GPIOA->MODER |= GPIO_MODER_MODE2_1;
  GPIOA->MODER &= ~GPIO_MODER_MODE2_0;
  
  /* PA2 set to AF7 */
  /* GPIOA->AFR[0] is the low register */
  GPIOA->AFR[0] |=  GPIO_AFRL_AFRL2_2 | GPIO_AFRL_AFRL2_1 | GPIO_AFRL_AFRL2_0;
  GPIOA->AFR[0] &= ~GPIO_AFRL_AFRL2_3;
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
  USART2->BRR = 0x683; 
  
  /* Bit 3 TE: Transmitter enable */
  /* 1: Transmitter is enabled */
   USART2->CR1 |= USART_CR1_TE;
  
  /* Bit 13 UE: USART enable */
  /* 1: USART enabled */
  USART2->CR1 |= USART_CR1_UE;
  
  /* Bit 12 M: Word length */
  /* 0: 1 Start bit, 8 Data bits, n Stop bit */
  USART2->CR1 &= ~USART_CR1_M;
  
  /* Bits 13:12 STOP: STOP bit */
  /* 00: 1 Stop bit */
  USART2->CR2 &= ~USART_CR2_STOP_1;
  USART2->CR2 &= ~USART_CR2_STOP_0;
}
//-----------------------------------------------------------------------------------------------
/* Write a character to UART2 */
void uart2_single_write (int ch) 
{
     /* wait until Tx buffer empty */
     while (!(USART2->SR & USART_SR_TXE)) {}
     USART2->DR = (ch & 0XFF);
}

//-----------------------------------------------------------------------------------------------
/* Write a string to UART2 */
void uart2_string_write(char *str)
{
  while (*str) 
    {
        // Wait until transmit data register (TDR) is empty
        while (!(USART2->SR & USART_SR_TXE));
        
        // Transmit string
        USART2->DR = *str++;
    }
}
//-----------------------------------------------------------------------------------------------
void delay(void) {
    int i = 0;
    for (i = 0; i < 500000; i++);
}
//-----------------------------------------------------------------------------------------------
