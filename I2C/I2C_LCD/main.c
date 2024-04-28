//-----------------------------------------------------------------------------------------------
#include "stm32f407xx.h"

void rcc_config(void);
void gpio_config(void);
void i2c_config(void);
void i2c_start(void);
void i2c_stop(void);
void LCD_SendData(unsigned char val);
void LCD_SendCommand(unsigned char command);
unsigned char PCF8574_ADDRESS = 0x3F; //I2C slave address
void i2c_data_write(uint8_t data);
  void delay(void);
//-----------------------------------------------------------------------------------------------

int main(void){
	rcc_config();
	gpio_config();
  	i2c_config();
 LCD_SendCommand(0x28); //4bit mode
	 LCD_SendCommand(0x0E); //Display ON, Cursor ON
    LCD_SendCommand(0x01); //Clear Display Screen
    LCD_SendCommand(0x80); //Force cursor to begin on 1st row
    LCD_SendData('L'); //Passing a single character to the LCD
    LCD_SendCommand(0xC0); //Change position to 1st coloumn of 2nd row


	while(1) {

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
    //GPIOB->PUPDR |= GPIO_PUPDR_PUPD6_0 | GPIO_PUPDR_PUPD7_0;
}

//-----------------------------------------------------------------------------------------------
void i2c_config(void) {
	// Reset the I2C
	I2C1->CR1 |= I2C_CR1_SWRST;  
	
	// Normal operation
	I2C1->CR1 &= ~I2C_CR1_SWRST;  
	
    // Clock frequency - 16MHz
    I2C1->CR2 = 0x10;
	
	/* Standard mode, 100kHz clock */  
	I2C1->CCR = 80; 
	
	/* Maximum rise time */
    I2C1->TRISE = 17; 
	
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
void i2c_stop(void) {
	// STOP: Stop generation
	I2C1->CR1 |= I2C_CR1_STOP;
	
	// Wait until the stop condition is generated
    while(I2C1->CR1 & I2C_CR1_STOP);
}

//-----------------------------------------------------------------------------------------------
void i2c_data_write(uint8_t data) {
	while (!(I2C1->SR1 & I2C_SR1_TXE));
	I2C1->DR = data;
	while (!(I2C1->SR1 & I2C_SR1_BTF));
}
//-----------------------------------------------------------------------------------------------
void LCD_SendCommand(unsigned char command) {
    unsigned char temp, cmd;
    //Masking the MSB 4 bits
    temp = command & 0xF0;
    //Backlight ON(P3 =1) and Register Select (RS) = 0 for command mode.
    cmd = (temp | 0x08) & (~0x01);
   	i2c_start();
i2c_data_write(PCF8574_ADDRESS << 1);
    //EN(Enable bit, P2) = 1.
i2c_data_write(cmd | 0x04);
    delay();
    //EN(Enable bit, P2) = 0.
i2c_data_write(cmd & (~0x04));
	i2c_stop();

    //Shifting the 4 LSB bit to MSB.
    temp = command << 4;
    //Backlight ON(P3 =1) and Register Select (RS) = 0 for command mode.
    cmd = (temp | 0x08) & (~0x01);
   	i2c_start();
i2c_data_write(PCF8574_ADDRESS << 1);
    //EN(Enable bit, P2) = 1.
i2c_data_write(cmd | 0x04);
    delay();
    //EN(Enable bit, P2) = 0.
i2c_data_write(cmd & (~0x04));
	i2c_stop();
}

//----------------------------------------------------------------------------
//Function to send single character to LCD 

void LCD_SendData(unsigned char val) {
    unsigned char k, str;
    //Masking the MSB 4 bits
    k = val & 0xF0;
    //Backlight ON(P3 =1) and Register Select (RS) = 1 for data mode.
    str = k | 0x08 | 0x01;
   	i2c_start();
i2c_data_write(PCF8574_ADDRESS << 1);
    //EN(Enable bit, P2) = 1.
i2c_data_write(str | 0x04);
    delay();
    //EN(Enable bit, P2) = 0.
i2c_data_write(str & (~0x04));
	i2c_stop();

    //Shifting the 4 LSB bit to MSB.
    k = val << 4;
    //Backlight ON(P3 =1) and Register Select (RS) = 1 for data mode.
    str = k | 0x08 | 0x01;
   	i2c_start();
i2c_data_write(PCF8574_ADDRESS << 1);
    //EN(Enable bit, P2) = 1.
i2c_data_write(str | 0x04);
     delay();
    //EN(Enable bit, P2) = 0.
i2c_data_write(str & (~0x04));
	i2c_stop();
}
  void delay(void)
    {
      int i = 0;
      for (i = 0; i < 70000; i++);
    }