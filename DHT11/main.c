
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
void uart2_config(void);
void uart2_single_write(int ch);
void delay(void);
int __io_putchar(int ch);
void read_dht11(uint8_t *temperature, uint8_t *humidity);

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
int inputValue = 0;
 int c = 0;
void read(void);
//-----------------------------------------------------------------------------------------------
int main(void)
{
   uint8_t temperature, humidity;
  clock_config();
    uart2_config();
  while(1)
  {   
		   mcu_send_start(); 
		   dht11_response();
       read();
//		   read_dht11(&temperature, &humidity);
//        printf("Temperature: %d\n", temperature);
//        printf("Humidity: %d%%\n", humidity);
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
void read_dht11(uint8_t *temperature, uint8_t *humidity)
{
    uint8_t data[5] = {0};
    // Data is transmitted in 40 bits, first 2 bytes are humidity data, the next 2 bytes are temperature data, and the last byte is checksum
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 8; j++) {
            // Wait for the falling edge of the signal
            while (!(GPIOA->IDR & GPIO_IDR_ID1));
            
            // Measure the width of the pulse
            delay_us(30); // DHT11 data pin will be low for 26-28 us for '0' and 70us for '1'
            if (GPIOA->IDR & GPIO_IDR_ID1) { // Check if the pin is still high after the delay
                data[i] |= (1 << (7 - j)); // Store '1'
            }
            // Wait for the signal to go back high
            while (GPIOA->IDR & GPIO_IDR_ID1);
        }
    }
    // Checksum verification
    if ((data[0] + data[1] + data[2] + data[3]) == data[4]) {
        *humidity = data[0];
        *temperature = data[2];
    } else {
        // Checksum error
        *humidity = 0;
        *temperature = 0;
    }
}
//-------------------------------------------------------------------------------------------------------------
void read(void)
{
    for(int k =0; k<40; k++)
  {
     while (!(GPIOA->IDR & GPIO_IDR_ID1));

    delay_us(30);
    if(GPIOA->IDR & GPIO_IDR_ID1)
    {
    c = 1;
    }
    else
    {
     c = 0;
    }
    while (GPIOA->IDR & GPIO_IDR_ID1);
    printf("%d",c);
  }
      printf("\n\r");
}
