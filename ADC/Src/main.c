
#include<stdio.h>
#include "STM32F407xx.h"
#include<stdint.h>

#define SYS_FREQ 			16000000
#define APB1_CLK 			SYS_FREQ
#define UART_BAUDRATE  		115200
#define USART2EN 			(1 << 17)  //USART2EN: USART2 clock enable pin
//#define  USART2_TX	PD5 AF7
#define GPIODEN 			(1 << 3)

int adc_val = 0;
void uart_tx_init(void);
static uint16_t compute_uart_bd(uint32_t periph_clk,uint32_t baudrate);
static void uart_set_baudrate(USART_TypeDef *USARTx,uint32_t periph_clk,uint32_t baudrate);
void uart2_write(int ch);
//void delay(void);
int __io_putchar(int ch)
{
	uart2_write(ch);
	return ch;
}



int main(void)
{
	uart_tx_init();

	//Enable clock access to GPIOF
	//Bit 5 GPIOFEN: IO port F clock enable
	RCC->AHB1ENR |= (RCC_AHB1ENR_GPIOFEN);


	//Set the mode of PF6 to analog
	//PF6 - ADC3_IN4
	//Analog Mode
	GPIOF->MODER |= (GPIO_MODER_MODE6);

	//Configure the ADC module
	//Enable clock access to ADC
	//ADC1EN: ADC3 clock enable
	RCC->APB2ENR |= (RCC_APB2ENR_ADC3EN);


	//Conversion sequence start
	ADC3->SQR3 = (ADC_SQR3_SQ1_0);

	//conversion sequence length
	ADC3->SQR1 |= 0x00;

	//Enable ADC module
	//ADC control register 2 (ADC_CR2)
	//ADON: A/D Converter ON / OFF
	//ADC ON
	ADC3->CR2 |= (ADC_CR2_ADON);

	while(1)
	{
		//Start conversion of regular channels
		//This bit is set by software to start conversion and cleared by hardware as soon as the conversion starts.
		ADC3->CR2 |= (ADC_CR2_SWSTART);

		//Checking if the conversion is complete
		while(!(ADC3->SR & ADC_SR_EOC)){};



		//Assigning the ADC value to the variable and printing it
		int adc_val = (ADC3->DR);
		printf("%d\n\r",adc_val);

	}
return 0;
}


void uart_tx_init(void)
{
	//configure uart gpio pin

	 //Enable clock access to GPIOD
	RCC->AHB1ENR |= GPIODEN;
	 //set pd5 and pd6 to alternate fn mode
	 GPIOD->MODER |= (1 << 11);
	 GPIOD->MODER &= ~(1 << 10);

	 GPIOD->MODER |= (1 << 13);
	 GPIOD->MODER &= ~(1 << 12);
	 //set pd5 to alt fn type to uart tx
	GPIOD->AFR[0] &= ~(1 << 23);
	GPIOD->AFR[0] |= (1 << 22) | (1 <<21) | (1 << 20);

	GPIOD->AFR[0] &= ~(1 << 27);
	GPIOD->AFR[0] |= (1 << 26) | (1 <<25) | (1 << 24);


	//enable clock access to uart2
	 RCC->APB1ENR |= USART2EN; //Clock enabled

	 //configure baudrate
	 uart_set_baudrate(USART2,APB1_CLK,UART_BAUDRATE);
	 //configrue transfer direction
	 USART2->CR1 =(1 << 3);
	 //enable uart module
	 USART2->CR1 |= (1 << 13);


}


static void uart_set_baudrate(USART_TypeDef *USARTx,uint32_t periph_clk,uint32_t baudrate)
{
	USARTx->BRR = compute_uart_bd(periph_clk,baudrate);
}
static uint16_t compute_uart_bd(uint32_t periph_clk,uint32_t baudrate)
{
	return ( (periph_clk + (baudrate/2U))/baudrate);
}

void uart2_write(int ch)
{
	//Make sure the transmit data register is empty

	while (!(USART2->SR  & (1 << 7))){}
	//Write to transmit data reigster

	USART2->DR = (ch & 0xFF);
}

//void delay(void) {
//	int i = 0;
//	for (i = 0; i < 50000; i++);
//}



