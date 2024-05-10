//-----------------------------------------------------------------------------------------------
/* 
To toggle led connected to PD12 every one second using Timer 6 with interrupt enabled.
Formula 
Timer Frequency = System_Clock/Prescalar
Time Period = 1/Timer Frequency
For generating 1000ms delay, Auto-reload value = 1000ms/Time Period(in ms)


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
void TIM6_DAC_IRQHandler(void);

//-----------------------------------------------------------------------------------------------

int main(void)
{
    clock_config();
    gpio_config();
    timer_config();
    
    while(1)
    {
        
    }
}

//-----------------------------------------------------------------------------------------------

// Enabling clock access to the GPIOD and TIM6
void clock_config(void)
{
    //Enable clock for GPIOD
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    
    // Bit 4 TIM6EN: TIM6 clock enable
    RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
}

//-----------------------------------------------------------------------------------------------

// Setting MODE register to output type
void gpio_config(void)
{
    // GPIOD pin - 12 set to output
    GPIOD->MODER |= GPIO_MODER_MODER12_0;
}

//-----------------------------------------------------------------------------------------------

void timer_config(void)
{
    /* Prescaler value */
    TIM6->PSC = 250;  //For 100ms, 16Mhz/250 = 64000Hz
    /* Auto-reload value */
    TIM6->ARR = 64000; //For 100ms, T=1/64000Hz = .015625ms, ARR = 1000ms/.015625 = 64000.
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
        /* Clear the flag */
        TIM6->SR &= ~TIM_SR_UIF;
        /* Toggle the LED */
        GPIOD->ODR ^= GPIO_ODR_OD12;
    }
}
