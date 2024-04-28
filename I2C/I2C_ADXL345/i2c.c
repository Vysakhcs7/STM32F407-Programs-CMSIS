#include "i2c.h"
//pb8 - scl
//pb9 - sda
void i2c_init(void)
{

  
    // Enable clock access for GPIOB
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

	// 10: Alternate function mode
    GPIOB->MODER &= ~(1U<<16);
	GPIOB->MODER |= (1U<<17);
  
	    GPIOB->MODER &= ~(1U<<18);
	GPIOB->MODER |= (1U<<19);
 
  
    // 1: Output open-drain
    GPIOB->OTYPER |= (1U<<8);
	 GPIOB->OTYPER |= (1U<<9);
	 
	  //11: Very high speed
	  //GPIOB->OSPEEDR |= GPIO_OSPEEDER_OSPEEDR6 | GPIO_OSPEEDER_OSPEEDR7;
	
	  //01: Pull-up
    GPIOB->PUPDR |= (1U<<16);
		GPIOB->PUPDR &= ~(1U<<17);
	
	    GPIOB->PUPDR |= (1U<<18);
		GPIOB->PUPDR &= ~(1U<<19);
		
		//AF4
		GPIOB->AFR[1] &= ~(1U<<0);
		GPIOB->AFR[1] &= ~(1U<<1);
		GPIOB->AFR[1] |= (1U<<2);
		GPIOB->AFR[1] &= ~(1U<<3);
		
		
		GPIOB->AFR[1] &= ~(1U<<4);
		GPIOB->AFR[1] &= ~(1U<<5);
		GPIOB->AFR[1] |= (1U<<6);
				GPIOB->AFR[1] &= ~(1U<<7);
				// Enable clock access for I2C1
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
		
	// Reset the I2C
	I2C1->CR1 |= I2C_CR1_SWRST;  
	
	// Normal operation
	I2C1->CR1 &= ~I2C_CR1_SWRST;  
	
    // Clock frequency - 16MHz
    I2C1->CR2 = 0x10;
	
		/* standard mode, 100kHz clock */  
	  I2C1->CCR = 80; 
	
	  /* maximum rise time */
    I2C1->TRISE = 17; 
	
    // ACK: Acknowledge enable
    //I2C1->CR1 |= I2C_CR1_ACK;
		
		//ITEVTEN: Event interrupt enable
		//I2C2->CR2 |= I2C_CR2_ITEVTEN;
		
		//PE: Peripheral enable
		I2C1->CR1 |= I2C_CR1_PE;
} 


//-----------------------------------------------------------------------------------------------
void i2c_byte_read(char saddr, char maddr, char* data)
{
	volatile int tmp;
	
	//Wait until bus is not busy
	  while (!(I2C1->SR2 & I2C_SR2_BUSY)){}
			
//start
		I2C1->CR1 |= I2C_CR1_START;
	
	// Wait until start flag is set
    while(!(I2C1->SR1 & I2C_SR1_SB)){}
			
			//Transmit slave address write
			I2C1->DR = saddr << 1;
	
	/* wait until data register empty */ //wait until address flag is set
	 while (!(I2C1->SR1 & I2C_SR1_ADDR)){} 
			
			//clear addr flag
			tmp = I2C1->SR2;
			
			//send Memory address
			I2C1->DR = maddr;
			
		 //wait until transmitter empty
			while (!(I2C1->SR1 & I2C_SR1_TXE)){} 
				
				//repeated start
						I2C1->CR1 |= I2C_CR1_START;
				
				// Wait until start condition is generated
    while(!(I2C1->SR1 & I2C_SR1_SB)){}

						//Transmit slave address + read
			I2C1->DR = saddr << 1 | 1;
			
			//wait until address flag is set
	 while (!(I2C1->SR1 & I2C_SR1_ADDR)){} 
		 
		 //disable ack
		 I2C1->CR1 &= ~I2C_CR1_ACK;
		 
		 			//clear addr flag
			tmp = I2C1->SR2;
		 
		 	//STOP: Stop generation
	I2C1->CR1 |= I2C_CR1_STOP;
	
	// Wait until RXNE FLAG IS SET
			while (!(I2C1->SR1 & I2C_SR1_RXNE)){} 
				
				//READ DATA from dr
				*data++ = I2C1->DR;
}

