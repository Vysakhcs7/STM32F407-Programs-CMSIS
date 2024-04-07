//-----------------------------------------------------------------------------------------------
/*
*Program to toggle the inbuild led of stm32f407 discovery board.

GPIOD -> PIN 12 -> Green LED
GPIOD -> PIN 13 -> Orange LED
GPIOD -> PIN 14 -> Red LED
GPIOD -> PIN 15 -> Blue LED

Steps:

1. Enable clock acces to the GPIOD by accessing 
   'RCC AHB1 peripheral clock enable register (RCC_AHB1ENR)' and setting bit 3 as high.
   
2. Set pins 12,13,14,15 of GPIOD as ouput type by accessing 'GPIO port mode register (GPIOx_MODER)'.
   01: General purpose output mode.
   
3. To turn on and off the led set the corresponding bits in the 
   'GPIO port bit set/reset register (GPIOx_BSRR)'.
   
4. Call delay function

Author            : Vysakh C S
Date              : 10-02-2024
Development board : STM32F407VGT6 Discovery Board
IDE               : Keil uVision V5.39.0.0

*/

//-----------------------------------------------------------------------------------------------

  #include "stm32f407xx.h"
  void clock_config(void);
  void gpio_config(void);
  void delay(void);
//-----------------------------------------------------------------------------------------------

  int main(void){
    clock_config();
    gpio_config();
    while(1){
      for (int i = 12; i <= 15; i++) {
      GPIOD->BSRR |= (1 << i);
      delay();
      }

      for (int j = 28; j <= 31; j++) {
      GPIOD->BSRR |= (1 << j);
      delay();
    }
    }
  }
//-----------------------------------------------------------------------------------------------
 //Enabling clock access to the GPIOD
  
  void clock_config(void)
    {
      //Setting 3rd bit in the RCC_AHB1ENR register.
      RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    }
//-----------------------------------------------------------------------------------------------
//Setting MODE register to output type
    
  void gpio_config(void)
    {
      //GPIOD pins - 12,13,14,15 set to output.
      GPIOD->MODER |= GPIO_MODER_MODER12_0 | GPIO_MODER_MODER13_0 | GPIO_MODER_MODER14_0 | GPIO_MODER_MODER15_0;
    }
//-----------------------------------------------------------------------------------------------
//Function to generate time delay
    
  void delay(void)
    {
      int i = 0;
      for (i = 0; i < 70000; i++);
    }
//-----------------------------------------------------------------------------------------------
   