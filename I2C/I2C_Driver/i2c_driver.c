#include "i2c_driver.h"

void i2c_init(void)
{
		//PE: Peripheral enable
		I2C1->CR1 &= ~I2C_CR1_PE;
	
    // Clock frequency - 16MHz
    I2C1->CR2 = 0x10;
	
		/* standard mode, 100kHz clock */  
	  I2C1->CCR = 80; 
	
	  /* maximum rise time */
    I2C1->TRISE = 17; 
	
    // ACK: Acknowledge enable
    I2C1->CR1 |= I2C_CR1_ACK;
		
		//ITEVTEN: Event interrupt enable
		//I2C2->CR2 |= I2C_CR2_ITEVTEN;
		
		//PE: Peripheral enable
		I2C1->CR1 |= I2C_CR1_PE;
} 
//-----------------------------------------------------------------------------------------------
void i2c_start(void)
{
	//START: Start generation
		I2C1->CR1 |= I2C_CR1_START;
	
	// Wait until start condition is generated
    while(!(I2C1->SR1 & I2C_SR1_SB));
}

//-----------------------------------------------------------------------------------------------
void i2c_stop(void)
{
	//STOP: Stop generation
	I2C1->CR1 |= I2C_CR1_STOP;
	
	// Wait until stop condition is generated
  while(I2C1->CR1 & I2C_CR1_STOP);
}

//-----------------------------------------------------------------------------------------------
void i2c_addr_write(uint8_t addr)
{
	I2C1->DR = addr;
	
	/* wait until addr flag is set */
	while (!(I2C1->SR1 & 2));
}
//-----------------------------------------------------------------------------------------------
void i2c_data_write(char data)
{
	I2C1->DR = data;
	
	/* wait until data register empty */
	 while (!(I2C1->SR1 & 0x80)); 
}
//-----------------------------------------------------------------------------------------------
void i2c_data_read(void)
{
	  while (!(I2C1->SR1 & 0x40)){} /* Wait until RXNE flag is set */
    char data = I2C1->DR; /* Read data from DR	*/
}

//-----------------------------------------------------------------------------------------------
