//-----------------------------------------------------------------------------------------------
/*
* Program to transmit single character and string using UART2 in interrupt mode.

*Procedure:

1. Enable the Clock to GPIOA.
2. Enable the Clock to USART2.
3. Select the peripheral function AF7 for PA2 (USART2_TxD) pin using the
   GPIO_MODER and GPIO_AFRL registers.
4. Set the baud rate for USART2 using USART2_BRR register.
5. Configure the CR1 (Control 1) register for enabling transmit (TE) and TXE interrupt. 
6. Configure the CR2 (Control 2) register for number of stop bit(s) and so on. 
7. Configure the CR3 (Control 3) register for no hardware flow control and so on. 
8. Enable USART2 after configuration complete.
9. Implement USART2_IRQHandler to handle the TXE interrupt.
10. In the interrupt handler, write data from the buffer to the DR register.
11. To transmit the next character, add it to the buffer.

Author            : Vysakh C S
Date              : 20-02-2024
Development board : STM32F407VGT6 Discovery Board
IDE               : Keil uVision V5.39.0.0

*/
//-----------------------------------------------------------------------------------------------
#include "stm32f407xx.h"

#define BUFFER_SIZE 128

volatile char tx_buffer[BUFFER_SIZE];
volatile int tx_buffer_head = 0;
volatile int tx_buffer_tail = 0;

void gpio_config(void);
void uart2_config(void);
void uart2_single_write(char ch);
void uart2_string_write(char *str);
void delay(void);

//-----------------------------------------------------------------------------------------------
int main(void)
{
  /* GPIO and UART2 Configuration setting function call */
  gpio_config();
  uart2_config();
  
  while(1)
  {   
    /* Passing single character */
    uart2_single_write('A');
    uart2_single_write('\r');
    uart2_single_write('\n');
    delay();
    /* Passing a string */
    uart2_string_write("Hello\n\r");
    delay();
  }
}
//-----------------------------------------------------------------------------------------------
/* GPIO configurations */
/* PA2 is connected to the USART2_TX */
void gpio_config(void)
{
  /* Clock access enabled for GPIOA */
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
  
  /* PA2 set to alternate function mode */
  GPIOA->MODER |= GPIO_MODER_MODE2_1;
  GPIOA->MODER &= ~GPIO_MODER_MODE2_0;
  
  /* PA2 set to AF7 */
  /* GPIOA->AFR[0] is the low register */
  GPIOA->AFR[0] |=  GPIO_AFRL_AFRL2_2 | GPIO_AFRL_AFRL2_1 | GPIO_AFRL_AFRL2_0;
  GPIOA->AFR[0] &= ~GPIO_AFRL_AFRL2_3;
}
//-----------------------------------------------------------------------------------------------
/* UART configurations */
void uart2_config(void)
{

  /* Bit 17 USART2EN: USART2 clock enable */
  /* 1: USART2 clock enabled */
  RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
  
  /* USART Baud Rate */
  /* 9600 baud @ 16 MHz */
  USART2->BRR =0x0683; 
  
  /* Bit 3 TE: Transmitter enable */
  /* 1: Transmitter is enabled */
   USART2->CR1 |= USART_CR1_TE;
  
  /* Bit 7 TXEIE: TXE interrupt enable */
  /* 1: interrupt enable */
  USART2->CR1 |= USART_CR1_TXEIE;
  
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
  
  /* Enable USART2_IRQn */
  NVIC_EnableIRQ(USART2_IRQn);
}
//-----------------------------------------------------------------------------------------------
/* Write a character to UART2 */
void uart2_single_write (char ch) 
{
    // Put the character in the buffer
    tx_buffer[tx_buffer_head++] = ch;
    tx_buffer_head %= BUFFER_SIZE;
}
//-----------------------------------------------------------------------------------------------
/* Write a string to UART2 */
void uart2_string_write(char *str)
{
    while (*str) 
    {
        // Put characters in the buffer
        tx_buffer[tx_buffer_head++] = *str++;
        tx_buffer_head %= BUFFER_SIZE;
    }
}

//-----------------------------------------------------------------------------------------------
void USART2_IRQHandler(void)
{
    if (USART2->SR & USART_SR_TXE)  // Check TXE flag
    {
        if (tx_buffer_head != tx_buffer_tail)  // Check if buffer is not empty
        {
            USART2->DR = tx_buffer[tx_buffer_tail++];  // Send the next character
            tx_buffer_tail %= BUFFER_SIZE;
        }
        else
        {
            // Buffer is empty, disable TXE interrupt
            USART2->CR1 &= ~USART_CR1_TXEIE;
        }
    }
}
//-----------------------------------------------------------------------------------------------
void delay(void) {
  int i = 0;
  for (i = 0; i < 500000; i++);
}
//-----------------------------------------------------------------------------------------------
