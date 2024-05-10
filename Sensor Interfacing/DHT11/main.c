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

#define DEBUG_MODE
#define TEST_MODE

void clock_config(void);
void gpio_input(void);
void gpio_output(void);
void TIM6_delay_us(uint16_t us);
void TIM6_delay_ms(uint16_t ms);
void TIM6_DAC_IRQHandler(void);
void mcu_send_start(void);
void dht11_response(void);
void pin_high(void);
void pin_low(void);
//-----------------------------------------------------------------------------------------------
int main(void)
{
  clock_config();
  while(1)
  {
		   mcu_send_start();  
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
void TIM6_delay_us(uint16_t us)
{
     // Reset TIM6
    //RCC->APB1RSTR |= RCC_APB1RSTR_TIM6RST;
    //RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM6RST;
	
    /* Prescaler value */
    TIM6->PSC = 16;  //For 1us
	
    /* Auto-reload value */
     TIM6->ARR = us;
	
    /* Update interrupt enable */
    TIM6->DIER |= TIM_DIER_UIE;
	
    /* Enable TIM6 interrupt in NVIC */
    NVIC_EnableIRQ(TIM6_DAC_IRQn);
	
    /* CEN: Counter enable */
    TIM6->CR1 |= TIM_CR1_CEN;
}
//-----------------------------------------------------------------------------------------------
void TIM6_DAC_IRQHandler(void)
{
    if(TIM6->SR & TIM_SR_UIF)
    {
        // Clear the flag
        TIM6->SR &= ~TIM_SR_UIF;
			
			  /* Toggle the LED */
        GPIOD->ODR ^= GPIO_ODR_OD12;
			  //GPIOA->ODR ^= GPIO_ODR_OD1;
    }
}
//-----------------------------------------------------------------------------------------------
void TIM6_delay_ms(uint16_t ms)
{
     // Reset TIM6
    //RCC->APB1RSTR |= RCC_APB1RSTR_TIM6RST;
   //RCC->APB1RSTR &= ~RCC_APB1RSTR_TIM6RST;
	
    /* Prescaler value */
    TIM6->PSC = 16000;  //1ms
	
    /* Auto-reload value */
     TIM6->ARR = ms;
	
    /* Update interrupt enable */
    TIM6->DIER |= TIM_DIER_UIE;
	
    /* Enable TIM6 interrupt in NVIC */
    NVIC_EnableIRQ(TIM6_DAC_IRQn);
	
    /* CEN: Counter enable */
    TIM6->CR1 |= TIM_CR1_CEN;
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
	TIM6_delay_ms(18); //18ms delay
  pin_high();
	TIM6_delay_us(40); //40us delay
	}
//-----------------------------------------------------------------------------------------------
	void dht11_response(void)
	{
			gpio_input();
		  if( (GPIOA->IDR & GPIO_IDR_IDR_1) == 0 )
			{
				 TIM6_delay_us(80); //80us delay
			}
			
			
			 if( (GPIOA->IDR & GPIO_IDR_IDR_1) == 1 )
			{
				 TIM6_delay_us(80); //80us delay
			}
		 
	}
//-----------------------------------------------------------------------------------------------
	
//-----------------------------------------------------------------------------------------------
	
//-----------------------------------------------------------------------------------------------