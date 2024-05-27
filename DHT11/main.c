
//-----------------------------------------------------------------------------------------------
/*
*Program to interface DHT11 with the STM32F407 discovery board.

GPIOA -> PIN 1  -> DHT11

Steps:



Author            : Vysakh C S
Date              : 10-02-2024
Development board : STM32F407VGT6 Discovery Board
IDE               : Keil uVision V5.39.0.0
*/

//-----------------------------------------------------------------------------------------------

#include <stdio.h>
#include "stm32f407xx.h"

#include<stdint.h>

// Function prototypes
void uart2_config(void);
void uart2_single_write(int ch);
void delay(void);
int __io_putchar(int ch);
void read_dht11(uint8_t *temperature, uint8_t *humidity);
void i2c_gpio_config(void);

// Retargeting printf() to USART2
int __io_putchar(int ch) {
    while (!(USART2->SR & USART_SR_TXE)); // Wait for TX buffer to be empty
    USART2->DR = (ch & 0xFF);             // Transmit character
    return ch;
}


// Redirect STDOUT to USART2
FILE __stdout;
int fputc(int ch, FILE *f) {
  return __io_putchar(ch);
}



void i2c_start(void);
void i2c_addr_write(char saddr);
void i2c_stop(void);

void lcd_send_cmd(unsigned char command);
void i2c_data_write(uint8_t data);
void lcd_send_data(unsigned char val);
void lcd_send_string(char *stringValue);
#define SLAVE_ADDR 0x3F


void clock_config(void);
void gpio_input(void);
void gpio_output(void);
void mcu_send_start(void);
void dht11_response(void);
void pin_high(void);
void pin_low(void);
void delay_us(uint32_t us);
int inputValue = 0;
 int c = 0;
void read(void);
//-----------------------------------------------------------------------------------------------
int main(void)
{
   uint8_t temperature, humidity;
  clock_config();
    uart2_config();
  while(1)
  { 
    i2c_gpio_config();
   i2c_config();
    lcd_send_cmd(0x28); // 4-bit mode
    delay_us(60000); 
    lcd_send_cmd(0x0C); // Display ON, Cursor ON
    delay_us(60000); 
    lcd_send_cmd(0x01); // Clear Display Screen
     delay_us(60000); 

		   mcu_send_start(); 
		   dht11_response();
       
		   read_dht11(&temperature, &humidity);
        //printf("Temperature: %d\n", temperature);
        //printf("Humidity: %d%%\n", humidity);
        lcd_send_cmd(0x80); // Force cursor to begin on 1st row
        //sprintf(ab,"%u",a);
       // lcd_send_string(ab);
	     delay_us(500000); //500ms	
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
  
   /*UART*/
  
  /*PA2 set to alternate function mode */
  GPIOA->MODER |= GPIO_MODER_MODE2_1;
  GPIOA->MODER &= ~GPIO_MODER_MODE2_0;
  
  /* PA2 set to AF7 */
  /* GPIOA->AFR[0] is the low register */
  GPIOA->AFR[0] |=  GPIO_AFRL_AFRL2_2 | GPIO_AFRL_AFRL2_1 | GPIO_AFRL_AFRL2_0;
  GPIOA->AFR[0] &= ~GPIO_AFRL_AFRL2_3;

//lcd
   // Enable clock access for I2C1
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;

    // Enable clock access for GPIOB
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
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
  gpio_input();
	}
//-----------------------------------------------------------------------------------------------
	void dht11_response(void)
	{
			
      inputValue = (GPIOA->IDR & GPIO_IDR_ID1)? 1 : 0;
    if(inputValue == 0)
     {
       //printf("54: %d\n\r",inputValue);
       delay_us(54); //54us
     }
      
     inputValue = (GPIOA->IDR & GPIO_IDR_ID1)? 1 : 0;
     if(inputValue == 1)
     {
       //printf("80: %d\n\r",inputValue);
       
       delay_us(80); //80us
     }
	}
//-----------------------------------------------------------------------------------------------
/*UART configurations */
void uart2_config(void)
{
  /* Bit 17 USART2EN: USART2 clock enable */
  /* 1: USART2 clock enabled */
  RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
  
  /* USART Baud Rate */
  /* 9600 baud @ 16 MHz */
  USART2->BRR = 0x683; 
  
  /* Bit 3 TE: Transmitter enable */
  /* 1: Transmitter is enabled */
   USART2->CR1 |= USART_CR1_TE;
  
  /* Bit 13 UE: USART enable */
  /* 1: USART enabled */
  USART2->CR1 |= USART_CR1_UE;
  
  /* Bit 12 M: Word length */
  /* 0: 1 Start bit, 8 Data bits, n Stop bit */
  USART2->CR1 &= ~USART_CR1_M;
  
  /* Bits 13:12 STOP: STOP bit */
  /* 00: 1 Stop bit */
  USART2->CR2 &= ~USART_CR2_STOP_1;
  USART2->CR2 &= ~USART_CR2_STOP_0;
}
//-----------------------------------------------------------------------------------------------
/* Write a character to UART2 */
void uart2_single_write (int ch) 
{
     /* wait until Tx buffer empty */
     while (!(USART2->SR & USART_SR_TXE)) {}
     USART2->DR = (ch & 0XFF);
}
//-----------------------------------------------------------------------------------------------
void read_dht11(uint8_t *temperature, uint8_t *humidity)
{
    uint8_t data[5] = {0};
    // Data is transmitted in 40 bits, first 2 bytes are humidity data, the next 2 bytes are temperature data, and the last byte is checksum
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 8; j++) {
            // Wait for the falling edge of the signal
            while (!(GPIOA->IDR & GPIO_IDR_ID1));
            
            // Measure the width of the pulse
            delay_us(30); // DHT11 data pin will be low for 26-28 us for '0' and 70us for '1'
            if (GPIOA->IDR & GPIO_IDR_ID1) { // Check if the pin is still high after the delay
                data[i] |= (1 << (7 - j)); // Store '1'
            }
            // Wait for the signal to go back high
            while (GPIOA->IDR & GPIO_IDR_ID1);
        }
    }
    // Checksum verification
    if ((data[0] + data[1] + data[2] + data[3]) == data[4]) {
        *humidity = data[0];
        *temperature = data[2];
    } else {
        // Checksum error
        *humidity = 0;
        *temperature = 0;
    }
}
//-------------------------------------------------------------------------------------------------------------
void i2c_gpio_config(void) {
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
//-----------------------------------------------------------------------------------------------	
void lcd_send_string(char *stringValue) {
    while ((*stringValue) != '\0') {
        lcd_send_data(*stringValue);
        stringValue++;
			//stringValue = (char *)((uint32_t)stringValue + 1);
    }
}
//-----------------------------------------------------------------------------------------------	