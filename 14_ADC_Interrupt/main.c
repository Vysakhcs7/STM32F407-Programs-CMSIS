//-----------------------------------------------------------------------------------------------
/*
* Program to read an ADC value using interrupt.

*Procedure:

1. Enable ADC clock to ADC in RCC_APB2ENR register.
2. Configure ADC right adjusted result (ALIGN), free running conversion
   (CONT, continuous conversion) using ADC_CR2 register.
3. Using ADC_CR1 register, select the resolution and the analog input
   channel.
4. Use GPIOx_MODER register to configure input pin for analog input
   channel.
5. Using ADC_CR2 register Enable the ADC (ADON) and START
   (SWSTART) to start the conversion.
6. Monitor the EOC flag in the ADC_SR register for end-of-conversion. 
7. When the EOC flag goes HIGH, read the ADC data result from the
   ADC_DR register and save it.
8. Repeat steps 6 through 7 for the next conversion


Author            : Vysakh C S
Date              : 21-02-2024
Development board : STM32F407VGT6 Discovery Board
IDE               : Keil uVision V5.39.0.0

*/
//-----------------------------------------------------------------------------------------------
#include "stm32f407xx.h"
void gpio_config(void);
void adc1_config(void);
void ADC_IRQHandler(void);
static uint16_t result = 0;
//-----------------------------------------------------------------------------------------------
int main(void)
{
  
  /* GPIO and UART2 Conmfiguration setting function call */
  gpio_config();
  adc1_config();
  while(1)
  {
		
  }
}
//-----------------------------------------------------------------------------------------------
  /*GPIO configurations */
 /* */
 void gpio_config(void)
 {
  /*Clock access enabled for GPIOA */
  RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
  
  /*PA1 set to analog mode */
  GPIOA->MODER |= GPIO_MODER_MODE1_0;
  GPIOA->MODER |= GPIO_MODER_MODE1_1;
 }
//-----------------------------------------------------------------------------------------------
void adc1_config(void)
{
	/*Clock access enabled for ADC1 */
  /* Bit 8 ADC1EN: ADC1 clock enable */
  /* 1: ADC1 clock enabled */
  RCC->APB2ENR  |= RCC_APB2ENR_ADC1EN;
  
  /* Bit 11 ALIGN: Data alignment */
  /* 0: Right alignment */
  ADC1->CR2 &= ~ADC_CR2_ALIGN;

  /* Bit 1 CONT: Continuous conversion */
  /* 1: Continuous conversion mode */
  ADC1->CR2 |= ADC_CR2_CONT;
  
  /* Bits 25:24 RES[1:0]: Resolution */
  /* 00: 12-bit (15 ADCCLK cycles) */
  ADC1->CR1 &= ~ADC_CR1_RES_0 & ~ADC_CR1_RES_1;
	
	/* Bit 5 EOCIE: Interrupt enable for EOC */
	ADC1->CR1 |= ADC_CR1_EOCIE;
	
	/* Enable ADC interrupt in NVIC */
	NVIC_EnableIRQ(ADC_IRQn);
	
	/* Bits 4:0 AWDCH[4:0]: Analog watchdog channel select bits */
	/* 00001: ADC analog input Channel **/
	ADC1->CR1 |= ADC_CR1_AWDCH_0;
	
	/* Bits 4:0 SQ1[4:0]: 1st conversion in regular sequence */
	ADC1->SQR3 |= ADC_SQR3_SQ1_0;
  
  /* Bit 0 ADON: A/D Converter ON / OFF */
  /* 1: Enable ADC */
  ADC1->CR2 |= ADC_CR2_ADON;
  
	/* Bit 30 SWSTART: Start conversion of regular channels */
  /* 1: Starts conversion of regular channel */
  ADC1->CR2 |= ADC_CR2_SWSTART;
	
}
//-----------------------------------------------------------------------------------------------
void ADC_IRQHandler(void)
{
	if( (ADC1->SR & ADC_SR_EOC) != 0)
	{
		/* Clear the flag */
		ADC1->SR &= ~ADC_SR_EOC;
		result =  (uint16_t)ADC1->DR;
	}
}

//-----------------------------------------------------------------------------------------------
