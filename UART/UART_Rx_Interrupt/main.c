//-----------------------------------------------------------------------------------------------
/*
* Program to receive single character and string using UART2 in interrupt mode.

*Procedure:

1. Enable the Clock to GPIOA.
2. Enable the Clock to USART2.
3. Select the peripheral function AF7 for PA2 (USART2_TxD) and PA3 (USART2_RxD) pins using the
   GPIO_MODER and GPIO_AFRL registers.
4. Set the baud rate for USART2 using USART2_BRR register.
5. Configure the CR1 (Control 1) register for enabling receive (RE) and RXNE interrupt. 
6. Configure the CR2 (Control 2) register for number of stop bit(s) and so on. 
7. Configure the CR3 (Control 3) register for no hardware flow control and so on. 
8. Enable USART2 after configuration complete.
9. Implement USART2_IRQHandler to handle the RXNE interrupt.
10. In the interrupt handler, read data from the DR register and store it in a buffer.

Author            : Vysakh C S
Date              : 20-02-2024
Development board : STM32F407VGT6 Discovery Board
IDE               : Keil uVision V5.39.0.0

*/
//-----------------------------------------------------------------------------------------------
#include "stm32f407xx.h"

#define BUFFER_SIZE 128

static volatile char rx_buffer[BUFFER_SIZE];
static volatile int rx_buffer_head = 0;
static volatile int rx_buffer_tail = 0;
void USART2_IRQHandler(void);
void gpio_config(void);
void uart2_config(void);
void uart2_rx_handler(void);
char received_char;
//-----------------------------------------------------------------------------------------------
int main(void)
{
  /* GPIO and UART2 Configuration setting function call */
  gpio_config();
  uart2_config();
  
  while(1)
  {   
    // Your main code here
  }
}
//-----------------------------------------------------------------------------------------------
/* GPIO configurations */
/* PA2 is connected to the USART2_TX and PA3 is connected to the USART2_RX */
void gpio_config(void)
{
  /* Clock access enabled for GPIOA */
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
  
  /* PA2 and PA3 set to alternate function mode */
  GPIOA->MODER |= GPIO_MODER_MODE2_1 | GPIO_MODER_MODE3_1;
  GPIOA->MODER &= ~(GPIO_MODER_MODE2_0 | GPIO_MODER_MODE3_0);
  
  /* PA2 and PA3 set to AF7 */
  /* GPIOA->AFR[0] is the low register */
  GPIOA->AFR[0] |=  GPIO_AFRL_AFRL2_2 | GPIO_AFRL_AFRL2_1 | GPIO_AFRL_AFRL2_0 |
                    GPIO_AFRL_AFRL3_2 | GPIO_AFRL_AFRL3_1 | GPIO_AFRL_AFRL3_0;
  GPIOA->AFR[0] &= ~(GPIO_AFRL_AFRL2_3 | GPIO_AFRL_AFRL3_3);
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
  USART2->BRR = 0x0683; 
  
  /* Bit 2 RE: Receiver enable */
  /* 1: Receiver is enabled */
  USART2->CR1 |= USART_CR1_RE;
  
  /* Bit 5 RXNEIE: RXNE interrupt enable */
  /* 1: interrupt enable */
  USART2->CR1 |= USART_CR1_RXNEIE;
  
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
/* UART2 receive interrupt handler */
void uart2_rx_handler(void)
{
    received_char = USART2->DR; // Read received character
    rx_buffer[rx_buffer_head++] = received_char; // Store in the buffer
    rx_buffer_head %= BUFFER_SIZE;
}

//-----------------------------------------------------------------------------------------------
void USART2_IRQHandler(void)
{
    if (USART2->SR & USART_SR_RXNE)  // Check RXNE flag
    {
        uart2_rx_handler(); // Handle received data
    }
}
//-----------------------------------------------------------------------------------------------