//-----------------------------------------------------------------------------------------------
void i2c_burst_read(char saddr, char maddr, int n, char* data)
{
	volatile int tmp;

	//Wait until bus is not busy
	  while (!(I2C1->SR2 & I2C_SR2_BUSY)){}
			
//start
		I2C1->CR1 |= I2C_CR1_START;
	
	// Wait until start flag is set
    while(!(I2C1->SR1 & I2C_SR1_SB)){}
			
			//Transmit slave address write
			I2C1->DR = saddr << 1;
	
	/* wait until data register empty */ //wait until address flag is set
	 while (!(I2C1->SR1 & I2C_SR1_ADDR)){} 
			
			//clear addr flag
			tmp = I2C1->SR2;
			
		 //wait until transmitter empty
			while (!(I2C1->SR1 & I2C_SR1_TXE)){} 
				
							//send Memory address
			I2C1->DR = maddr;
				
						 //wait until transmitter empty
			while (!(I2C1->SR1 & I2C_SR1_TXE)){} 
				
								//repeated start
						I2C1->CR1 |= I2C_CR1_START;
				
								// Wait until start condition is generated
    while(!(I2C1->SR1 & I2C_SR1_SB)){}

						//Transmit slave address + read
			I2C1->DR = saddr << 1 | 1;
			
			//wait until address flag is set
	 while (!(I2C1->SR1 & I2C_SR1_ADDR)){} 
		 
		 			//clear addr flag
			tmp = I2C1->SR2;
		 
			 //enable ack
		 I2C1->CR1 |= I2C_CR1_ACK;
		 
		 while(n > 0U)
		 {
			 //if 1 byte
			 if(n == 1U)
			 {
			   //disable ack
		     I2C1->CR1 &= ~I2C_CR1_ACK;
				 
				 		 	//STOP: Stop generation
	I2C1->CR1 |= I2C_CR1_STOP;
	
	// Wait until RXNE FLAG IS SET
			while (!(I2C1->SR1 & I2C_SR1_RXNE)){} 
				
								//READ DATA from dr
				*data++ = I2C1->DR;
				break;
				
			 }
			 else
				 {
					 	// Wait until RXNE FLAG IS SET
			      while (!(I2C1->SR1 & I2C_SR1_RXNE)){} 
					 	(*data++) = I2C1->DR;
							n--;
			   }
		 }
		 
		 
}



void i2c_burst_write(char saddr, char maddr, int n, char* data)
{
		volatile int tmp;

	//Wait until bus is not busy
	  while (!(I2C1->SR2 & I2C_SR2_BUSY)){}
			
//start
		I2C1->CR1 |= I2C_CR1_START;
	
	// Wait until start flag is set
    while(!(I2C1->SR1 & I2C_SR1_SB)){}
			
			//Transmit slave address write
			I2C1->DR = saddr << 1;
			
				/* wait until data register empty */ //wait until address flag is set
	 while (!(I2C1->SR1 & I2C_SR1_ADDR)){} 
			
			//clear addr flag
			tmp = I2C1->SR2;
			
		 //wait until transmitter empty
			while (!(I2C1->SR1 & I2C_SR1_TXE)){} 
				
							//send Memory address
			I2C1->DR = maddr;
				
				for(int i = 0; i<n; i++)
				{
					//wait until data register empty
			     while (!(I2C1->SR1 & I2C_SR1_TXE)){} 
						 
						 I2C1->DR = *data++;
					
				}
				//wait until transfer finished
			     while (!(I2C1->SR1 & I2C_SR1_BTF)){} 
						 
						 		 	//STOP: Stop generation
	I2C1->CR1 |= I2C_CR1_STOP;
				
}