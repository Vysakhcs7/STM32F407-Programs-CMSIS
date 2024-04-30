//-----------------------------------------------------------------------------------------------
#include "stm32f407xx.h"
#include <stdio.h>
//-----------------------------------------------------------------------------------------------
void rcc_config(void);
void gpio_config(void);
void i2c_config(void);

void i2c_start(void);
void i2c_addr_write(char saddr);
void i2c_stop(void);

void lcd_send_cmd(unsigned char command);
void i2c_data_write(uint8_t data);
void lcd_send_data(unsigned char val);
void lcd_send_string(char *stringValue);
#define SLAVE_ADDR 0x3F
void delay(void);
//-----------------------------------------------------------------------------------------------
int main(void){
    uint32_t a = 0;
    char ab[4];
    rcc_config();
    gpio_config();
    i2c_config();
    lcd_send_cmd(0x28); // 4-bit mode
    delay();
    lcd_send_cmd(0x0C); // Display ON, Cursor ON
    delay();
    lcd_send_cmd(0x01); // Clear Display Screen
    delay();
	//lcd_send_data('*'); //Passing a single character to the LCD
	//delay();
	//lcd_send__cmd(0xC0); //Change position to 1st coloumn of 2nd row
	//delay();
	//lcd_send__string("HELLO"); ////Passing a string to the LCD
    while(1) {
        a++;
        lcd_send_cmd(0x80); // Force cursor to begin on 1st row
        sprintf(ab,"%u",a);
        lcd_send_string(ab);
        delay();
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
void i2c_addr_write(char saddr) {
    volatile uint32_t tmp;
    I2C1->DR = (uint8_t)(saddr << 1); /* transmit slave address */
    while (!(I2C1->SR1 & 2)); /* wait until addr flag is set */
    tmp = (uint32_t)I2C1->SR2; /* clear addr flag */
}
//-----------------------------------------------------------------------------------------------
void lcd_send_cmd(unsigned char command) {
    unsigned char temp, cmd;
    // Masking the MSB 4 bits
    temp = command & 0xF0;
    // Backlight ON(P3 =1) and Register Select (RS) = 0 for command mode.
    cmd = (temp | 0x08) & (~0x01);
    i2c_start();
    i2c_addr_write(SLAVE_ADDR);
    // EN(Enable bit, P2) = 1.
    i2c_data_write(cmd | 0x04);
    delay();
    // EN(Enable bit, P2) = 0.
    i2c_data_write(cmd & (~0x04));
    i2c_stop();
	
    // Shifting the 4 LSB bit to MSB.
    temp = (uint8_t)(command << 4);
    // Backlight ON(P3 =1) and Register Select (RS) = 0 for command mode.
    cmd = (temp | 0x08) & (~0x01);
    i2c_start();
    i2c_addr_write(SLAVE_ADDR);
    // EN(Enable bit, P2) = 1.
    i2c_data_write(cmd | 0x04);
    delay();
    // EN(Enable bit, P2) = 0.
    i2c_data_write(cmd & (~0x04));
    i2c_stop();
}
//-----------------------------------------------------------------------------------------------
void i2c_data_write(uint8_t data) {
    while (!(I2C1->SR1 & 0x80)){} /* wait until data register empty */
    I2C1->DR = data; /* send memory address */
}
//-----------------------------------------------------------------------------------------------
void lcd_send_data(unsigned char val) {
    unsigned char k, str;
    // Masking the MSB 4 bits
    k = val & 0xF0;
    // Backlight ON(P3 =1) and Register Select (RS) = 1 for data mode.
    str = k | 0x08 | 0x01;
    i2c_start();
    i2c_addr_write(SLAVE_ADDR);

    // EN(Enable bit, P2) = 1.
    i2c_data_write(str | 0x04);
    delay();
    // EN(Enable bit, P2) = 0.
    i2c_data_write(str & (~0x04));
    i2c_stop();

    // Shifting the 4 LSB bit to MSB.
    k = (uint8_t)(val << 4);
    // Backlight ON(P3 =1) and Register Select (RS) = 1 for data mode.
    str = k | 0x08 | 0x01;
    i2c_start();
    i2c_addr_write(SLAVE_ADDR);
    // EN(Enable bit, P2) = 1.
    i2c_data_write(str | 0x04);
    delay();
    // EN(Enable bit, P2) = 0.
    i2c_data_write(str & (~0x04));
    i2c_stop();
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
void lcd_send_string(char *stringValue) {
    while ((*stringValue) != '\0') {
        lcd_send_data(*stringValue);
        stringValue++;
			//stringValue = (char *)((uint32_t)stringValue + 1);
    }
}
//-----------------------------------------------------------------------------------------------	