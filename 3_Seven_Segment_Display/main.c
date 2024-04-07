//-----------------------------------------------------------------------------------------------
/*
*Program to interface seven segment display (common anode) with the stm32f407vgt6 discovery board.


Steps:

1. Enable clock acces to the GPIOE by accessing 
   'RCC AHB1 peripheral clock enable register (RCC_AHB1ENR)' and setting bit 4 as high.
   
2. Set pin PE0,PE1,PE2,PE3,PE4,PE5,PE6 as output type by accessing 'GPIO port mode register (GPIOx_MODER)'.
   01: General purpose output mode.
   
3. Create an array to store the hex value of digits and alphabets to be printed (0 to 9 and A to F).
 
4. Using for loop print the values. 


Author            : Vysakh C S
Date              : 11-02-2024
Development board : STM32F407VGT6 Discovery Board
IDE               : Keil uVision V5.39.0.0

*/
//-----------------------------------------------------------------------------------------------

#include "stm32f407xx.h"
void clock_config(void);
void gpio_config(void);
void delay(void);
//-----------------------------------------------------------------------------------------------
int main(void)
{
  char arr[16] = {0x000000C0,0x000000F9,0x000000A4,0x000000B0,0x00000099,0x00000092,0x00000082,0x000000F8,0x00000080,0x00000090,0x00000088,0x00000083,0x000000C6,0x000000A1,0x00000086,0x0000008E};
  clock_config();
  gpio_config();
  while(1)
  {
    for (int i =0; i<16; i++)
    {
      //Assigning hex values to the Output Data Register (ODR)
      GPIOE->ODR = arr[i];
      delay();
    }
  }
}
//-----------------------------------------------------------------------------------------------
void clock_config(void)
{
  //Enable clock for GPIOE
  RCC->AHB1ENR |= (1 << 4);

}
//-----------------------------------------------------------------------------------------------
void gpio_config(void)
{
  //Pin 0,1,2,3,4,5,6 of PORTE is set as ouput
  GPIOE->MODER |= (1 << 0 ) | (1 << 2) | (1 << 4) | (1 << 6 ) | (1 << 8) | (1 << 10) | (1 << 12); 
}
//-----------------------------------------------------------------------------------------------

//Function to generate time delay
  void delay(void)
    {
      int i = 0;
      for (i = 0; i < 600000; i++);
    }
//-----------------------------------------------------------------------------------------------