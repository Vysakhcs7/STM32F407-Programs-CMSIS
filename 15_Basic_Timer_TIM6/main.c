//-----------------------------------------------------------------------------------------------
/* 
TIM6 - APB1

Author            : Vysakh C S
Date              : 22-04-2024
Development board : STM32F407VGT6 Discovery Board
IDE               : Keil uVision V5.39.0.0

*/

//-----------------------------------------------------------------------------------------------

  #include "stm32f407xx.h"
  void clock_config(void);
  void gpio_config(void);
  void timer_config(void);
//-----------------------------------------------------------------------------------------------

  int main(void){
    clock_config();
    gpio_config();
    timer_config();
    while(1){

}
    }
//-----------------------------------------------------------------------------------------------
 //Enabling clock access to the GPIOD and TIM6
  
  void clock_config(void)
    {
      //Setting 3rd bit in the RCC_AHB1ENR register.
      RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
      
      //Bit 4 TIM6EN: TIM6 clock enable
      RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
      
    }
//-----------------------------------------------------------------------------------------------
//Setting MODE register to output type
    
  void gpio_config(void)
    {
      //GPIOD pin - 12 set to output.
      GPIOD->MODER |= GPIO_MODER_MODER12_0;
    }
//-----------------------------------------------------------------------------------------------
  void timer_config(void)
  {
		/* Prescaler value */
		TIM6->PSC = 0x00;
		/* Auto-reload value */
		TIM6->ARR = 0;

  }
  //-----------------------------------------------------------------------------------------------
  
  
  
  