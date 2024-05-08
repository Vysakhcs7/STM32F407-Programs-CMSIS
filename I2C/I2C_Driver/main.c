//-----------------------------------------------------------------------------------------------
/*

I2C1 - AF4 - PB6 - SCL PB7 - SDA
1101000 - 0X68 - ADDRESS
1101000'0' - 0xD0 - Write <<
1101000'1' - 0XD1 - Read  |

Author            : Vysakh C S
Date              : 25-04-2024
Development board : STM32F407VGT6 Discovery Board
IDE               : Keil uVision V5.39.0.0

*/

//-----------------------------------------------------------------------------------------------
#include "stm32f407xx.h"
void rcc_config(void);
void gpio_config(void);
void i2c_config(void);

void i2c_start(void);
void i2c_addr_write(char saddr);
void i2c_data_write(uint8_t data);
void i2c_stop(void);

void delay(void);
//-----------------------------------------------------------------------------------------------

int main(void){
    rcc_config();
    gpio_config();
    i2c_config();
 while(1)
	{

  }
}

//-----------------------------------------------------------------------------------------------
void rcc_config(void) {
    // Enable clock access for I2C1
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

    // Enable clock access for GPIOB
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
}
//-----------------------------------------------------------------------------------------------
void gpio_config(void) {
    // 10: Alternate function mode
    GPIOB->MODER |= GPIO_MODER_MODE6_1 |  GPIO_MODER_MODE7_1;

    // 0100: AF4
    GPIOB->AFR[0] |= GPIO_AFRL_AFRL6_2 | GPIO_AFRL_AFRL7_2;

    // 1: Output open-drain
    GPIOB->OTYPER |= GPIO_OTYPER_OT6 | GPIO_OTYPER_OT7;

    // 11: Very high speed
    GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR6 | GPIO_OSPEEDER_OSPEEDR7;

    // 01: Pull-up
    GPIOB->PUPDR |= GPIO_PUPDR_PUPD6_0 | GPIO_PUPDR_PUPD7_0;
}
//-----------------------------------------------------------------------------------------------
void i2c_config(void) {
    // Reset the I2C
    I2C1->CR1 |= I2C_CR1_SWRST;
  
    // Normal operation
    I2C1->CR1 &= ~I2C_CR1_SWRST;
  
    // Clock frequency - 16MHz
    I2C1->CR2 = 0x10;
  
    // Standard mode, 100kHz clock 
    I2C1->CCR = 0x50;
  
    // Maximum rise time
    I2C1->TRISE = 0x10;
  
    // PE: Peripheral enable
    I2C1->CR1 |= I2C_CR1_PE;
} 
//-----------------------------------------------------------------------------------------------
void i2c_start(void) {
    // START: Start generation
    I2C1->CR1 |= I2C_CR1_START;

    // Wait until the start condition is generated
    while(!(I2C1->SR1 & I2C_SR1_SB));
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
void i2c_addr_write(char saddr) {
    volatile uint32_t tmp;
    I2C1->DR = (uint8_t)(saddr << 1); /* transmit slave address */
    while (!(I2C1->SR1 & 2)); /* wait until addr flag is set */
    tmp = (uint32_t)I2C1->SR2; /* clear addr flag */
}
//-----------------------------------------------------------------------------------------------
void i2c_data_write(uint8_t data) {
    while (!(I2C1->SR1 & 0x80)){} /* wait until data register empty */
    I2C1->DR = data; /* send memory address */
}
//-----------------------------------------------------------------------------------------------
void i2c_stop(void) {
    // STOP: Stop generation
    I2C1->CR1 |= I2C_CR1_STOP;
    // Wait until the stop condition is generated
    while(I2C1->CR1 & I2C_CR1_STOP);
}
//-----------------------------------------------------------------------------------------------
void delay(void) {
    volatile int i = 0;
    for (i = 0; i < 60000; i++);
}
//-----------------------------------------------------------------------------------------------
