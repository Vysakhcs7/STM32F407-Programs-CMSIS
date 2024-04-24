//-----------------------------------------------------------------------------------------------
/* 
Procedure:
1. Select the counter clock (internal, external, prescaler).
2. Write the desired data in the TIMx_ARR and TIMx_CCRx registers.
3. Set the CCxIE and/or CCxDE bits if an interrupt and/or a DMA request is to be 
generated.
4. Select the output mode. For example, the user must write OCxM=011, OCxPE=0, 
CCxP=0 and CCxE=1 to toggle OCx output pin when CNT matches CCRx, CCRx 
preload is not used, OCx is enabled and active high.
5. Enable the counter by setting the CEN bit in the TIMx_CR1 register

Here the interuppt handler is used to toggle another led.Without the interrupt handler also
this program will work by toggling the PA5 LED.

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
void TIM2_IRQHandler(void);

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

// Function to configure clock settings
void clock_config(void)
{
    // Enable clock access to GPIOA and GPIOD
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIODEN;
    
    // Enable TIM2 clock
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;   
}

//-----------------------------------------------------------------------------------------------

// Function to configure GPIO settings
void gpio_config(void)
{
    // Configure PA5 as alternate function mode
    GPIOA->MODER &= ~GPIO_MODER_MODER5_0;
    GPIOA->MODER |= GPIO_MODER_MODER5_1;
    
    // Set alternate function AF1 for PA5
    GPIOA->AFR[0] |= GPIO_AFRL_AFRL5_0;
    
    // Configure GPIOD pin 13 as output
    GPIOD->MODER |= GPIO_MODER_MODER13_0;
}

//-----------------------------------------------------------------------------------------------

// Function to configure timer settings
void timer_config(void)
{
    // Set prescaler value to get 1MHz
    TIM2->PSC = 15999; 
    
    // Set auto-reload value to get 1 second interrupt
    TIM2->ARR = 999;
    
    // Enable TIM2 Update Interrupt
    TIM2->DIER |= TIM_DIER_UIE;
    NVIC_EnableIRQ(TIM2_IRQn);

    // Configure TIM2 to toggle on output compare match
    TIM2->CCMR1 |= TIM_CCMR1_OC1M_0 | TIM_CCMR1_OC1M_1;
    TIM2->CCMR1 &= ~TIM_CCMR1_OC1M_2;
    
    // Configure output polarity as active high
    TIM2->CCER &= ~TIM_CCER_CC1P;
    
    // Enable capture/compare output
    TIM2->CCER |= TIM_CCER_CC1E;
    
    // Enable the timer
    TIM2->CR1 |= TIM_CR1_CEN;
}

//-----------------------------------------------------------------------------------------------

// TIM2 interrupt handler
void TIM2_IRQHandler(void)
{
    // Check if the update interrupt flag is set
    if(TIM2->SR & TIM_SR_UIF)
    {
        // Clear the update interrupt flag
        TIM2->SR &= ~TIM_SR_UIF;
        
        // Toggle the LED
        GPIOD->ODR ^= GPIO_ODR_OD13;
    }
}
