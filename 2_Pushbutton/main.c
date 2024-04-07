//-----------------------------------------------------------------------------------------------
/*
*Program to interface inbuild pushbutton with the inbuild led on the stm32f407vgt6 discovery board.

GPIOD -> PIN 12 -> Green LED
GPIOA -> PIN 0  -> Pushbutton

Steps:

1. Enable clock acces to the GPIOD by accessing 
   'RCC AHB1 peripheral clock enable register (RCC_AHB1ENR)' and setting bit 3 as high.
   
2. Enable clock acces to the GPIOA by accessing 
   'RCC AHB1 peripheral clock enable register (RCC_AHB1ENR)' and setting bit 0 as high.
   
3. Set pin PA0 (MODER0) as input type by accessing 'GPIO port mode register (GPIOx_MODER)'.
   00: General purpose input mode.
   
4. Set pin PD12 (MODER12) as output type by accessing 'GPIO port mode register (GPIOx_MODER)'.
   01: General purpose output mode.
   
5. Check if button is pressed or not and according to that toggle the led.

Author            : Vysakh C S
Date              : 10-02-2024
Development board : STM32F407VGT6 Discovery Board
IDE               : Keil uVision V5.39.0.0

*/
//-----------------------------------------------------------------------------------------------

#include "stm32f407xx.h"
void clock_config(void);
void gpio_config(void);
//-----------------------------------------------------------------------------------------------
int main(void)
{
  clock_config();
  gpio_config();
  while(1)
  {
    if(GPIOA->IDR & GPIO_IDR_IDR_0)
    {
      //Setting bit PD12
      GPIOD->BSRR |= GPIO_BSRR_BS12;
    }
    else
    {
       //Resetting bit PD12
      GPIOD->BSRR |= GPIO_BSRR_BR12;
    }
   
  }
}
//-----------------------------------------------------------------------------------------------
void clock_config(void)
{
  //Enable clock for GPIOD and GPIOA
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
}
//-----------------------------------------------------------------------------------------------
void gpio_config(void)
{
  //PD12 as Output
  GPIOD->MODER |= GPIO_MODER_MODE12_0;
  GPIOD->MODER &= ~GPIO_MODER_MODE12_1;
  
  //PA0 as input
  GPIOA->MODER &= ~GPIO_MODER_MODE0_0;
  GPIOA->MODER &= ~GPIO_MODER_MODE0_1;
}
//-----------------------------------------------------------------------------------------------