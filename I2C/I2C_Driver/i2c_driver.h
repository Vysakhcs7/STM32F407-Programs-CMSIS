#include "stm32f407xx.h"
#define DS1307_ADDRESS 0x68

void i2c_init(void);
void i2c_start(void);
void i2c_stop(void);
void i2c_addr_write(uint8_t addr);
void i2c_data_read(void);
void i2c_data_write(char data);

