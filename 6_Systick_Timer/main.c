/* p5_3.c Toggle Green LED (LD2) on STM32F446RE Nucleo64 using
SysTick *
* This program uses SysTick to generate multiples of millisecond
delay. * System clock is running at 16 MHz. SysTick is configure *
to count down from 16000 to zero to give a 1 ms delay.
* A for loop counts how many millisecond the delay should be. * When
1000 is used for loop count, the delay is 1000 ms or 1 second. *
* This program was tested with Keil uVision v5.24a with DFP v2.11.0
*/


//-----------------------------------------------------------------------------------------------
/*

GPIOD -> PIN 14 -> Red LED


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
  void systick_config(void);
  void systick_reload_value(void);
  void SysTick_Handler(void);

//-----------------------------------------------------------------------------------------------

  int main(void){
    clock_config();
    gpio_config();
    systick_config();
    systick_reload_value();
    while(1){
    
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
      //GPIOD pin - 14 set to output.
      GPIOD->MODER |= GPIO_MODER_MODER14_0;
    }
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
//Functino to configure systick timer
    
/*[2]CLKSOURCE Indicates the clock source:
  0 = external clock
  1 = processor clock.
  [1] TICKINT Enables SysTick exception request:
  0 = counting down to zero does not assert the SysTick exception request
  1 = counting down to zero asserts the SysTick exception request.
  Software can use COUNTFLAG to determine if SysTick has ever counted to zero.
  [0] ENABLE Enables the counter:
  0 = counter disabled
  1 = counter enabled
*/
    
    void systick_config(void)
    {
      //SysTick->CTRL |= SysTick_CTRL_ENABLE_Pos | SysTick_CTRL_TICKINT_Pos | SysTick_CTRL_CLKSOURCE_Pos;
      SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk | SysTick_CTRL_TICKINT_Msk | SysTick_CTRL_CLKSOURCE_Msk;
      //SysTick->CTRL = 5; 
    }
//-----------------------------------------------------------------------------------------------
//Functino to load SysTick Value
    
/*
*/
    
    void systick_reload_value(void)
    {
      SysTick->LOAD = 8000000 - 1;
      //SysTick->VAL = 0;
      
    }
//-----------------------------------------------------------------------------------------------
    
  void SysTick_Handler(void)
  {
      GPIOD->ODR ^= GPIO_ODR_OD14;
      
  }  
 //-----------------------------------------------------------------------------------------------
 