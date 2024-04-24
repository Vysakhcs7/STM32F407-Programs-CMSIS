//-----------------------------------------------------------------------------------------------
/*

Program the peripheral input clock in I2C_CR2 register in order to generate correct 
timings
• Configure the clock control registers
• Configure the rise time register
• Program the I2C_CR1 register to enable the peripheral
• Set the START bit in the I2C_CR1 register to generate a Start condition
The peripheral input clock frequency must be at least:
• 2 MHz in Sm mode 
• 4 MHz in Fm mode

I2C1 - AF4 - PB6 - SCL PB7 - SDA

Author            : Vysakh C S
Date              : 25-04-2024
Development board : STM32F407VGT6 Discovery Board
IDE               : Keil uVision V5.39.0.0

*/

//-----------------------------------------------------------------------------------------------
  #include "stm32f407xx.h"
  void I2C_init(void);
//-----------------------------------------------------------------------------------------------

  int main(void){
  I2C_init();
    while(1){

    }
  }
//-----------------------------------------------------------------------------------------------
void I2C_init(void)
{
  //Enable clock access for I2C1
  RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
  
  //Enable clock access for GPIOB
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
  
  //10: Alternate function mode
  GPIOB->MODER |= GPIO_MODER_MODE6_1;
   GPIOB->MODER |= GPIO_MODER_MODE7_1;
  
  //0100: AF4
  GPIOB->AFR[0] |= GPIO_AFRL_AFRL6_2;
  GPIOB->AFR[0] |= GPIO_AFRL_AFRL7_2;
  
  //1: Output open-drain
  GPIOB->OTYPER |= GPIO_OTYPER_OT6;
  GPIOB->OTYPER |= GPIO_OTYPER_OT7;
  
  //Clock frequency
  I2C1->CR2 = ;
  
  //
}  
//-----------------------------------------------------------------------------------------------