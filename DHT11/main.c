
//-----------------------------------------------------------------------------------------------
/*
*Program to interface DHT11 with the stm32f407vgt6 discovery board.

GPIOA -> PIN 1  -> DHT11

Steps:



Author            : Vysakh C S
Date              : 10-02-2024
Development board : STM32F407VGT6 Discovery Board
IDE               : Keil uVision V5.39.0.0
*/

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
int __io_putchar(int ch);
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



void clock_config(void);
void gpio_input(void);
void gpio_output(void);
void mcu_send_start(void);
void dht11_response(void);
void pin_high(void);
void pin_low(void);
void delay_us(uint32_t us);
void dht11_data(void);
int inputValue = 0;
uint16_t data = 0;
uint8_t byte1,byte2;

//-----------------------------------------------------------------------------------------------
int main(void)
{
  clock_config();
    uart2_config();
  while(1)
  {    
		   mcu_send_start(); 
		   dht11_response();
		   dht11_data();
	     delay_us(2000000); //2s	
  }
	
}
//-----------------------------------------------------------------------------------------------
void clock_config(void)
{
  //Enable clock for GPIOA
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
  
   // Bit 4 TIM6EN: TIM6 clock enable
   RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
	
	 //Enable clock for GPIOD
   RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
	 // GPIOD pin - 12 set to output
    GPIOD->MODER |= GPIO_MODER_MODER12_0;
  
   /*UART*/
  
  /*PA2 set to alternate function mode */
  GPIOA->MODER |= GPIO_MODER_MODE2_1;
  GPIOA->MODER &= ~GPIO_MODER_MODE2_0;
  
  /* PA2 set to AF7 */
  /* GPIOA->AFR[0] is the low register */
  GPIOA->AFR[0] |=  GPIO_AFRL_AFRL2_2 | GPIO_AFRL_AFRL2_1 | GPIO_AFRL_AFRL2_0;
  GPIOA->AFR[0] &= ~GPIO_AFRL_AFRL2_3;
}
//-----------------------------------------------------------------------------------------------
void gpio_input(void)
{ 
  //PA1 as input
  GPIOA->MODER &= ~GPIO_MODER_MODE1_0;
  GPIOA->MODER &= ~GPIO_MODER_MODE1_1;
}
//-----------------------------------------------------------------------------------------------
void gpio_output(void)
{ 
  //PA1 as output
  GPIOA->MODER |= GPIO_MODER_MODE1_0;
  GPIOA->MODER &= ~GPIO_MODER_MODE1_1;
}
//-----------------------------------------------------------------------------------------------
void delay_us(uint32_t us) {
    // Delay loop
    for(uint32_t i = 0; i < us; i++) {
        __NOP(); // No operation, just to consume time
    }
}
//-----------------------------------------------------------------------------------------------
  void pin_high(void)
  {
    GPIOA->ODR |= GPIO_ODR_OD1;
    
  }
//-----------------------------------------------------------------------------------------------
    void pin_low(void)
  {
    GPIOA->ODR &= ~GPIO_ODR_OD1; 
  }
 //-----------------------------------------------------------------------------------------------
	void mcu_send_start(void)
	{
  gpio_output();
	pin_high();
  pin_low();
	delay_us(18000); //18ms
  pin_high();
	delay_us(40); //40us
  gpio_input();
	}
//-----------------------------------------------------------------------------------------------
	void dht11_response(void)
	{
			
      inputValue = (GPIOA->IDR & GPIO_IDR_ID1)? 1 : 0;
    if(inputValue == 0)
     {
       //printf("54: %d\n\r",inputValue);
       delay_us(54); //54us
     }
      
     inputValue = (GPIOA->IDR & GPIO_IDR_ID1)? 1 : 0;
     if(inputValue == 1)
     {
       //printf("80: %d\n\r",inputValue);
       
       delay_us(80); //80us
     }
	}
//-----------------------------------------------------------------------------------------------
	void dht11_data(void)
	{
    for (int i = 0; i<16; i++)
    {
    inputValue = (GPIOA->IDR & GPIO_IDR_ID1) ? 1 : 0;
     if(inputValue == 0)
     {
       delay_us(54); //54us
     }
     
     delay_us(40); //54us
     inputValue = (GPIOA->IDR & GPIO_IDR_ID1) ? 1 : 0;
     if(inputValue == 0)
     {
       
       data =  0;
         printf("1 : %d\n\r",data);
       
     }
     else{
       data =  1;
        printf("2 : %d\n\r",data); 
     }
   }
    byte1 = data >> 8;
    byte2 = data & 0xff;
  
  
	}
//-----------------------------------------------------------------------------------------------
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
