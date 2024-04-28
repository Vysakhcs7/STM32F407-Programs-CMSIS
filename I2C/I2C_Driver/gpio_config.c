#include "gpio_config.h"
void gpio_config(void)
{  
    // 10: Alternate function mode
    GPIOB->MODER |= GPIO_MODER_MODE6_1 |  GPIO_MODER_MODE7_1;
  
    // 0100: AF4
    GPIOB->AFR[0] |= GPIO_AFRL_AFRL6_2 | GPIO_AFRL_AFRL7_2;
  
    // 1: Output open-drain
    GPIOB->OTYPER |= GPIO_OTYPER_OT6 | GPIO_OTYPER_OT7;
	 
	  //11: Very high speed
	  GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR6 | GPIO_OSPEEDER_OSPEEDR7;
	
	  //01: Pull-up
    GPIOB->PUPDR |= GPIO_PUPDR_PUPD6_0 | GPIO_PUPDR_PUPD7_0;
	
}