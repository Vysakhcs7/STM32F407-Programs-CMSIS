
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


#include "stm32f407xx.h"

void clock_config(void);
void gpio_input(void);
void gpio_output(void);
void mcu_send_start(void);
void dht11_response(void);
void pin_high(void);
void pin_low(void);
void delay_us(uint32_t us);
	void dht11_data(void);
		uint32_t data = 0;
//-----------------------------------------------------------------------------------------------
int main(void)
{
  clock_config();
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
	}
//-----------------------------------------------------------------------------------------------
	void dht11_response(void)
	{
			gpio_input();

	}
//-----------------------------------------------------------------------------------------------
	void dht11_data(void)
	{
			  if( (GPIOA->IDR & GPIO_IDR_IDR_1) == 0 )
			{
			  delay_us(50); //50us
			}
		  data =  GPIOA->IDR & GPIO_IDR_IDR_1;
			delay_us(70); //70us
		
	}
//-----------------------------------------------------------------------------------------------
