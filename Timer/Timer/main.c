//-----------------------------------------------------------------------------------------------
/* Toggle Green LED (PD12) on STM32F446RE Nucleo64 using TIM2
*
* This program configures TIM2 with prescaler divides by 1600 * and
counter wraps around at 10000. So the 16 MHz system clock * is
divided by 1600 then divided by 10000 to become 1 Hz. * Every time
the counter wraps around, it sets UIF flag * (bit 0 of TIM2_SR
register). The program waits for the * UIF to set then toggles the
LED (PD12).

Author            : Vysakh C S
Date              : 02-03-2024
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
      while(!(TIM2->SR & 1));      /* wait until UIF set */
      TIM2->SR &= (uint32_t)~1;              /* clear UIF */
      GPIOD->ODR ^= GPIO_ODR_OD12; /* toggle green LED */
}
    }
//-----------------------------------------------------------------------------------------------
 //Enabling clock access to the GPIOD and TIM2
  
  void clock_config(void)
    {
      //Setting 3rd bit in the RCC_AHB1ENR register.
      RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
      
      //Bit 0 TIM2EN: TIM2 clock enable
      RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
      
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
    TIM2->PSC = 1600 - 1; /*divided by 1600 */ 
    TIM2->ARR = 10000 - 1; /* divided by 10000 */
    TIM2->CNT = 0; /* clear timer counter */ 
    TIM2->CR1 = 1; /* enable TIM3 */
  }
  //-----------------------------------------------------------------------------------------------
  
  
  
  