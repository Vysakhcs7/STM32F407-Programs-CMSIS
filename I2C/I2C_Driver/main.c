//-----------------------------------------------------------------------------------------------
/*

Program the peripheral input clock in I2C_CR2 register in order to generate correct 
timings
 Configure the clock control registers
 Configure the rise time register
 Program the I2C_CR1 register to enable the peripheral
 Set the START bit in the I2C_CR1 register to generate a Start condition
The peripheral input clock frequency must be at least:
 2 MHz in Sm mode 
 4 MHz in Fm mode

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
#include "rcc_config.h"
#include "gpio_config.h"
#include "i2c_driver.h"

//-----------------------------------------------------------------------------------------------

int main(void){
	rcc_config();
	gpio_config();
  i2c_init();
 while(1)
	{
	i2c_start();
  i2c_addr_write(DS1307_ADDRESS << 1); //Addr + W
	i2c_data_write(0); //Memory address
	i2c_start(); //Repeated start
	i2c_addr_write(DS1307_ADDRESS | 1); //Addr + R
	i2c_stop();
  }
}

//-----------------------------------------------------------------------------------------------