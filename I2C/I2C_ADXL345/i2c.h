#include "stm32f407xx.h"


void i2c_init(void);
void i2c_byte_read(char saddr, char maddr, char* data);
void i2c_burst_read(char saddr, char maddr, int n, char* data);
void i2c_burst_write(char saddr, char maddr, int n, char* data);

