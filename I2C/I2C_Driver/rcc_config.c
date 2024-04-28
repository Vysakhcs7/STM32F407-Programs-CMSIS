  #include "rcc_config.h"
	void rcc_config(void)
	{
		
		// Enable clock access for I2C1
    RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
  
    // Enable clock access for GPIOB
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;
	}